/*
 *  Copyright (C) 2015  Dmitry Soloviov
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

#include <math.h>
#include "frameproc.h"

namespace doscard {

static inline int max(const int a, const int b)
{
	if (a > b) return a;
	else return b;
}

static inline void bound(float* f, float min, float max)
{
	if (*f < min) *f = min;
	if (*f > max) *f = max;
}

bool convert_frame(uint32_t* src, uint32_t* dst, const int ow, const int oh, const LDBI_PostProcess* set)
{
	int x,y,z,q;
	uint32_t v;
	uint16_t* wbuf;
	uint8_t* bbuf,br,bg,bb,byt;
	float b,g,r,kx,ky,ex,ey;
	if ((!src) || (!dst) || (ow < 1) || (oh < 1) || (!set)) return false;


	uint32_t* temp = (uint32_t*)malloc(max(ow,set->w)*max(oh,set->h)*4);
	if (!temp) return false;

	uint32_t* line = (uint32_t*)malloc(max(max(ow,set->w),max(oh,set->h))*4);
	if (!line) {
		free(temp);
		return false;
	}

	//apply gamma
	for (x = 0; x < ow * oh; x++) {
		v = src[x];
		b = set->gamma[2] * (float)(v&0xff);
		bound(&b,0,255);
		v >>= 8;
		g = set->gamma[1] * (float)(v&0xff);
		bound(&g,0,255);
		v >>= 8;
		r = set->gamma[0] * (float)(v&0xff);
		bound(&r,0,255);
		v = (uint32_t)(floor(r));
		v <<= 8;
		v |= (uint32_t)(floor(g));
		v <<= 8;
		v |= (uint32_t)(floor(b));
		src[x] = v | ALPHAVALUE;
	}

	//resize
	kx = (float)set->w / (float)ow;
	ky = (float)set->h / (float)oh;
	ey = 0;
	q = 0;
	for (y = 0; y < oh; y++) /* Scale up and Scale down by X */ {
		ex = 0;
		z = 0;
		if (kx == 1.0) {
			memcpy(line,src+(ow*y),ow*4);
		} else {
			r = g = b = 0;
			for (x = 0; x < ow; x++) {
				v = src[ow*y+x];
				ex += kx;
				if (kx > 1) {
					while (ex >= 1.0) {
						line[z++] = v;
						ex -= 1.0;
					}
				} else if (kx < 1) {
					b += v & 0xff;
					g += (v >> 8) & 0xff;
					r += (v >> 16) & 0xff;
					if (ex >= 1.0) {
						v = (uint32_t)(floor(r / floor(1/kx)));
						v <<= 8;
						v |= (uint32_t)(floor(g / floor(1/kx)));
						v <<= 8;
						v |= (uint32_t)(floor(b / floor(1/kx)));
						line[z++] = v | ALPHAVALUE;
						r = g = b = 0;
						ex -= 1.0;
					}
				}
			}
		}
		if (ky > 1) {
			ey += ky;
			while (ey >= 1.0) {
				memcpy(temp+(set->w*q),line,set->w*4);
				q++;
				ey -= 1.0;
			}
		} else
			memcpy(temp+(set->w*y),line,set->w*4);
	}
	if (ky < 1) /* Scale down by Y */ {
		ey = 0;
		q = 0;
		for (y = 1; y < oh; y++) {
			for (x = 0; x < set->w; x++) {
				v = temp[set->w*(y-1)+x];
				b = v & 0xff;
				g = (v >> 8) & 0xff;
				r = (v >> 16) & 0xff;
				v = temp[set->w*y+x];
				b += v & 0xff;
				g += (v >> 8) & 0xff;
				r += (v >> 16) & 0xff;
				v = (uint32_t)(floor(r / 2));
				v <<= 8;
				v |= (uint32_t)(floor(g / 2));
				v <<= 8;
				v |= (uint32_t)(floor(b / 2));
				line[x] = v | ALPHAVALUE;
			}
			ey += ky;
			if (ey >= 1.0) {
				memcpy(temp+(set->w*q),line,set->w*4);
				q++;
				ey -= 1.0;
			}
		}
	}

	//apply grayscale and/or b/w
	q = set->w * set->h;
	for (z = 0; (z < q) && (set->typ != DOSCRD_VID_COLOR); z++) {
		v = temp[z];
		r = v & 0xff;
		r += (v >> 8) & 0xff;
		r += (v >> 16) & 0xff;
		r /= 3;
		bound(&r,0,255);
		if (set->typ == DOSCRD_VID_BW)
			r = (r >= set->threshold)? 255:0;
		v = (uint32_t)(floor(r));
		v <<= 8;
		v |= (uint32_t)(floor(r));
		v <<= 8;
		v |= (uint32_t)(floor(r));
		temp[z] = v | ALPHAVALUE;
	}

	//format conversion
	wbuf = reinterpret_cast<uint16_t*> (temp);
	bbuf = reinterpret_cast<uint8_t*> (temp);
	if (set->fmt != DOSCRD_VIDFMT_DWORD) {
		byt = 0;
		for (z=0,y=0,x=0; z < q; z++) {
			v = temp[z];
			bb = v & 0xff;
			bg = (v >> 8) & 0xff;
			br = (v >> 16) & 0xff;
			switch (set->fmt) {
			case DOSCRD_VIDFMT_WORD:
				//rgb555
				wbuf[y++] = ((br>>3) << 10) | ((bg>>3) << 5) | (bb>>3);
				break;
			case DOSCRD_VIDFMT_BYTE:
				//rgb332
				bbuf[y++] = ((br>>5) << 5) | ((bg>>5) << 2) | (bb>>6);
				break;
			case DOSCRD_VIDFMT_BIT:
				//bw 1
				byt |= ((br & 0x80) || (bg & 0x80) || (bb & 0x80))? 1:0;
				byt <<= 1;
				if (++x > 7) {
					bbuf[y++] = byt;
					byt = 0;
					x = 0;
				}
				break;
			default: break; //just to make compiler happy :)
			}
		}
	}
	switch (set->fmt) {
	case DOSCRD_VIDFMT_DWORD: q *= 4; break;
	case DOSCRD_VIDFMT_WORD: q *= 2; break;
	case DOSCRD_VIDFMT_BIT: q /= 8; break;
	default: break; //just to make compiler happy :)
	}

	//copy to dest
	memcpy(dst,temp,q);

	free(line);
	free(temp);

	return true;
}

uint32_t get_buffer_size(const LDBI_PostProcess* set)
{
	uint32_t r = 0;
	if (!set) return 0;
	switch(set->fmt) {
	case DOSCRD_VIDFMT_DWORD: r = 4; break;
	case DOSCRD_VIDFMT_WORD: r = 2; break;
	case DOSCRD_VIDFMT_BYTE: r = 1; break;
	default: r = 0; break;
	}
	if (r) r *= set->w * set->h;
	else r = set->w * set->h / 8;
	if (r % 4) {
		r /= 4;
		r = (r+1) * 4;
	}
	return r;
}

}
