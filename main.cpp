/*
 * main.cpp
 *
 *  Created on: Mar 6, 2014
 *      Author: master
 */

#include <string>
#include <vector>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

typedef struct SxsSDL {
	SDL_AudioDeviceID audio;
	SDL_Window* wnd;
	SDL_Renderer* ren;
	TTF_Font* fnt;
	SDL_Color txtcol;
} XSSDL;

static XSSDL sdl;

int main(void)
{
	std::string mystr;
	sdl.wnd = NULL;
	sdl.ren = NULL;
	sdl.audio = 0;
	if (SDL_Init(SDL_INIT_VIDEO|SDL_INIT_AUDIO)) {
		printf("SDL2 Init Error");
		return 1;
	}
	if (TTF_Init()) {
		printf("SDL2 TTF Init Error");
		return 2;
	}
	if (SDL_CreateWindowAndRenderer(640,480,SDL_WINDOW_RESIZABLE,&sdl.wnd,&sdl.ren)) {
		printf("CreateWindowAndRenderer() failed.");
		return 3;
	}
	sdl.fnt = TTF_OpenFont("LiberationMono-Regular.ttf",12);
	if (!sdl.fnt) {
		printf("Couldn't load font");
		return 4;
	}
	sdl.txtcol.a = 0xff;
	sdl.txtcol.r = 0xff;
	sdl.txtcol.g = 0;
	sdl.txtcol.b = 0xff;
	SDL_SetWindowTitle(sdl.wnd,"TTF test");
	SDL_RenderClear(sdl.ren);
	SDL_RenderPresent(sdl.ren);

	SDL_Event e;
	unsigned int i;
	bool quit = false;
	printf("Loop begins");
	do {
		while (SDL_PollEvent(&e)) {
			switch (e.type) {
			case SDL_QUIT:
				quit = true;
				break;

			case SDL_KEYUP:
				mystr += static_cast<char> (e.key.keysym.sym);
				printf("%s\n",mystr.c_str());
				break;

			default:
				break;
			}
		}

		//

		SDL_Delay(5);
	} while (!quit);

	if (sdl.audio) SDL_CloseAudioDevice(sdl.audio);
	if (sdl.ren) SDL_DestroyRenderer(sdl.ren);
	if (sdl.wnd) SDL_DestroyWindow(sdl.wnd);
	if (sdl.fnt) TTF_CloseFont(sdl.fnt);
	TTF_Quit();
	SDL_Quit();
	return 0;
}
