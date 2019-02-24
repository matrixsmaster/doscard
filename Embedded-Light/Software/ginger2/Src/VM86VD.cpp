/*
 * This file is a part of the DOSCard project.
 *
 * (C) Copyright Dmitry 'MatrixS_Master' Soloviov, 2018-2019
 */

#include "VM86.h"
#include "VM86macro.h"
#include "main.h"

void VM86::LocalVideoMode()
{
	// Update the video graphics display every GRAPHICS_UPDATE_DELAY instructions
	if (inst_counter % GRAPHICS_UPDATE_DELAY) return;

	// Prepare the main graphics output pointer
	__IO uint16_t* buf = (__IO uint16_t*)&(g_frames[2*TFT_TOTAL_PIXELS]); //2 is index of video framebuffer for CGA

	// If we don't already have a framebuffer, create a new one
	if (!framebuf_opened)
	{
		for (int i = 0; i < 16; i++)
			pixel_colors[i] = mem[0x4AC] ? // CGA?
					cga_colors[(i & 12) >> 2] + (cga_colors[i & 3] << 16) // CGA -> RGB332
					: 0xFF*(((i & 1) << 24) + ((i & 2) << 15) + ((i & 4) << 6) + ((i & 8) >> 3)); // Hercules -> RGB332

		for (unsigned i = 0; i < graphics_x * graphics_y / 4; i++)
			vid_addr_lookup[i] = i / graphics_x * (graphics_x / 8) + (i / 2) % (graphics_x / 8) + 0x2000*(mem[0x4AC] ? (2 * i / graphics_x) % 2 : (4 * i / graphics_x) % 4);

//		memset((void*)buf,0,TFT_TOTAL_BYTES);
//		framebuf_opened = true;
	}

	// Start refreshing framebuffer from emulated graphics card video RAM
	vid_mem_base = mem + 0xB0000 + 0x8000*(mem[0x4AC] ? 1 : io_ports[0x3B8] >> 7); // B800:0 for CGA/Hercules bank 2, B000:0 for Hercules bank 1

	// Blit & scale everything, converting RGB332 to RGB565
	int dx = graphics_x / TFT_LCD_WIDTH;
	int dy = 2;//graphics_y / TFT_LCD_HEIGHT * 2; //FIXME: Debug only, since CGA _could_ use height-doubling
	int kx = graphics_x / 4;
	uint8_t r,g,b;

	for (int y = 0; y < TFT_LCD_HEIGHT; y++) {
		int idx = y * dy * kx;
		buf += TFT_LCD_WIDTH; //the TFT screen has a horizontal scanline reversed
		__IO uint16_t* cbuf = buf;

		for (int x = 0, cx = 0; x < kx; x++,idx++) {
			unsigned tmp = pixel_colors[15 & (vid_mem_base[vid_addr_lookup[idx]] >> 4*!(idx & 1))];

			for (int i = 0; i < 4; i++, tmp >>= 8, cx++) {
				if (cx % dx == 0) {
					r = ((tmp & 0xE0) << 0) >> 3;
					g = ((tmp & 0x1C) << 3) >> 2;
					b = ((tmp & 0x03) << 6) >> 3;
					*cbuf = (((uint16_t)r) << 11) | (((uint16_t)g) << 5) | b;
					--cbuf; //the TFT screen has a horizontal scanline reversed
				}
			}
		}
	}
}
