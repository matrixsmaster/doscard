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

static int max(const int a, const int b)
{
	if (a > b) return a;
	else return b;
}

bool convert_frame(uint32_t* src, uint32_t* dst, const int ow, const int oh, const LDBI_PostProcess* set)
{
	int x,y,z,q,i;
	uint32_t v;
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
		b = (b > 255)? 255:b;
		b = (b < 0)? 0:b;
		v >>= 8;
		g = set->gamma[1] * (float)(v&0xff);
		g = (g > 255)? 255:g;
		g = (g < 0)? 0:g;
		v >>= 8;
		r = set->gamma[0] * (float)(v&0xff);
		r = (r > 255)? 255:r;
		r = (r < 0)? 0:r;
		v = (uint32_t)(floor(r));
		v <<= 8;
		v |= (uint32_t)(floor(g));
		v <<= 8;
		v |= (uint32_t)(floor(b));
		temp[x] = v | 0xff000000;
	}

	//resize
	kx = (float)set->w / (float)ow;
	ky = (float)set->h / (float)oh;
	ey = 0;
	q = 0;
	for (y = 0; y < oh; y++) {
		ex = 0;
		z = 0;
		if (kx == 1.0) {
			memcpy(line,temp+(ow*y),ow*4);
		} else {
			r = g = b = 0;
			for (x = 0; x < ow; x++) {
				v = temp[ow*y+x];
				if (kx > 1) {
					//for (i = 0; i < floor(kx); i++) line[z++] = v;
					//ex += kx - floor(kx);
					ex += kx;
					while (ex >= 1.0) {
						line[z++] = v;
						ex -= 1.0;
					}
				} else if (kx < 1) {
					b += v & 0xff;
					g += (v >> 8) & 0xff;
					r += (v >> 16) & 0xff;
					ex += kx;
					if (ex >= 1.0) {
						v = (uint32_t)(floor(r / floor(1/kx)));
						v <<= 8;
						v |= (uint32_t)(floor(g / floor(1/kx)));
						v <<= 8;
						v |= (uint32_t)(floor(b / floor(1/kx)));
						v |= 0xff000000;
						r = g = b = 0;
						line[z++] = v;
						ex -= 1.0;
					}
				}
			}
		}
		if (ky > 1) {
			//for (i = 0; i < floor(ky); i++) {
				/*memcpy(dst+(set->w*q),line,set->w*4);
				q++;
				ey += ky - floor(ky);*/
				ey += ky;
				while (ey >= 1.0) {
					memcpy(dst+(set->w*q),line,set->w*4);
					q++;
					ey -= 1.0;
				}
			//}
		} else if (ky < 1)
			memcpy(temp+(set->w*y),line,set->w*4);
		else
			memcpy(dst+(set->w*y),line,set->w*4);
	}
	//printf("k = %.0f\tz = %d\n",k,z);
	if (ky < 1) {
		ey = 0;
		q = 0;
//		memcpy(line,temp,set->w*4);
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
				v |= 0xff000000;
				line[x] = v;
			}
			ey += ky;
			if (ey >= 1.0) {
				memcpy(dst+(set->w*q),line,set->w*4);
				q++;
				ey -= 1.0;
			}
		}
	}

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
