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


#include "libserial.h"

#include "config.h"

#if defined (LINUX) || defined (MACOSX) || defined (BSD)

#include <string.h> // strlen
#include <stdlib.h>

#include <termios.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>

#include <errno.h>
#include <fcntl.h>
#include <stdio.h> // sprinf

namespace dosbox {

struct _COMPORT {
	int porthandle;
	bool breakstatus;
	termios backup;
};

bool SERIAL_open(const char* portname, COMPORT* port) {
	int result;
	// allocate COMPORT structure
	COMPORT cp = (_COMPORT*)malloc(sizeof(_COMPORT));
	if(cp == NULL) return false;

	cp->breakstatus=false;

	int len = strlen(portname);
	if(len > 240) {
		///////////////////////////////////SetLastError(ERROR_BUFFER_OVERFLOW);
		return false;
	}
	char extended_portname[256] = "/dev/";
	memcpy(extended_portname+5,portname,len);

	cp->porthandle = open (extended_portname, O_RDWR | O_NOCTTY | O_NONBLOCK);
	if (cp->porthandle < 0) goto cleanup_error;

	result = tcgetattr(cp->porthandle,&cp->backup);
	if (result==-1) goto cleanup_error;

	// get port settings
	termios termInfo;
	memcpy(&termInfo,&cp->backup,sizeof(termios));

	// initialize the port
	termInfo.c_cflag = CS8 | CREAD | CLOCAL; // noparity, 1 stopbit
	termInfo.c_iflag = PARMRK | INPCK;
	termInfo.c_oflag = 0;
	termInfo.c_lflag = 0;
	termInfo.c_cc[VMIN] = 0;
	termInfo.c_cc[VTIME] = 0;

	tcflush (cp->porthandle, TCIFLUSH);
	tcsetattr (cp->porthandle, TCSANOW, &termInfo);

	*port = cp;
	return true;

cleanup_error:
	if (cp->porthandle != 0) close(cp->porthandle);
	free(cp);
	return false;
}

void SERIAL_close(COMPORT port) {
	// restore original termios, close handle, free the COMPORT struct
	if (port->porthandle >= 0) {
		tcsetattr(port->porthandle, TCSANOW, &port->backup);
		close(port->porthandle);
	}
	free(port);
}

void SERIAL_getErrorString(char* buffer, int length) {
	int error = errno;
	if(length < 50) return;
	memset(buffer,0,length);
	// get the error message text from the operating system
	// TODO (or not)
	
	const char* err5text = "The specified port is already in use.\n";
	const char* err2text = "The specified port does not exist.\n";
	
	int sysmsg_offset = 0;

	if(error == EBUSY) {
		sysmsg_offset = strlen(err5text);
		memcpy(buffer,err5text,sysmsg_offset);

	} else if(error == 2) {
		sysmsg_offset = strlen(err2text);
		memcpy(buffer,err2text,sysmsg_offset);
	}
	
	sprintf(buffer + sysmsg_offset, "System error %d.",error);
	
}

int SERIAL_getmodemstatus(COMPORT port) {
	long flags = 0;
	ioctl (port->porthandle, TIOCMGET, &flags);
	int retval = 0;
	if (flags & TIOCM_CTS) retval |= SERIAL_CTS;
	if (flags & TIOCM_DSR) retval |= SERIAL_DSR;
	if (flags & TIOCM_RI) retval |= SERIAL_RI;
	if (flags & TIOCM_CD) retval |= SERIAL_CD;
	return retval;
}

bool SERIAL_sendchar(COMPORT port, char data) {
	if(port->breakstatus) return true; // true or false?!; Does POSIX need this check?
	int bytesWritten = write(port->porthandle, &data, 1);
	if(bytesWritten==1) return true;
	else return false;
}

int SERIAL_getextchar(COMPORT port) {
	unsigned char chRead = 0;
	int dwRead = 0;
	unsigned char error = 0;
	int retval = 0;

	dwRead=read(port->porthandle,&chRead,1);
	if (dwRead==1) {
		if(chRead==0xff) // error escape
		{
			dwRead=read(port->porthandle,&chRead,1);
			if(chRead==0x00) // an error 
			{
				dwRead=read(port->porthandle,&chRead,1);
				if(chRead==0x0) error=SERIAL_BREAK_ERR;
				else error=SERIAL_FRAMING_ERR;
			}
		}
		retval |= (error<<8);
		retval |= chRead;
		retval |= 0x10000; 
	}
	return retval;
}

bool SERIAL_setCommParameters(COMPORT port,
			int baudrate, char parity, int stopbits, int length) {
	
	termios termInfo;
	int result = tcgetattr(port->porthandle, &termInfo);
	if (result==-1) return false;
	termInfo.c_cflag = CREAD | CLOCAL;

	// parity
	// "works on many systems"
	#define CMSPAR 010000000000
	switch (parity) {
	case 'n': break;
	case 'o': termInfo.c_cflag |= (PARODD | PARENB); break;
	case 'e': termInfo.c_cflag |= PARENB; break;
	case 'm': termInfo.c_cflag |= (PARENB | CMSPAR | PARODD); break;
	case 's': termInfo.c_cflag |= (PARENB | CMSPAR); break;
	default:
		return false;
	}
	// stopbits
	switch(stopbits) {
	case SERIAL_1STOP: break;
	case SERIAL_2STOP: 
	case SERIAL_15STOP: termInfo.c_cflag |= CSTOPB; break;
	default:
		return false;
	}
	// byte length
	if(length > 8 || length < 5) return false;
	switch (length) {
	case 5: termInfo.c_cflag |= CS5; break;
	case 6: termInfo.c_cflag |= CS6; break;
	case 7: termInfo.c_cflag |= CS7; break;
	case 8: termInfo.c_cflag |= CS8; break;
	}
	// baudrate
	int posix_baudrate=0;
	switch(baudrate) {
		case 115200: posix_baudrate = B115200; break;
		case  57600: posix_baudrate = B57600; break;
		case  38400: posix_baudrate = B38400; break;
		case  19200: posix_baudrate = B19200; break;
		case   9600: posix_baudrate = B9600; break;
		case   4800: posix_baudrate = B4800; break;
		case   2400: posix_baudrate = B2400; break;
		case   1200: posix_baudrate = B1200; break;
		case    600: posix_baudrate = B600; break;
		case    300: posix_baudrate = B300; break;
		case    110: posix_baudrate = B110; break;
		default: return false;
	}
	cfsetospeed (&termInfo, posix_baudrate);
	cfsetispeed (&termInfo, posix_baudrate);

	int retval = tcsetattr(port->porthandle, TCSANOW, &termInfo);
	if(retval==-1) return false;
	return true;
}

void SERIAL_setBREAK(COMPORT port, bool value) {
	ioctl(port->porthandle, value?TIOCSBRK:TIOCCBRK);
}

void SERIAL_setDTR(COMPORT port, bool value) {
	long flag = TIOCM_DTR;
	ioctl(port->porthandle, value?TIOCMBIS:TIOCMBIC, &flag);
}

void SERIAL_setRTS(COMPORT port, bool value) {
	long flag = TIOCM_RTS;
	ioctl(port->porthandle, value?TIOCMBIS:TIOCMBIC, &flag);
}

} //namespace

#endif
