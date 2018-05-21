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

#ifndef LDCDLL_H_
#define LDCDLL_H_


#define LDC_DLL __declspec(dllexport)

namespace doscard {

class LDC_DLL CDosCard;
void LDC_DLL LibDosCardInit(int verb);
void LDC_DLL LibDosCardExit(void);

}

#endif /* LDCDLL_H_ */
