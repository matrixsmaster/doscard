/*
 *  Copyright (C) 2013-2014  Dmitry Soloviov
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */

#ifndef XSUPPORT_H_
#define XSUPPORT_H_

#include <string.h>
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <SDL2/SDL.h>

#define XSUPP_MAX_MESSAGE 2048
#define XSUPP_MAX_PREFIX 64

#define XS_CONTEXT_COUNT 17
static const char xs_contexts[XS_CONTEXT_COUNT][XSUPP_MAX_PREFIX] = {
		"Undefined context","XSHELL2 MAIN","SDLInit","SDLKill","DrawUI",
		"SDLoop","PushMachine","PopMachine","ClearMachines","UpdateMachine",
		"AddMachineEvents","PauseActive","SDLInitAudio","XS_AudioCallback",
		"ModILeaveActive","ToggleUnlockActive","ParseArgs"
};

/*
 * xnfo(int sev, int ctx, char const* format,...)
 * sev: severity (-1-fatal; 0-info; 1-error;)
 * ctx: context (function number)
 */
void xnfo(int sev, int ctx, char const* format,...);

#endif /* XSUPPORT_H_ */
