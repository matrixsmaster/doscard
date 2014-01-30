#include "xshell.h"

/*
 * The Basic model:
 * 1. Run SDL window
 * 2. Start Box thread
 * 2.a. Box thread calls UpdateScreenBuffer() every it's own frame
 * 2.b. Box thread calls UpdateSoundBuffer() every time the buffer is ready
 * 2.c. Box thread calls QueryUIEvents() every time the thread ready to process them
 * 2.note. This mechanism is close enough to what will happen on a real chip
 * (video generator may be a separate chip, which connected to main chip via UART/SPI/I2C
 * or may be just a timer's interrupt handler; audio output controller may be implemented
 * as simple timer interrupt, which updates the other timer's values to change duty
 * cycle of PWM generated; keyboard handler is a pin change interrupt handler too;
 * all of this parts will never affect the main code)
 *
 */

int XS_UpdateScreenBuffer(void* buf, size_t len)
{
	return 0;
}

int XS_UpdateSoundBuffer(void* buf, size_t len)
{
	return 0;
}

int XS_QueryUIEvents(void* buf, size_t len)
{
	return 0;
}

int XS_GetTicks(void* buf, size_t len)
{
	return 0;
}

void ldb_register()
{
#ifdef XSHELL_VERBOSE
	xvrb("ldb_register()");
#endif
	Dosbox_RegisterCallback(DBCB_GetTicks,&XS_GetTicks);
	Dosbox_RegisterCallback(DBCB_PushScreen,&XS_UpdateScreenBuffer);
	Dosbox_RegisterCallback(DBCB_PushSound,&XS_UpdateSoundBuffer);
	Dosbox_RegisterCallback(DBCB_PullUIEvents,&XS_QueryUIEvents);
}

#ifdef XSHELL_VERBOSE
void xvrb(char const* format,...)
{
	char buf[1024];
	va_list msg;
	va_start(msg,format);
	vsprintf(buf,format,msg);
	va_end(msg);
	fprintf(stderr,"%s\n",buf);
}
#endif

int main(int argc, char* argv[])
{
	pthread_t dosbox;
	printf("[XSHELL MAIN]: ALIVE!\n");
	ldb_register();
	dosbox = pthread_create(&dosbox,NULL,Dosbox_Run,NULL);
	SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION,"SDL2","SDL2 Alive",NULL);
	pthread_join(dosbox,NULL);
	printf("[XSHELL MAIN]: EXIT\n");
	return (EXIT_SUCCESS);
}
