/*
 * This file is a part of the DOSCard project.
 *
 * (C) Copyright Dmitry 'MatrixS_Master' Soloviov, 2018-2019
 */

#include "VM86.h"
#include "VM86macro.h"
#include "main.h"

void VM86::LocalVDInput()
{
	io_ports[0x20] = 0; // PIC EOI
	io_ports[0x42] = --io_ports[0x40]; // PIT channel 0/2 read placeholder
	io_ports[0x3DA] ^= 9; // CGA refresh
	scratch_uint = extra ? regs16[REG_DX] : (uint8_t)i_data0;

	switch (scratch_uint) {
	case 0x60:
		io_ports[0x64] = 0; // Scancode read flag
		break;
	case 0x3D5:
		if (io_ports[0x3D4] >> 1 == 7) // CRT cursor position
			io_ports[0x3D5] = ((mem[0x49E]*80 + mem[0x49D] + CAST(int16_t,mem[0x4AD])) & (io_ports[0x3D4] & 1 ? 0xFF : 0xFF00)) >> (io_ports[0x3D4] & 1 ? 0 : 8);
		break;
	}
	R_M_OP(regs8[REG_AL], =, io_ports[scratch_uint]);
}

void VM86::LocalVDOutput()
{
	scratch_uint = extra ? regs16[REG_DX] : (uint8_t)i_data0;
	R_M_OP(io_ports[scratch_uint], =, regs8[REG_AL]);

	switch (scratch_uint) {
	case 0x61:
		io_hi_lo = 0;
		spkr_en |= regs8[REG_AL] & 3; // Speaker control
		break;
	case 0x43:
		//FIXME: add Speaker enable control
		//scratch_uint == 0x43 && (io_hi_lo = 0, regs8[REG_AL] >> 6 == 2) && (SDL_PauseAudio((regs8[REG_AL] & 0xF7) != 0xB6), 0); // Speaker enable
		break;
	case 0x40:
	case 0x42:
		if (io_ports[0x43] & 6)
			mem[0x469 + scratch_uint - (io_hi_lo ^= 1)] = regs8[REG_AL]; // PIT rate programming
		break;
	case 0x3D5:
		if (io_ports[0x3D4] >> 1 == 6)
			mem[0x4AD + !(io_ports[0x3D4] & 1)] = regs8[REG_AL]; // CRT video RAM start offset
		if (io_ports[0x3D4] >> 1 == 7)
			scratch2_uint = ((mem[0x49E]*80 + mem[0x49D] + CAST(int16_t,mem[0x4AD])) & (io_ports[0x3D4] & 1 ? 0xFF00 : 0xFF)) + (regs8[REG_AL] << (io_ports[0x3D4] & 1 ? 0 : 8)) - CAST(int16_t,mem[0x4AD]), mem[0x49D] = scratch2_uint % 80, mem[0x49E] = scratch2_uint / 80; // CRT cursor position
		break;
	case 0x3B5:
		if (io_ports[0x3B4] == 1)
			graphics_x = regs8[REG_AL] * 16; // Hercules resolution reprogramming. Defaults are set in the BIOS
		if (io_ports[0x3B4] == 6)
			graphics_y = regs8[REG_AL] * 4;
		framebuf_opened = false;
		break;
	}
}

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

		memset((void*)buf,0,TFT_TOTAL_BYTES);
		framebuf_opened = true;
	}

	// Start refreshing framebuffer from emulated graphics card video RAM
	vid_mem_base = mem + 0xB0000 + 0x8000*(mem[0x4AC] ? 1 : io_ports[0x3B8] >> 7); // B800:0 for CGA/Hercules bank 2, B000:0 for Hercules bank 1

	// Blit & scale everything, converting RGB332 to RGB565
	int dx = graphics_x / TFT_LCD_WIDTH;
	int dy = 2;//graphics_y / TFT_LCD_HEIGHT * 2; //FIXME: Debug only, since CGA _could_ use height-doubling
	int kx = graphics_x / 4;
	uint8_t r,g,b;

	for (int y = 0; y < graphics_y / dy; y++) {
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
