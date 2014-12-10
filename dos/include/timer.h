/*
 *  Copyright (C) 2002-2013  The DOSBox Team
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

#ifndef DOSBOX_TIMER_H
#define DOSBOX_TIMER_H

#include "ldb.h"

namespace dosbox {

//ripped from time.h
typedef struct Sprtc_data {
	int32_t tm_sec;			/* Seconds.	[0-60] (1 leap second) */
	int32_t tm_min;			/* Minutes.	[0-59] */
	int32_t tm_hour;		/* Hours.	[0-23] */
	int32_t tm_mday;		/* Day.		[1-31] */
	int32_t tm_mon;			/* Month.	[0-11] */
	int32_t tm_year;		/* Year	- 1900.  */
	int32_t tm_wday;		/* Day of week.	[0-6] */
	int32_t tm_yday;		/* Days in year.[0-365]	*/
	int32_t	tm_millis;		/* Milliseconds (added) */
} prtc_data;

#define PIT_TICK_RATE 1193182

//#define GetTicks() XS_GetTicks()
uint32_t GetTicks();
extern LDB_CallbackFunc libdosbox_callbacks[];
void TIMER_Delay(Bitu c);

typedef void (*TIMER_TickHandler)(void);

/* Register a function that gets called everytime if 1 or more ticks pass */
void TIMER_AddTickHandler(TIMER_TickHandler handler);
void TIMER_DelTickHandler(TIMER_TickHandler handler);

/* This will add 1 milliscond to all timers */
void TIMER_AddTick(void);

/* Get the pseudo-RTC data */
void PRTC_GetDateTime(prtc_data* buf);

}

#endif
