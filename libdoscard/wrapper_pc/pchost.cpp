/*
 *  Copyright (C) 2014  Soloviov Dmitry
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

#include "ldbwrap.h"

using namespace dosbox;

namespace doscard {

int LDBCB_LCD(void* buf, size_t len)
{
	printf("LCD\n");
//	uint8_t* b;
	uint32_t* dw;
	if ((!buf) || (!len)) return -1;

	dw = reinterpret_cast<uint32_t*>(buf);
	if (len == 4) {
		switch (*dw) {
		case DISPLAY_INIT_SIGNATURE:
			Runtime->frameskip_cnt = 0;
			if (!Runtime->disp_fsm) Runtime->disp_fsm = 1;
			break;

		case DISPLAY_NFRM_SIGNATURE:
			Runtime->disp_fsm = 1;
			break;

		case DISPLAY_ABOR_SIGNATURE:
			if (Runtime->disp_fsm) {
				Runtime->disp_fsm = 1;
//				if (SDL_AtomicGet(&at_flag) >= 0)
//					SDL_AtomicIncRef(&at_flag);
			}
			break;

		default:
			if (Runtime->disp_fsm == 1) {
//				if (SDL_AtomicGet(&at_flag) > 0) {
//					if (Runtime->frameskip_cnt++ >= FRAMESKIP_MAX) {
//						while (SDL_AtomicGet(&at_flag) > 0) ;
//					} else
//						return DISPLAY_RET_BUSY;
//				}
				Runtime->frameskip_cnt = 0;

				uint16_t old_w = Runtime->lcdw;
				uint16_t old_h = Runtime->lcdh;
				Runtime->lcdw = (*dw >> 16);
				Runtime->lcdh = *dw & 0xffff;
				Runtime->frame_cnt = 0;
				Runtime->disp_fsm = 2;
//				if ((!framebuf) || (old_w*old_h != Runtime->lcdw*Runtime->lcdh)) {
//					framebuf = reinterpret_cast<uint32_t*>(realloc(framebuf,sizeof(uint32_t)*Runtime->lcdw*Runtime->lcdh));
//					Runtime->frame_dirty = true;
//				}
				printf("frm sz = %d x %d\n",Runtime->lcdw,Runtime->lcdh);
				Runtime->crc = 0;
			}
			break;
		}
	} else if ((Runtime->disp_fsm == 2) && (len == Runtime->lcdw * 4)) {
//		memcpy(framebuf+(Runtime->lcdw*Runtime->frame_cnt),buf,len);
		for (uint64_t i=0; i<len/4; i++)
			Runtime->crc += dw[i];
		if (++Runtime->frame_cnt >= Runtime->lcdh) {
//			if (SDL_AtomicGet(&at_flag) >= 0)
//				SDL_AtomicIncRef(&at_flag);
			Runtime->disp_fsm = 1;
			printf("frm crc = %u\n",Runtime->crc);
		}
	} else {
		return -1;
	}
	return 0;
}

int LDBCB_SND(void* buf, size_t len)
{
	printf("SND\n");
	return 0;
}

int LDBCB_UIE(void* buf, size_t len)
{
	printf("UIE\n");
	return 0;
}

int LDBCB_TCK(void* buf, size_t len)
{
	printf("TCK\n");
	if ((!buf) || (len < 4)) return -1;
	uint32_t* val = reinterpret_cast<uint32_t*>(buf);
	struct timespec r;
	if (!Runtime->clkres) {
		Runtime->clkres = new struct timespec;
		clock_getres(CLOCK_MONOTONIC,Runtime->clkres);
	}
	clock_gettime(CLOCK_MONOTONIC,&r);
	if (!Runtime->clkbeg) {
		Runtime->clkbeg = new uint32_t;
		*Runtime->clkbeg = r.tv_sec * 1000 + (r.tv_nsec / 1000000);// / Runtime->clkres->tv_nsec);
		*val = 0;
	} else {
		*val = r.tv_sec * 1000 + (r.tv_nsec / 1000000);
		*val -= *Runtime->clkbeg;
	}
	return 0;
}

int LDBCB_MSG(void* buf, size_t len)
{
	if ((!buf) || (!len)) return -1;
	char* str = reinterpret_cast<char*> (buf);
	printf("LDBCB_MSG: %s\n",str);
	return 0;
}

int LDBCB_FIO(void* buf, size_t len)
{
	uint64_t* x;
	int64_t* sx;
	struct stat tstat;
	struct dirent* pdir;

	if ((!buf) || (len < sizeof(DBFILE))) return -1;
	DBFILE* f = reinterpret_cast<DBFILE*>(buf);

	if ((f->todo > 0) && (f->todo < 10) && (!f->rf)) return -1;
	if ((f->todo > 20) && (!f->df)) return -1;

	switch (f->todo) {
	case 0:
		//open
		//TODO: track opened files
		f->rf = fopen(f->name,f->op);
		if (!f->rf) return -1;
		break;

	case 1:
		//close
		fclose(f->rf);
		break;

	case 2:
		//fread
		return (fread(f->buf,f->p_x,f->p_y,f->rf));

	case 3:
		//fwrite
		return (fwrite(f->buf,f->p_x,f->p_y,f->rf));

	case 4:
		//fseek
		if (f->p_y != 8) return -1;
		x = reinterpret_cast<uint64_t*>(f->buf);
		return (fseek(f->rf,*x,f->p_x));

	case 5:
		//ftell
		if (f->p_y != 8) return 0;
		x = reinterpret_cast<uint64_t*>(f->buf);
		*x = ftell(f->rf);
		break;

	case 6:
		//feof
		return (feof(f->rf));

	case 7:
		//ftruncate
		if (f->p_y != 8) return -1;
		sx = reinterpret_cast<int64_t*>(f->buf);
		return (ftruncate(fileno(f->rf),*sx));

	case 10:
		//isfileexist
		if ((stat(f->name,&tstat) == 0) && (tstat.st_mode & S_IFREG)) return 0;
		return -1;

	case 11:
		//isdirexist
		if ((stat(f->name,&tstat) == 0) && (tstat.st_mode & S_IFDIR)) return 0;
		return -1;

	case 12:
		//get file size (we can't handle >2GB filesize, but that's OK)
		if (!stat(f->name,&tstat)) return static_cast<int>(tstat.st_size);
		break;

	case 20:
		//open dir
		f->df = opendir(f->name);
		if (!f->df) return -1;
		break;

	case 21:
		//read dir (X param = is directory)
		pdir = readdir(f->df);
		if (!pdir) return -1;
		strncpy(reinterpret_cast<char*>(f->buf),pdir->d_name,f->p_y);
		f->p_x = (pdir->d_type == DT_DIR)? 1:0;
		break;

	case 22:
		//close dir
		closedir(f->df);
		break;

	default:
		return -1;
	}
	return 0;
}

} //namespace doscard
