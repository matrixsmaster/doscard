/*
 *  Copyright (C) 2014-2015  Dmitry Soloviov
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

int32_t LDBCB_LCD(void* buf, size_t len)
{
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
				MUTEX_UNLOCK;
			}
			break;

		default:
			if (Runtime->disp_fsm == 1) {
				if (mutex > 0) {
					if (Runtime->frameskip_cnt++ >= LDBW_FRAMESKIP_MAX) {
						MUTEX_LOCK;
					} else
						return DISPLAY_RET_BUSY;
				}
				Runtime->frameskip_cnt = 0;

				uint16_t old_w = Runtime->lcdw;
				uint16_t old_h = Runtime->lcdh;
				Runtime->lcdw = (*dw >> 16);
				Runtime->lcdh = *dw & 0xffff;
				Runtime->frame_cnt = 0;
				Runtime->disp_fsm = 2;
				if ((!Runtime->framebuf) || (old_w*old_h != Runtime->lcdw*Runtime->lcdh)) {
					Runtime->framebuf = reinterpret_cast<uint32_t*>
					(realloc(Runtime->framebuf,sizeof(uint32_t)*Runtime->lcdw*Runtime->lcdh));
					Runtime->frame_dirty = true;
				}
//				printf("frm sz = %d x %d\n",Runtime->lcdw,Runtime->lcdh);
//				Runtime->crc = 0;
			}
			break;
		}
	} else if ((Runtime->disp_fsm == 2) && (len == Runtime->lcdw * 4)) {
		memcpy(Runtime->framebuf+(Runtime->lcdw*Runtime->frame_cnt),buf,len);
//		for (uint64_t i=0; i<len/4; i++)
//			Runtime->crc += dw[i];
		if (++Runtime->frame_cnt >= Runtime->lcdh) {
			Runtime->disp_fsm = 1;
//			printf("frm crc = %u\n",Runtime->crc);
			MUTEX_UNLOCK;
		}
	} else {
		return -1;
	}
	return 0;
}

int32_t LDBCB_SND(void* buf, size_t len)
{
#if 0
	if ((!buf) || (!len)) return -1;
	uint8_t* ptr;
	unsigned int sz = LDBW_SNDBUF_SAMPLES * sizeof(LDBI_SndSample);
	int64_t rem = 0;
	MUTEX_LOCK;
	if (len == sizeof(LDB_SoundInfo)) {
		memcpy(&Runtime->sound_req,buf,len);
		Runtime->sound_fmt_ok = true;
	} else {
		if (!Sound) {
			Sound = reinterpret_cast<LDBI_SndSample*> (malloc(sz));
			memset(Sound,0,sz); //silence
		}
		if (len > sz) len = sz;
		rem = (len + Runtime->sound_rec) - sz;
		if (rem < 0) rem = 0;
		else if (rem > 0) len = sz - Runtime->sound_rec;
//		Runtime->sound_avail = len;
//		Runtime->sound_pos = 0;
		if (Sound) {
			ptr = reinterpret_cast<uint8_t*> (Sound);
//			memcpy(ptr+Runtime->sound_rec,buf,len);
			if (rem) {
				ptr = reinterpret_cast<uint8_t*> (buf);
//				memcpy(Sound,ptr+len,rem);
			}
		}
		Runtime->sound_rec += len;
		if (Runtime->sound_rec > sz)
			Runtime->sound_rec -= sz;
	}
	MUTEX_UNLOCK;
#endif
	return 0;
}

int32_t LDBCB_UIE(void* buf, size_t len)
{
	int32_t r = 0;
	LDB_UIEvent e;
	if ((!buf) || (len != sizeof(LDB_UIEvent))) return -1;
	if (mutex) return 0;
//	MUTEX_LOCK;
	if (Events->empty()) return 0;
	r = Events->size();
	e = Events->back();
	Events->pop_back();
	MUTEX_UNLOCK;			//FIXME: WTF?!
	memcpy(buf,&e,len);
	return r;
}

int32_t LDBCB_TCK(void* buf, size_t len)
{
	if ((!buf) || (len < 4)) return -1;
	uint32_t* val = reinterpret_cast<uint32_t*>(buf);
#if USE_WEAK_CLOCKING
	if (!Runtime->clkbeg) {
		Runtime->clkbeg = new uint32_t;
		*Runtime->clkbeg = static_cast<uint32_t> (clock());
	} else {
		*val = static_cast<uint32_t> (clock());
		*val -= *Runtime->clkbeg;
	}
#else
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
#endif
	return 0;
}

int32_t LDBCB_MSG(void* buf, size_t len)
{
	if ((!buf) || (!len)) return -1;
	char* str = reinterpret_cast<char*> (buf);
	MUTEX_LOCK;
	std::string nstr(str);
	Messages->push_back(nstr);
	MUTEX_UNLOCK;
	return 0;
}

int32_t LDBCB_FIO(void* buf, size_t len)
{
	uint64_t* x;
	int64_t* sx;
	struct stat tstat;
	struct dirent* pdir;

	if ((!buf) || (len < sizeof(DBFILE))) return -1;
	DBFILE* f = reinterpret_cast<DBFILE*>(buf);

	if ((f->todo > 0) && (f->todo < 10) && (!f->rf)) return -1;
	if ((f->todo > 20) && (!f->df)) return -1;
	if ((f->todo >= 20) && (!(Caps & DOSCRD_CAP_DIRIO))) return -1;

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

int32_t LDBCB_CIO(void* buf, size_t len)
{
	//TODO COM port IO
	return 0;
}

int32_t LDBCB_LIO(void* buf, size_t len)
{
	//TODO LPT port IO
	return 0;
}

int32_t LDBCB_STO(void* buf, size_t len)
{
	if ((!buf) || (!len)) return -1;
	char* ins = reinterpret_cast<char*> (buf);
	uint32_t sl = len + strlen(DOSCRD_EHOUT_MARKER) + 1;
	char* str = reinterpret_cast<char*> (malloc(sl));
	if (!str) return -1;
	strcpy(str,DOSCRD_EHOUT_MARKER);
	strncat(str,ins,len);
	str[sl-1] = 0;
	LDBCB_MSG(str,sl);
	free(str);
	return 0;
}

int32_t LDBCB_STI(void* buf, size_t len)
{
	if ((!buf) || (!len)) return -1;
	char* out = reinterpret_cast<char*> (buf);
	uint32_t i,l,j = 1;
	if (Caps & DOSCRD_TTYIN_BLK) {
		MUTEX_WAIT_FOR_EVENT((strlen(StringInput)));
	} else {
		MUTEX_LOCK;
	}
	l = strlen(StringInput);
	for (i=0; ((i<len) && j && l); i++) {
		out[i] = StringInput[i];
		switch (out[i]) {
		case 0:
		case 0x0D:
		case 0x0A:
			out[i] = 0x0D;
			if (i+2 < len) out[++i] = 0x0A;
			j = 0;
			break;
		default:
			break;
		}
	}
	if (l) {
		memmove(StringInput,StringInput+i,l-i);
		StringInput[l-i] = 0;
	}
	MUTEX_UNLOCK;
	return static_cast<int32_t> (i);
}

int32_t LDBCB_NOP(void* buf, size_t len)
{
	if (len) usleep(len*1000);
	return 0;
}

} //namespace doscard
