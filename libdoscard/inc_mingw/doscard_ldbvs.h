/*
 *  Copyright (C) 2013-2015  Dmitry Soloviov
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

#ifndef DOSCARD_LDBVS_H_
#define DOSCARD_LDBVS_H_


/* ****************** DosCard Generic Constants ****************** */
/// API Version.
#define LDBWINTVER 4

/// Letter index of last wrapper function.
#define LDBWRAP_LAST_FUNC_L 'R'

/// Number of wrapper functions accessible.
#define LDBWRAP_FUNCS_Q ((int)(LDBWRAP_LAST_FUNC_L-'A'+1))

/// Frameskip absolute maximum.
#define LDBW_FRAMESKIP_MAX 10

/// Input string buffer size
#define LDBW_STRINGBUF_SIZE 2048

/// Buffer holds at most this number of samples.
#define LDBW_SNDBUF_SAMPLES 4096

/* ****************** Capabilities Data Constants ****************** */
/// Video output.
#define DOSCRD_CAP_VIDEO 0x001

/// Audio Output.
#define DOSCRD_CAP_AUDIO 0x002

/// Events input.
#define DOSCRD_CAP_EVENT 0x004

/// Internal messages output.
#define DOSCRD_CAP_MESSG 0x008

/// File I/O and control.
/** Wrapper-embedded functionality. */
#define DOSCRD_CAP_FILIO 0x010

/// Directory I/O and control.
/** Wrapper-embedded functionality. */
#define DOSCRD_CAP_DIRIO 0x020

/// Serial (RS232) I/O and control.
/** Maps through extended IO. */
#define DOSCRD_CAP_SERIO 0x040

/// Parallel (LPT) I/O and control.
/** Maps through extended IO. */
#define DOSCRD_CAP_PARIO 0x080

/// STDOUT echo output.
/** Maps through message system IO. */
#define DOSCRD_CAP_EHOUT 0x100
/// STDOUT echo output prefix (marker).
#define DOSCRD_EHOUT_MARKER "{!}"

/// STDIN input.
#define DOSCRD_CAP_TTYIN 0x200
/// STDIN blocking input.
#define DOSCRD_TTYIN_BLK 0x400

/// Interleaved mode auto-restart
#define DOSCRD_ILV_AUTOR 0x800

/// Basic functionality macro.
#define DOSCRD_CAPS_BASIC	 (	DOSCRD_CAP_MESSG | \
								DOSCRD_CAP_FILIO | \
								DOSCRD_CAP_DIRIO )

/// Standard functionality macro.
#define DOSCRD_CAPS_STANDARD (	DOSCRD_CAPS_BASIC | \
								DOSCRD_CAP_VIDEO  | \
								DOSCRD_CAP_AUDIO  | \
								DOSCRD_CAP_EVENT  )

/// Macro for headless mode.
#define DOSCRD_CAPS_HEADLESS (	DOSCRD_CAPS_BASIC | \
								DOSCRD_CAP_TTYIN  | \
								DOSCRD_TTYIN_BLK  | \
								DOSCRD_CAP_EHOUT  )

/// Everything is ON.
#define DOSCRD_CAPS_EVERYTHN 0x0fff

#endif /* DOSCARD_LDBVS_H_ */
