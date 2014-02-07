#include <vector>
#include <string.h>
#include <inttypes.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include "SDL.h"

#define DISPLAY_INIT_SIGNATURE 0xFFABCD00
#define DISPLAY_NFRM_SIGNATURE 0xAABBCCDD
#define XSHELL_CAPTION "SDL2 Test"
#define XSHELL_DEF_WND_W 640
#define XSHELL_DEF_WND_H 480

enum LDB_UIEventE {
	LDB_UIE_NONE = 0,
	LDB_UIE_QUIT = 1,
	LDB_UIE_KBD = 2,
	LDB_UIE_MOUSE = 3
};

typedef struct {
	LDB_UIEventE t; 	//event type
	SDL_Scancode key;		//keyboard key
	int32_t m_x,m_y,m_b;//mouse X Y and buttons
	bool pressed;		//is pressed?
} LDB_UIEvent;

using namespace std;

static SDL_Thread* dosbox = NULL;
static SDL_Window* wnd = NULL;
static SDL_Renderer* ren = NULL;
static uint8_t disp_fsm = 0;
static uint8_t pxclock;
static uint16_t lcd_w,lcd_h;
static uint32_t frame_cnt,cur_pixel,frame_crc;
static uint32_t* framebuf = NULL;
static SDL_Texture* frame_sdl = NULL;
static bool frame_dirty = false;
static SDL_mutex* frame_mutex = NULL;
static SDL_mutex* evt_mutex = NULL;
static vector<LDB_UIEvent> evt_fifo;
static int frame_block = 0;
static bool quit = false;
static struct timespec* clkres = NULL;
static uint32_t* clkbeg = NULL;
SDL_atomic_t at_flag;

int XS_UpdateScreenBuffer(void* buf, size_t len)
{
	if (!buf) return -1;
	uint32_t* dw;
	uint8_t* b;
	switch (len) {
	case 1:
		b = reinterpret_cast<uint8_t*>(buf);
		if (disp_fsm == 2) {
			if (frame_cnt >= static_cast<uint32_t>(lcd_w*lcd_h)) {
				disp_fsm = 1;
//				SDL_UnlockMutex(frame_mutex);
				SDL_AtomicSet(&at_flag,0);
			} else {
				cur_pixel = (pxclock)? (cur_pixel|(*b << (pxclock*8))):(*b);
				if (++pxclock > 2) {
					pxclock = 0;
					framebuf[frame_cnt++] = cur_pixel;
					frame_crc += cur_pixel;
				}
			}
		}
		break;

	case 4:
		dw = reinterpret_cast<uint32_t*>(buf);
		if (*dw == DISPLAY_INIT_SIGNATURE) {
			printf("XSUSB: Init received\n");
			if (!disp_fsm) disp_fsm = 1;
		} else if (*dw == DISPLAY_NFRM_SIGNATURE) {
			disp_fsm = 1;
		} else if (disp_fsm == 1) {
//			if (SDL_LockMutex(frame_mutex)) {
//				printf("XSUSB: Couldn't lock frame mutex: %s\n",SDL_GetError());
//				abort();
//			}
			while (SDL_AtomicGet(&at_flag) != 2) usleep(5);
			printf("update\n");

			uint16_t old_w = lcd_w;
			uint16_t old_h = lcd_h;
			lcd_w = (*dw >> 16);
			lcd_h = *dw & 0xffff;
			pxclock = 0;
			frame_cnt = 0;
			frame_crc = 0;
			disp_fsm = 2;
			if ((!framebuf) || (old_w*old_h != lcd_w*lcd_h)) {
				framebuf = reinterpret_cast<uint32_t*>(realloc(framebuf,sizeof(uint32_t)*lcd_w*lcd_h));
				frame_dirty = true;
				printf("XSUSB: Frame resized\n");
			}
		}
		break;
	default:
		printf("Unsupported transfer length (%d)\n",len);
		return 1;
	}
	return 0;
}

int XS_QueryUIEvents(void* buf, size_t len)
{
	if ((!buf) || (len < sizeof(LDB_UIEvent))) return -1;
//	if (SDL_LockMutex(frame_mutex)) {
//		printf("XSQUE: Couldn't lock event buffer mutex!\n");
//		abort();
//	}

	while (SDL_AtomicGet(&at_flag) != 1) usleep(10);
	printf("query\n");

	int r = evt_fifo.size();
	if (!evt_fifo.empty()) {
		LDB_UIEvent e = evt_fifo.back();
		evt_fifo.pop_back();
		memcpy(buf,&e,sizeof(LDB_UIEvent));
	}
//	SDL_UnlockMutex(frame_mutex);

	SDL_AtomicSet(&at_flag,2);
	return r;
}

int XS_GetTicks(void* buf, size_t len)
{
	uint32_t* val = reinterpret_cast<uint32_t*>(buf);
	struct timespec r;
	if ((!buf) || (len < 4)) return -1;
	if (!clkres) {
		clkres = new struct timespec;
		clock_getres(CLOCK_MONOTONIC,clkres);
		if (clkres->tv_nsec > 1000000)
			printf("CLOCK_MONOTONIC resolution is too low!\n");
	}
	clock_gettime(CLOCK_MONOTONIC,&r);
	if (!clkbeg) {
		clkbeg = new uint32_t;
		printf("Clocks start point set to %d:%d\n",r.tv_sec,r.tv_nsec);
		*clkbeg = r.tv_sec * 1000 + (r.tv_nsec / 1000000);// / clkres->tv_nsec);
		*val = 0;
	} else {
		*val = r.tv_sec * 1000 + (r.tv_nsec / 1000000);
		*val -= *clkbeg;
	}
	return 0;
}

static int XS_SDLInit()
{
	if (SDL_Init(SDL_INIT_EVERYTHING)) {
		printf("SDL2 Init Error\n");
		return 1;
	}
	if (SDL_CreateWindowAndRenderer(XSHELL_DEF_WND_W,XSHELL_DEF_WND_H,
			SDL_WINDOW_OPENGL,&wnd,&ren)) {
		printf("CreateWindowAndRenderer() failed.\n");
		return 2;
	}
	lcd_w = XSHELL_DEF_WND_W;
	lcd_h = XSHELL_DEF_WND_H;
	if (SDL_SetRenderDrawColor(ren,0,0,0,255)) {
		printf("SetRenderDrawColor() failed.\n");
		return 10;
	}
	SDL_SetWindowTitle(wnd,XSHELL_CAPTION);
	SDL_RenderClear(ren);
	SDL_RenderPresent(ren);

	frame_mutex = SDL_CreateMutex();
	evt_mutex = SDL_CreateMutex();
	if ((!frame_mutex) || (!evt_mutex)) {
		printf("Frame or Event mutex couldn't be created!\n");
		return 20;
	}

	return 0;
}

static void XS_SDLKill()
{
	if (ren) SDL_DestroyRenderer(ren);
	if (wnd) SDL_DestroyWindow(wnd);
	if (frame_sdl) SDL_DestroyTexture(frame_sdl);
	if (frame_mutex) SDL_DestroyMutex(frame_mutex);
	if (evt_mutex) SDL_DestroyMutex(evt_mutex);
	SDL_Quit();
}

static void XS_SDLoop()
{
	SDL_Event e;
	LDB_UIEvent mye;
	printf("Loop begins\n");
//	SDL_CompilerBarrier();
	for(;;) {

		/* Event Processing*/
//		if (SDL_LockMutex(frame_mutex)) {
//			printf("Couldn't lock event buffer mutex: %s\n",SDL_GetError());
//			abort();
//		}
		while (SDL_AtomicGet(&at_flag)) usleep(2);
		printf("loop\n");

		while (SDL_PollEvent(&e)) {
			memset(&mye,0,sizeof(mye));
			switch (e.type) {

			case SDL_QUIT:
				mye.t = LDB_UIE_QUIT;
				break;

			case SDL_KEYDOWN:
				if (e.key.keysym.scancode == SDL_SCANCODE_PAUSE) {
					frame_block ^= 1;
					printf("frame_block=%d\n",frame_block);
					continue;
				}
			case SDL_KEYUP:
				mye.t = LDB_UIE_KBD;
				mye.pressed = (e.type == SDL_KEYDOWN);
				mye.key = e.key.keysym.scancode;
				break;

			default: continue;
			}
			evt_fifo.insert(evt_fifo.begin(),mye);
			if (mye.t == LDB_UIE_QUIT) {
//				SDL_UnlockMutex(evt_mutex);
				return;
			}
		}
//		SDL_UnlockMutex(evt_mutex);

		/* Frame Processing*/
		if (!frame_block) {
//			if (SDL_LockMutex(frame_mutex)) {
//				printf("Couldn't lock frame mutex: %s\n",SDL_GetError());
//				abort();
//			}
			if (framebuf) {
				if (frame_dirty) {
					printf("frame is dirty\n");
					if (frame_sdl) SDL_DestroyTexture(frame_sdl);
					frame_sdl = SDL_CreateTexture(ren,SDL_PIXELFORMAT_ARGB8888,SDL_TEXTUREACCESS_STREAMING,lcd_w,lcd_h);
					frame_dirty = false;
				}
				if (frame_sdl) {
					SDL_UpdateTexture(frame_sdl,NULL,framebuf,lcd_w*sizeof(uint32_t));
					SDL_RenderClear(ren);
					SDL_RenderCopy(ren,frame_sdl,NULL,NULL);
				}
			}
		}
//			SDL_UnlockMutex(frame_mutex);
		SDL_AtomicSet(&at_flag,1);

			/* Update Window*/
			SDL_RenderPresent(ren);
//			SDL_Delay(1);
			usleep(1000);
//		}
	}
}

int TestThread(void*)
{
	uint32_t x = DISPLAY_INIT_SIGNATURE;
	uint32_t i,j,k;
	uint8_t b;
	LDB_UIEvent mye;
	uint32_t ffsize = 0;
	XS_UpdateScreenBuffer(&x,4);
	for(;;) {
		printf("thread\n");
		if (XS_QueryUIEvents(&mye,sizeof(mye))) {
			if (mye.pressed) goto t_next; //switch on release
			switch (mye.t) {
			case LDB_UIE_QUIT:
				quit = true;
				return -1;

			case LDB_UIE_KBD:
				switch (mye.key) {
				case SDL_SCANCODE_1:
					ffsize = (640 << 16) | 480;
					printf("640x480\n");
					break;
				case SDL_SCANCODE_2:
					ffsize = (640 << 16) | 400;
					printf("640x400\n");
					break;
				case SDL_SCANCODE_3:
					ffsize = (320 << 16) | 200;
					printf("320x200\n");
					break;
				case SDL_SCANCODE_4:
					ffsize = (800 << 16) | 600;
					printf("800x600\n");
					break;
				default: break;
				}
				break;

				default: continue;
			}
		}
t_next:
		XS_GetTicks(&x,4);
		XS_UpdateScreenBuffer(&ffsize,4);
		for (i=0; i<(ffsize&0xffff); i++)
			for (j=0; j<(ffsize>>16); j++)
				for (k=0; k<3; k++) {
					b = random() & 255;
					XS_UpdateScreenBuffer(&b,1);
				}
		b = 0xff;
		XS_UpdateScreenBuffer(&b,1); //eof
		XS_GetTicks(&j,4);
		printf("Buffer generation took %d ms\n",j-x);

		x = abs(random());
		while (x > 20000) x >>= 1;
		while (x < 1000) x <<= 1;
		usleep(x);
		if (quit) return 2;
	}
	return 1;
}

int main(int argc, char* argv[])
{
	int r;
	printf("ALIVE!\n");
	if (XS_SDLInit()) return 1;
	printf("SDL2 context created successfully\n");
	SDL_AtomicSet(&at_flag,0);
	dosbox = SDL_CreateThread(TestThread,"TestThread",NULL);
	if (!dosbox) {
		printf("Unable to create DOS thread!");
		XS_SDLKill();
		return -1;
	}
	printf("Test Thread running!\n");
	XS_SDLoop();
	printf("SDLoop() Exited\n");
	SDL_WaitThread(dosbox,&r);
	printf("Test Thread Exited (%d)\n",r);
	XS_SDLKill();
	printf("QUIT\n");
	return (EXIT_SUCCESS);
}
