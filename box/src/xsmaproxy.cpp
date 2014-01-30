/*
 * xsmaproxy.cpp
 *
 *  Created on: Jan 30, 2014
 */

#include <vector>
#include <list>
#include <string.h>
#include <ctype.h>
#include <stdarg.h>
#include <stdio.h>
#include <assert.h>
#include "dosbox.h"
#include "video.h"
#include "keyboard.h"
#include "support.h"
#include "mapper.h"
#include "setup.h"

void MAPPER_Init(void)
{
}

void MAPPER_AddHandler(MAPPER_Handler * handler,MapKeys key,Bitu mods,char const * const eventname,char const * const buttonname)
{
}

void Mouse_AutoLock(bool enable)
{
}
