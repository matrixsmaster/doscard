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

#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <string>
#include <vector>
#include "dosbox.h"
#include "programs.h"
#include "support.h"
#include "drives.h"
#include "cross.h"
#include "regs.h"
#include "callback.h"
#include "cdrom.h"
#include "dos_system.h"
#include "dos_inc.h"
#include "bios.h"
#include "bios_disk.h"
#include "ldb.h"

namespace dosbox {

void MSCDEX_SetCDInterface(int intNr, int forceCD);
static Bitu ZDRIVE_NUM = 25;

class MOUNT : public Program {
public:
	void ListMounts(void) {
		char name[DOS_NAMELENGTH_ASCII];Bit32u size;Bit16u date;Bit16u time;Bit8u attr;
		/* Command uses dta so set it to our internal dta */
		RealPt save_dta = dos.dta();
		dos.dta(dos.tables.tempdta);
		DOS_DTA dta(dos.dta());

		WriteOut(MSG_Get("PROGRAM_MOUNT_STATUS_1"));
		WriteOut(MSG_Get("PROGRAM_MOUNT_STATUS_FORMAT"),"Drive","Type","Label");
		for(int p = 0;p < 8;p++) WriteOut("----------");

		for (int d = 0;d < DOS_DRIVES;d++) {
			if (!Drives[d]) continue;

			char root[4] = {'A'+d,':','\\',0};
			bool ret = DOS_FindFirst(root,DOS_ATTR_VOLUME);
			if (ret) {
				dta.GetResult(name,size,date,time,attr);
				DOS_FindNext(); //Mark entry as invalid
			} else name[0] = 0;

			/* Change 8.3 to 11.0 */
			char* dot = strchr(name,'.');
			if(dot && (dot - name == 8) ) { 
				name[8] = name[9];name[9] = name[10];name[10] = name[11];name[11] = 0;
			}

			root[1] = 0; //This way, the format string can be reused.
			WriteOut(MSG_Get("PROGRAM_MOUNT_STATUS_FORMAT"),root, Drives[d]->GetInfo(),name);		
		}
		dos.dta(save_dta);
	}

	void Run(void) {
		DOS_Drive * newdrive;char drive;
		std::string label;
		std::string umount;
		std::string newz;

		//Hack To allow long commandlines
		ChangeToLongCmd();
		/* Parse the command line */
		/* if the command line is empty show current mounts */
		if (!cmd->GetCount()) {
			ListMounts();
			return;
		}

		/* In secure mode don't allow people to change mount points. 
		 * Neither mount nor unmount */
		if(myldbi->GetConfig()->secure) {
			WriteOut(MSG_Get("PROGRAM_CONFIG_SECURE_DISALLOW"));
			return;
		}

		/* Check for unmounting */
		if (cmd->FindString("-u",umount,false)) {
			umount[0] = toupper(umount[0]);
			int i_drive = umount[0]-'A';
			if (i_drive < DOS_DRIVES && i_drive >= 0 && Drives[i_drive]) {
				switch (DriveManager::UnmountDrive(i_drive)) {
				case 0:
					Drives[i_drive] = 0;
					if(i_drive == DOS_GetDefaultDrive())
						DOS_SetDrive(ZDRIVE_NUM);
					WriteOut(MSG_Get("PROGRAM_MOUNT_UMOUNT_SUCCESS"),umount[0]);
					break;
				case 1:
					WriteOut(MSG_Get("PROGRAM_MOUNT_UMOUNT_NO_VIRTUAL"));
					break;
				case 2:
					WriteOut(MSG_Get("MSCDEX_ERROR_MULTIPLE_CDROMS"));
					break;
				}
			} else {
				WriteOut(MSG_Get("PROGRAM_MOUNT_UMOUNT_NOT_MOUNTED"),umount[0]);
			}
			return;
		}

		/* Check for moving Z: */
		/* Only allowing moving it once. It is merely a convenience added for the wine team */
		if (ZDRIVE_NUM == 25 && cmd->FindString("-z", newz,false)) {
			newz[0] = toupper(newz[0]);
			int i_newz = newz[0] - 'A';
			if (i_newz >= 0 && i_newz < DOS_DRIVES-1 && !Drives[i_newz]) {
				ZDRIVE_NUM = i_newz;
				/* remap drives */
				Drives[i_newz] = Drives[25];
				Drives[25] = 0;
				DOS_Shell *fs = static_cast<DOS_Shell *>(first_shell); //dynamic ?				
				/* Update environment */
				std::string line = "";
				char ppp[2] = {newz[0],0};
				std::string tempenv = ppp; tempenv += ":\\";
				if (fs->GetEnvStr("PATH",line)){
					std::string::size_type idx = line.find('=');
					std::string value = line.substr(idx +1 , std::string::npos);
					while ( (idx = value.find("Z:\\")) != std::string::npos ||
							(idx = value.find("z:\\")) != std::string::npos  )
						value.replace(idx,3,tempenv);
					line = value;
				}
				if (!line.size()) line = tempenv;
				fs->SetEnv("PATH",line.c_str());
				tempenv += "COMMAND.COM";
				fs->SetEnv("COMSPEC",tempenv.c_str());

				/* Update batch file if running from Z: (very likely: autoexec) */
				if(fs->bf) {
					std::string &name = fs->bf->filename;
					if(name.length() >2 &&  name[0] == 'Z' && name[1] == ':') name[0] = newz[0];
				}
				/* Change the active drive */
				if (DOS_GetDefaultDrive() == 25) DOS_SetDrive(i_newz);
			}
			return;
		}
		/* Show list of cdroms */
		if (cmd->FindExist("-cd",false)) {
			WriteOut("SDL and IOCTL drives not found. Are you joking you want to mount real CD on DosCard?!\n");
			//			int num = SDL_CDNumDrives();
			//   		WriteOut(MSG_Get("PROGRAM_MOUNT_CDROMS_FOUND"),num);
			//			for (int i=0; i<num; i++) {
			//				WriteOut("%2d. %s\n",i,SDL_CDName(i));
			//			};
			return;
		}

		std::string type="dir";
		cmd->FindString("-t",type,true);
		bool iscdrom = (type =="cdrom"); //Used for mscdex bug cdrom label name emulation
		if (type=="floppy" || type=="dir" || type=="cdrom") {
			Bit16u sizes[4];
			Bit8u mediaid;
			std::string str_size;
			if (type=="floppy") {
				str_size="512,1,2880,2880";/* All space free */
				mediaid=0xF0;		/* Floppy 1.44 media */
			} else if (type=="dir") {
				// 512*32*32765==~500MB total size
				// 512*32*16000==~250MB total free size
				str_size="512,32,32765,16000";
				mediaid=0xF8;		/* Hard Disk */
			} else if (type=="cdrom") {
				str_size="2048,1,65535,0";
				mediaid=0xF8;		/* Hard Disk */
			} else {
				WriteOut(MSG_Get("PROGAM_MOUNT_ILL_TYPE"),type.c_str());
				return;
			}
			/* Parse the free space in mb's (kb's for floppies) */
			std::string mb_size;
			if(cmd->FindString("-freesize",mb_size,true)) {
				char teststr[1024];
				Bit16u freesize = static_cast<Bit16u>(atoi(mb_size.c_str()));
				if (type=="floppy") {
					// freesize in kb
					sprintf(teststr,"512,1,2880,%d",freesize*1024/(512*1));
				} else {
					Bit32u total_size_cyl=32765;
					Bit32u free_size_cyl=(Bit32u)freesize*1024*1024/(512*32);
					if (free_size_cyl>65534) free_size_cyl=65534;
					if (total_size_cyl<free_size_cyl) total_size_cyl=free_size_cyl+10;
					if (total_size_cyl>65534) total_size_cyl=65534;
					sprintf(teststr,"512,32,%d,%d",total_size_cyl,free_size_cyl);
				}
				str_size=teststr;
			}

			cmd->FindString("-size",str_size,true);
			char number[20];const char * scan=str_size.c_str();
			Bitu index=0;Bitu count=0;
			/* Parse the str_size string */
			while (*scan) {
				if (*scan==',') {
					number[index]=0;sizes[count++]=atoi(number);
					index=0;
				} else number[index++]=*scan;
				scan++;
			}
			number[index]=0;sizes[count++]=atoi(number);

			// get the drive letter
			cmd->FindCommand(1,temp_line);
			if ((temp_line.size() > 2) || ((temp_line.size()>1) && (temp_line[1]!=':'))) goto showusage;
			drive=toupper(temp_line[0]);
			if (!isalpha(drive)) goto showusage;

			if (!cmd->FindCommand(2,temp_line)) goto showusage;
			if (!temp_line.size()) goto showusage;
//			struct stat test;
			bool failed = false;
//			if (stat(temp_line.c_str(),&test)) {
			if (!dbisitexist(temp_line.c_str())) {
				failed = true;
//				Cross::ResolveHomedir(temp_line);
//				//Try again after resolving ~
//				if(!stat(temp_line.c_str(),&test)) failed = false;
			}
			if (failed) {
				WriteOut(MSG_Get("PROGRAM_MOUNT_ERROR_1"),temp_line.c_str());
				return;
			}
			/* Not a switch so a normal directory/file */
//			if (!(test.st_mode & S_IFDIR)) {
			if (!dbisdirex(temp_line.c_str())) {
				WriteOut(MSG_Get("PROGRAM_MOUNT_ERROR_2"),temp_line.c_str());
				return;
			}

			if (temp_line[temp_line.size()-1]!=CROSS_FILESPLIT) temp_line+=CROSS_FILESPLIT;
			Bit8u bit8size=(Bit8u) sizes[1];
			if (type=="cdrom") {
				int error = 0;
				newdrive  = new cdromDrive(drive,temp_line.c_str(),sizes[0],bit8size,sizes[2],0,mediaid,error);
				// Check Mscdex, if it worked out...
				switch (error) {
				case 0  :	WriteOut(MSG_Get("MSCDEX_SUCCESS"));				break;
				case 1  :	WriteOut(MSG_Get("MSCDEX_ERROR_MULTIPLE_CDROMS"));	break;
				case 2  :	WriteOut(MSG_Get("MSCDEX_ERROR_NOT_SUPPORTED"));	break;
				case 3  :	WriteOut(MSG_Get("MSCDEX_ERROR_PATH"));				break;
				case 4  :	WriteOut(MSG_Get("MSCDEX_TOO_MANY_DRIVES"));		break;
				case 5  :	WriteOut(MSG_Get("MSCDEX_LIMITED_SUPPORT"));		break;
				default :	WriteOut(MSG_Get("MSCDEX_UNKNOWN_ERROR"));			break;
				};
				if (error && error!=5) {
					delete newdrive;
					return;
				}
			} else {
				/* Give a warning when mount c:\ or the / */
				if(temp_line == "/") WriteOut(MSG_Get("PROGRAM_MOUNT_WARNING_OTHER"));
				newdrive=new localDrive(temp_line.c_str(),sizes[0],bit8size,sizes[2],sizes[3],mediaid);
			}
		} else {
			WriteOut(MSG_Get("PROGRAM_MOUNT_ILL_TYPE"),type.c_str());
			return;
		}
		if (Drives[drive-'A']) {
			WriteOut(MSG_Get("PROGRAM_MOUNT_ALREADY_MOUNTED"),drive,Drives[drive-'A']->GetInfo());
			if (newdrive) delete newdrive;
			return;
		}
		if (!newdrive) E_Exit("DOS:Can't create drive");
		Drives[drive-'A']=newdrive;
		/* Set the correct media byte in the table */
		mem_writeb(Real2Phys(dos.tables.mediaid)+(drive-'A')*2,newdrive->GetMediaByte());
		WriteOut(MSG_Get("PROGRAM_MOUNT_STATUS_2"),drive,newdrive->GetInfo());
		/* check if volume label is given and don't allow it to updated in the future */
		if (cmd->FindString("-label",label,true)) newdrive->dirCache.SetLabel(label.c_str(),iscdrom,false);
		/* For hard drives set the label to DRIVELETTER_Drive.
		 * For floppy drives set the label to DRIVELETTER_Floppy.
		 * This way every drive except cdroms should get a label.*/
		else if(type == "dir") { 
			label = drive; label += "_DRIVE";
			newdrive->dirCache.SetLabel(label.c_str(),iscdrom,true);
		} else if(type == "floppy") {
			label = drive; label += "_FLOPPY";
			newdrive->dirCache.SetLabel(label.c_str(),iscdrom,true);
		}
		if(type == "floppy") incrementFDD();
		return;
		showusage:
#if defined (WIN32) || defined(OS2)
		WriteOut(MSG_Get("PROGRAM_MOUNT_USAGE"),"d:\\dosprogs","d:\\dosprogs");
#else
		WriteOut(MSG_Get("PROGRAM_MOUNT_USAGE"),"~/dosprogs","~/dosprogs");
#endif
		return;
	}
};

static void MOUNT_ProgramStart(Program * * make) {
	*make=new MOUNT;
}

class MEM : public Program {
public:
	void Run(void) {
		/* Show conventional Memory */
		WriteOut("\n");

		Bit16u umb_start=dos_infoblock.GetStartOfUMBChain();
		Bit8u umb_flag=dos_infoblock.GetUMBChainState();
		Bit8u old_memstrat=DOS_GetMemAllocStrategy()&0xff;
		if (umb_start!=0xffff) {
			if ((umb_flag&1)==1) DOS_LinkUMBsToMemChain(0);
			DOS_SetMemAllocStrategy(0);
		}

		Bit16u seg,blocks;blocks=0xffff;
		DOS_AllocateMemory(&seg,&blocks);
//		if ((machine==MCH_PCJR) && (real_readb(0x2000,0)==0x5a) && (real_readw(0x2000,1)==0) && (real_readw(0x2000,3)==0x7ffe)) {
//			WriteOut(MSG_Get("PROGRAM_MEM_CONVEN"),0x7ffe*16/1024);
//		} else
			WriteOut(MSG_Get("PROGRAM_MEM_CONVEN"),blocks*16/1024);

		if (umb_start!=0xffff) {
			DOS_LinkUMBsToMemChain(1);
			DOS_SetMemAllocStrategy(0x40);	// search in UMBs only

			Bit16u largest_block=0,total_blocks=0,block_count=0;
			for (;; block_count++) {
				blocks=0xffff;
				DOS_AllocateMemory(&seg,&blocks);
				if (blocks==0) break;
				total_blocks+=blocks;
				if (blocks>largest_block) largest_block=blocks;
				DOS_AllocateMemory(&seg,&blocks);
			}

			Bit8u current_umb_flag=dos_infoblock.GetUMBChainState();
			if ((current_umb_flag&1)!=(umb_flag&1)) DOS_LinkUMBsToMemChain(umb_flag);
			DOS_SetMemAllocStrategy(old_memstrat);	// restore strategy

			if (block_count>0) WriteOut(MSG_Get("PROGRAM_MEM_UPPER"),total_blocks*16/1024,block_count,largest_block*16/1024);
		}

		/* Test for and show free XMS */
		reg_ax=0x4300;CALLBACK_RunRealInt(0x2f);
		if (reg_al==0x80) {
			reg_ax=0x4310;CALLBACK_RunRealInt(0x2f);
			Bit16u xms_seg=SegValue(es);Bit16u xms_off=reg_bx;
			reg_ah=8;
			CALLBACK_RunRealFar(xms_seg,xms_off);
			if (!reg_bl) {
				WriteOut(MSG_Get("PROGRAM_MEM_EXTEND"),reg_dx);
			}
		}	
		/* Test for and show free EMS */
		Bit16u handle;
		char emm[9] = { 'E','M','M','X','X','X','X','0',0 };
		if (DOS_OpenFile(emm,0,&handle)) {
			DOS_CloseFile(handle);
			reg_ah=0x42;
			CALLBACK_RunRealInt(0x67);
			WriteOut(MSG_Get("PROGRAM_MEM_EXPAND"),reg_bx*16);
		}
	}
};


static void MEM_ProgramStart(Program * * make) {
	*make=new MEM;
}

extern Bit32u floppytype;

void XMS_Clear();
void XMS_Init(Section* /*sec*/);
void EMS_Clear();
void EMS_Init(Section* /*sec*/);

class BOOT : public Program {
private:

	DBFILE*getFSFile_mounted(char const* filename, Bit32u *ksize, Bit32u *bsize, Bit8u *error) {
		//if return NULL then put in error the errormessage code if an error was requested
		bool tryload = (*error)?true:false;
		*error = 0;
		Bit8u drive;
		DBFILE*tmpfile;
		char fullname[DOS_PATHLENGTH];

		localDrive* ldp=0;
		if (!DOS_MakeName(const_cast<char*>(filename),fullname,&drive)) return NULL;

		try {		
			ldp=dynamic_cast<localDrive*>(Drives[drive]);
			if(!ldp) return NULL;

			tmpfile = ldp->GetSystemFilePtr(fullname, "rb");
			if(tmpfile == NULL) {
				if (!tryload) *error=1;
				return NULL;
			}

			// get file size
			dbfseek(tmpfile,0L, SEEK_END);
			*ksize = ( dbftell(tmpfile) / 1024);
			*bsize = dbftell(tmpfile);
			dbfclose(tmpfile);

			tmpfile = ldp->GetSystemFilePtr(fullname, "rb+");
			if(tmpfile == NULL) {
				//				if (!tryload) *error=2;
				//				return NULL;
				WriteOut(MSG_Get("PROGRAM_BOOT_WRITE_PROTECTED"));
				tmpfile = ldp->GetSystemFilePtr(fullname, "rb");
				if(tmpfile == NULL) {
					if (!tryload) *error=1;
					return NULL;
				}
			}

			return tmpfile;
		}
		catch(...) {
			return NULL;
		}
	}

	DBFILE*getFSFile(char const * filename, Bit32u *ksize, Bit32u *bsize,bool tryload=false) {
		Bit8u error = tryload?1:0;
		DBFILE* tmpfile = getFSFile_mounted(filename,ksize,bsize,&error);
		if(tmpfile) return tmpfile;
		//File not found on mounted filesystem. Try regular filesystem
		std::string filename_s(filename);
//		Cross::ResolveHomedir(filename_s);
		tmpfile = dbfopen(filename_s.c_str(),"rb+");
		if(!tmpfile) {
			if( (tmpfile = dbfopen(filename_s.c_str(),"rb")) ) {
				//File exists; So can't be opened in correct mode => error 2
				// dbfclose(tmpfile);
				//				if(tryload) error = 2;
				WriteOut(MSG_Get("PROGRAM_BOOT_WRITE_PROTECTED"));
				dbfseek(tmpfile,0L, SEEK_END);
				*ksize = ( dbftell(tmpfile) / 1024);
				*bsize = dbftell(tmpfile);
				return tmpfile;
			}
			// Give the delayed errormessages from the mounted variant (or from above)
			if(error == 1) WriteOut(MSG_Get("PROGRAM_BOOT_NOT_EXIST"));
			if(error == 2) WriteOut(MSG_Get("PROGRAM_BOOT_NOT_OPEN"));
			return NULL;
		}
		dbfseek(tmpfile,0L, SEEK_END);
		*ksize = ( dbftell(tmpfile) / 1024);
		*bsize = dbftell(tmpfile);
		return tmpfile;
	}

	void printError(void) {
		WriteOut(MSG_Get("PROGRAM_BOOT_PRINT_ERROR"));
	}

	void disable_umb_ems_xms(void)
	{
//		Section* dos_sec = myldbi->control->GetSection("dos");
//		dos_sec->ExecuteDestroy(false);
//		char test[20];
//		strcpy(test,"umb=false");
//		dos_sec->HandleInputline(test);
//		strcpy(test,"xms=false");
//		dos_sec->HandleInputline(test);
//		strcpy(test,"ems=false");
//		dos_sec->HandleInputline(test);
//		dos_sec->ExecuteInit(false);
		EMS_Clear();
		XMS_Clear();
		myldbi->GetConfig()->mem.ems = ALDB_MEM::LDB_MEM_EMS_DISABLE;
		myldbi->GetConfig()->mem.xms = false;
		myldbi->GetConfig()->mem.umb = false;
		XMS_Init(NULL);
		EMS_Init(NULL);
	}

public:

	void Run(void) {
		//Hack To allow long commandlines
		ChangeToLongCmd();
		/* In secure mode don't allow people to boot stuff. 
		 * They might try to corrupt the data on it */
		if(myldbi->GetConfig()->secure) {
			WriteOut(MSG_Get("PROGRAM_CONFIG_SECURE_DISALLOW"));
			return;
		}

		DBFILE* usefile_1=NULL;
		DBFILE* usefile_2=NULL;
		Bitu i=0; 
		Bit32u floppysize;
		Bit32u rombytesize_1=0;
		Bit32u rombytesize_2=0;
		Bit8u drive = 'A';
		std::string cart_cmd="";

		if(!cmd->GetCount()) {
			printError();
			return;
		}
		while(i<cmd->GetCount()) {
			if(cmd->FindCommand(i+1, temp_line)) {
				if((temp_line == "-l") || (temp_line == "-L")) {
					/* Specifying drive... next argument then is the drive */
					i++;
					if(cmd->FindCommand(i+1, temp_line)) {
						drive=toupper(temp_line[0]);
						if ((drive != 'A') && (drive != 'C') && (drive != 'D')) {
							printError();
							return;
						}

					} else {
						printError();
						return;
					}
					i++;
					continue;
				}

				if((temp_line == "-e") || (temp_line == "-E")) {
					/* Command mode for PCJr cartridges */
					i++;
					if(cmd->FindCommand(i + 1, temp_line)) {
						for(size_t ct = 0;ct < temp_line.size();ct++) temp_line[ct] = toupper(temp_line[ct]);
						cart_cmd = temp_line;
					} else {
						printError();
						return;
					}
					i++;
					continue;
				}

				WriteOut(MSG_Get("PROGRAM_BOOT_IMAGE_OPEN"), temp_line.c_str());
				Bit32u rombytesize;
				DBFILE*usefile = getFSFile(temp_line.c_str(), &floppysize, &rombytesize);
				if(usefile != NULL) {
					if(diskSwap[i] != NULL) delete diskSwap[i];
					diskSwap[i] = new imageDisk(usefile, (Bit8u *)temp_line.c_str(), floppysize, false);
					if (usefile_1==NULL) {
						usefile_1=usefile;
						rombytesize_1=rombytesize;
					} else {
						usefile_2=usefile;
						rombytesize_2=rombytesize;
					}
				} else {
					WriteOut(MSG_Get("PROGRAM_BOOT_IMAGE_NOT_OPEN"), temp_line.c_str());
					return;
				}

			}
			i++;
		}

		swapPosition = 0;

		swapInDisks();

		if(imageDiskList[drive-65]==NULL) {
			WriteOut(MSG_Get("PROGRAM_BOOT_UNABLE"), drive);
			return;
		}

		bootSector bootarea;
		imageDiskList[drive-65]->Read_Sector(0,0,1,(Bit8u *)&bootarea);
		if ((bootarea.rawdata[0]==0x50) && (bootarea.rawdata[1]==0x43) && (bootarea.rawdata[2]==0x6a) && (bootarea.rawdata[3]==0x72)) {
			WriteOut(MSG_Get("PROGRAM_BOOT_CART_WO_PCJR"));
		} else {
			disable_umb_ems_xms();
			void RemoveEMSPageFrame(void);
			RemoveEMSPageFrame();
			WriteOut(MSG_Get("PROGRAM_BOOT_BOOT"), drive);
			for(i=0;i<512;i++) real_writeb(0, 0x7c00 + i, bootarea.rawdata[i]);

			/* revector some dos-allocated interrupts */
			real_writed(0,0x01*4,0xf000ff53);
			real_writed(0,0x03*4,0xf000ff53);

			SegSet16(cs, 0);
			reg_ip = 0x7c00;
			SegSet16(ds, 0);
			SegSet16(es, 0);
			/* set up stack at a safe place */
			SegSet16(ss, 0x7000);
			reg_esp = 0x100;
			reg_esi = 0;
			reg_ecx = 1;
			reg_ebp = 0;
			reg_eax = 0;
			reg_edx = 0; //Head 0 drive 0
			reg_ebx= 0x7c00; //Real code probably uses bx to load the image
		}
	}
};

static void BOOT_ProgramStart(Program * * make) {
	*make=new BOOT;
}


#if C_DEBUG
class LDGFXROM : public Program {
public:
	void Run(void) {
		if (!(cmd->FindCommand(1, temp_line))) return;

		Bit8u drive;
		char fullname[DOS_PATHLENGTH];

		localDrive* ldp=0;
		if (!DOS_MakeName((char *)temp_line.c_str(),fullname,&drive)) return;

		try {		
			ldp=dynamic_cast<localDrive*>(Drives[drive]);
			if(!ldp) return;

			DBFILE*tmpfile = ldp->GetSystemFilePtr(fullname, "rb");
			if(tmpfile == NULL) {
				LOG_MSG("BIOS file not accessible.");
				return;
			}
			dbfseek(tmpfile, 0L, SEEK_END);
			if ( dbftell(tmpfile)>0x10000) {
				LOG_MSG("BIOS file too large.");
				return;
			}
			dbfseek(tmpfile, 0L, SEEK_SET);

			PhysPt rom_base=PhysMake(0xc000,0);

			Bit8u vga_buffer[0x10000];
			Bitu data_written=0;
			Bitu data_read= dbfread(vga_buffer, 1, 0x10000, tmpfile);
			for (Bitu ct=0; ct<data_read; ct++) {
				phys_writeb(rom_base+(data_written++),vga_buffer[ct]);
			} dbfclose(tmpfile);

			rom_base=PhysMake(0xf000,0);
			phys_writeb(rom_base+0xf065,0xcf);
		}
		catch(...) {
			return;
		}

		reg_flags&=~FLAG_IF;
		CALLBACK_RunRealFar(0xc000,0x0003);
	}
};

static void LDGFXROM_ProgramStart(Program * * make) {
	*make=new LDGFXROM;
}
#endif


// LOADFIX

class LOADFIX : public Program {
public:
	void Run(void);
};

void LOADFIX::Run(void) 
{
	Bit16u commandNr	= 1;
	Bit16u kb			= 64;
	if (cmd->FindCommand(commandNr,temp_line)) {
		if (temp_line[0]=='-') {
			char ch = temp_line[1];
			if ((*upcase(&ch)=='D') || (*upcase(&ch)=='F')) {
				// Deallocate all
				DOS_FreeProcessMemory(0x40);
				WriteOut(MSG_Get("PROGRAM_LOADFIX_DEALLOCALL"),kb);
				return;
			} else {
				// Set mem amount to allocate
				kb = atoi(temp_line.c_str()+1);
				if (kb==0) kb=64;
				commandNr++;
			}
		}
	}
	// Allocate Memory
	Bit16u segment;
	Bit16u blocks = kb*1024/16;
	if (DOS_AllocateMemory(&segment,&blocks)) {
		DOS_MCB mcb((Bit16u)(segment-1));
		mcb.SetPSPSeg(0x40);			// use fake segment
		WriteOut(MSG_Get("PROGRAM_LOADFIX_ALLOC"),kb);
		// Prepare commandline...
		if (cmd->FindCommand(commandNr++,temp_line)) {
			// get Filename
			char filename[128];
			safe_strncpy(filename,temp_line.c_str(),128);
			// Setup commandline
			bool ok;
			char args[256];
			args[0] = 0;
			do {
				ok = cmd->FindCommand(commandNr++,temp_line);
				if(sizeof(args)-strlen(args)-1 < temp_line.length()+1)
					break;
				strcat(args,temp_line.c_str());
				strcat(args," ");
			} while (ok);			
			// Use shell to start program
			DOS_Shell shell;
			shell.Execute(filename,args);
			DOS_FreeMemory(segment);		
			WriteOut(MSG_Get("PROGRAM_LOADFIX_DEALLOC"),kb);
		}
	} else {
		WriteOut(MSG_Get("PROGRAM_LOADFIX_ERROR"),kb);	
	}
}

static void LOADFIX_ProgramStart(Program * * make) {
	*make=new LOADFIX;
}

// RESCAN

class RESCAN : public Program {
public:
	void Run(void);
};

void RESCAN::Run(void) 
{
	bool all = false;

	Bit8u drive = DOS_GetDefaultDrive();

	if(cmd->FindCommand(1,temp_line)) {
		//-A -All /A /All 
		if(temp_line.size() >= 2 && (temp_line[0] == '-' ||temp_line[0] =='/')&& (temp_line[1] == 'a' || temp_line[1] =='A') ) all = true;
		else if(temp_line.size() == 2 && temp_line[1] == ':') {
			lowcase(temp_line);
			drive  = temp_line[0] - 'a';
		}
	}
	// Get current drive
	if (all) {
		for(Bitu i =0; i<DOS_DRIVES;i++) {
			if (Drives[i]) Drives[i]->EmptyCache();
		}
		WriteOut(MSG_Get("PROGRAM_RESCAN_SUCCESS"));
	} else {
		if (drive < DOS_DRIVES && Drives[drive]) {
			Drives[drive]->EmptyCache();
			WriteOut(MSG_Get("PROGRAM_RESCAN_SUCCESS"));
		}
	}
}

static void RESCAN_ProgramStart(Program * * make) {
	*make=new RESCAN;
}

class IMGMOUNT : public Program {
public:
	void Run(void) {
		//Hack To allow long commandlines
		ChangeToLongCmd();
		/* In secure mode don't allow people to change imgmount points. 
		 * Neither mount nor unmount */
		if(myldbi->GetConfig()->secure) {
			WriteOut(MSG_Get("PROGRAM_CONFIG_SECURE_DISALLOW"));
			return;
		}
		DOS_Drive * newdrive = NULL;
		imageDisk * newImage = NULL;
		Bit32u imagesize;
		char drive;
		std::string label;
		std::vector<std::string> paths;
		std::string umount;
		/* Check for unmounting */
		if (cmd->FindString("-u",umount,false)) {
			umount[0] = toupper(umount[0]);
			int i_drive = umount[0]-'A';
			if (i_drive < DOS_DRIVES && i_drive >= 0 && Drives[i_drive]) {
				switch (DriveManager::UnmountDrive(i_drive)) {
				case 0:
					Drives[i_drive] = 0;
					if (i_drive == DOS_GetDefaultDrive())
						DOS_SetDrive(toupper('Z') - 'A');
					WriteOut(MSG_Get("PROGRAM_MOUNT_UMOUNT_SUCCESS"),umount[0]);
					break;
				case 1:
					WriteOut(MSG_Get("PROGRAM_MOUNT_UMOUNT_NO_VIRTUAL"));
					break;
				case 2:
					WriteOut(MSG_Get("MSCDEX_ERROR_MULTIPLE_CDROMS"));
					break;
				}
			} else {
				WriteOut(MSG_Get("PROGRAM_MOUNT_UMOUNT_NOT_MOUNTED"),umount[0]);
			}
			return;
		}


		std::string type="hdd";
		std::string fstype="fat";
		cmd->FindString("-t",type,true);
		cmd->FindString("-fs",fstype,true);
		if(type == "cdrom") type = "iso"; //Tiny hack for people who like to type -t cdrom
		Bit8u mediaid;
		if (type=="floppy" || type=="hdd" || type=="iso") {
			Bit16u sizes[4];
			bool imgsizedetect=false;

			std::string str_size;
			mediaid=0xF8;

			if (type=="floppy") {
				mediaid=0xF0;		
			} else if (type=="iso") {
				str_size=="2048,1,60000,0";	// ignored, see drive_iso.cpp (AllocationInfo)
				mediaid=0xF8;		
				fstype = "iso";
			} 
			cmd->FindString("-size",str_size,true);
			if ((type=="hdd") && (str_size.size()==0)) {
				imgsizedetect=true;
			} else {
				char number[20];
				const char * scan=str_size.c_str();
				Bitu index=0;Bitu count=0;

				while (*scan) {
					if (*scan==',') {
						number[index]=0;sizes[count++]=atoi(number);
						index=0;
					} else number[index++]=*scan;
					scan++;
				}
				number[index]=0;sizes[count++]=atoi(number);
			}

			if(fstype=="fat" || fstype=="iso") {
				// get the drive letter
				if (!cmd->FindCommand(1,temp_line) || (temp_line.size() > 2) || ((temp_line.size()>1) && (temp_line[1]!=':'))) {
					WriteOut_NoParsing(MSG_Get("PROGRAM_IMGMOUNT_SPECIFY_DRIVE"));
					return;
				}
				drive=toupper(temp_line[0]);
				if (!isalpha(drive)) {
					WriteOut_NoParsing(MSG_Get("PROGRAM_IMGMOUNT_SPECIFY_DRIVE"));
					return;
				}
			} else if (fstype=="none") {
				cmd->FindCommand(1,temp_line);
				if ((temp_line.size() > 1) || (!isdigit(temp_line[0]))) {
					WriteOut_NoParsing(MSG_Get("PROGRAM_IMGMOUNT_SPECIFY2"));
					return;
				}
				drive=temp_line[0];
				if ((drive<'0') || (drive>3+'0')) {
					WriteOut_NoParsing(MSG_Get("PROGRAM_IMGMOUNT_SPECIFY2"));
					return;
				}
			} else {
				WriteOut(MSG_Get("PROGRAM_IMGMOUNT_FORMAT_UNSUPPORTED"),fstype.c_str());
				return;
			}

			// find all file parameters, assuming that all option parameters have been removed
			while(cmd->FindCommand((unsigned int)(paths.size() + 2), temp_line) && temp_line.size()) {

//				struct stat test;
//				if (stat(temp_line.c_str(),&test)) { //native filename couldn't be resolved
				if (!dbisitexist(temp_line.c_str())) { //native filename couldn't be resolved
					// convert dosbox filename to system filename
					char fullname[CROSS_LEN];
					char tmp[CROSS_LEN];
					safe_strncpy(tmp, temp_line.c_str(), CROSS_LEN);

					Bit8u dummy;
					if (!DOS_MakeName(tmp, fullname, &dummy) || strncmp(Drives[dummy]->GetInfo(),"local directory",15)) {
						WriteOut(MSG_Get("PROGRAM_IMGMOUNT_NON_LOCAL_DRIVE"));
						return;
					}

					localDrive *ldp = dynamic_cast<localDrive*>(Drives[dummy]);
					if (ldp==NULL) {
						WriteOut(MSG_Get("PROGRAM_IMGMOUNT_FILE_NOT_FOUND"));
						return;
					}
					ldp->GetSystemFilename(tmp, fullname);
					temp_line = tmp;

//					if (stat(temp_line.c_str(),&test)) {
					if (!dbisitexist(temp_line.c_str())) {
						WriteOut(MSG_Get("PROGRAM_IMGMOUNT_FILE_NOT_FOUND"));
						return;
					}
				}
//				if ((test.st_mode & S_IFDIR)) {
				if (dbisdirex(temp_line.c_str())) {
					WriteOut(MSG_Get("PROGRAM_IMGMOUNT_MOUNT"));
					return;
				}
				paths.push_back(temp_line);
			}
			if (paths.size() == 0) {
				WriteOut(MSG_Get("PROGRAM_IMGMOUNT_SPECIFY_FILE"));
				return;	
			}
			if (paths.size() == 1)
				temp_line = paths[0];

			if(fstype=="fat") {
				if (imgsizedetect) {
					DBFILE* diskfile = dbfopen(temp_line.c_str(), "rb+");
					if(!diskfile) {
						WriteOut(MSG_Get("PROGRAM_IMGMOUNT_INVALID_IMAGE"));
						return;
					}
					dbfseek(diskfile, 0L, SEEK_END);
					Bit32u fcsize = (Bit32u)( dbftell(diskfile) / 512L);
					Bit8u buf[512];
					dbfseek(diskfile, 0L, SEEK_SET);
					if (dbfread(buf,sizeof(Bit8u),512,diskfile)<512) {
						dbfclose(diskfile);
						WriteOut(MSG_Get("PROGRAM_IMGMOUNT_INVALID_IMAGE"));
						return;
					}
					dbfclose(diskfile);
					if ((buf[510]!=0x55) || (buf[511]!=0xaa)) {
						WriteOut(MSG_Get("PROGRAM_IMGMOUNT_INVALID_GEOMETRY"));
						return;
					}
					Bitu sectors=(Bitu)(fcsize/(16*63));
					if (sectors*16*63!=fcsize) {
						WriteOut(MSG_Get("PROGRAM_IMGMOUNT_INVALID_GEOMETRY"));
						return;
					}
					sizes[0]=512;	sizes[1]=63;	sizes[2]=16;	sizes[3]=sectors;
					LOG_MSG("autosized image file: %d:%d:%d:%d",sizes[0],sizes[1],sizes[2],sizes[3]);
				}

				if (Drives[drive-'A']) {
					WriteOut(MSG_Get("PROGRAM_IMGMOUNT_ALREADY_MOUNTED"));
					return;
				}

				std::vector<DOS_Drive*> imgDisks;
				std::vector<std::string>::size_type i;
				std::vector<DOS_Drive*>::size_type ct;

				for (i = 0; i < paths.size(); i++) {
					DOS_Drive* newDrive = new fatDrive(paths[i].c_str(),sizes[0],sizes[1],sizes[2],sizes[3],0);
					imgDisks.push_back(newDrive);
					if(!(dynamic_cast<fatDrive*>(newDrive))->created_successfully) {
						WriteOut(MSG_Get("PROGRAM_IMGMOUNT_CANT_CREATE"));
						for(ct = 0; ct < imgDisks.size(); ct++) {
							delete imgDisks[ct];
						}
						return;
					}
				}

				// Update DriveManager
				for(ct = 0; ct < imgDisks.size(); ct++) {
					DriveManager::AppendDisk(drive - 'A', imgDisks[ct]);
				}
				DriveManager::InitializeDrive(drive - 'A');

				// Set the correct media byte in the table 
				mem_writeb(Real2Phys(dos.tables.mediaid) + (drive - 'A') * 2, mediaid);

				/* Command uses dta so set it to our internal dta */
				RealPt save_dta = dos.dta();
				dos.dta(dos.tables.tempdta);

				for(ct = 0; ct < imgDisks.size(); ct++) {
					DriveManager::CycleAllDisks();

					char root[4] = {drive, ':', '\\', 0};
					DOS_FindFirst(root, DOS_ATTR_VOLUME); // force obtaining the label and saving it in dirCache
				}
				dos.dta(save_dta);

				std::string tmp(paths[0]);
				for (i = 1; i < paths.size(); i++) {
					tmp += "; " + paths[i];
				}
				WriteOut(MSG_Get("PROGRAM_MOUNT_STATUS_2"), drive, tmp.c_str());

				if (paths.size() == 1) {
					newdrive = imgDisks[0];
					if(((fatDrive *)newdrive)->loadedDisk->hardDrive) {
						if(imageDiskList[2] == NULL) {
							imageDiskList[2] = ((fatDrive *)newdrive)->loadedDisk;
							updateDPT();
							return;
						}
						if(imageDiskList[3] == NULL) {
							imageDiskList[3] = ((fatDrive *)newdrive)->loadedDisk;
							updateDPT();
							return;
						}
					}
					if(!((fatDrive *)newdrive)->loadedDisk->hardDrive) {
						imageDiskList[0] = ((fatDrive *)newdrive)->loadedDisk;
					}
				}
			} else if (fstype=="iso") {

				if (Drives[drive-'A']) {
					WriteOut(MSG_Get("PROGRAM_IMGMOUNT_ALREADY_MOUNTED"));
					return;
				}
				//				MSCDEX_SetCDInterface(CDROM_USE_SDL, -1);
				// create new drives for all images
				std::vector<DOS_Drive*> isoDisks;
				std::vector<std::string>::size_type i;
				std::vector<DOS_Drive*>::size_type ct;
				for (i = 0; i < paths.size(); i++) {
					int error = -1;
					DOS_Drive* newDrive = new isoDrive(drive, paths[i].c_str(), mediaid, error);
					isoDisks.push_back(newDrive);
					switch (error) {
					case 0  :	break;
					case 1  :	WriteOut(MSG_Get("MSCDEX_ERROR_MULTIPLE_CDROMS"));	break;
					case 2  :	WriteOut(MSG_Get("MSCDEX_ERROR_NOT_SUPPORTED"));	break;
					case 3  :	WriteOut(MSG_Get("MSCDEX_ERROR_OPEN"));				break;
					case 4  :	WriteOut(MSG_Get("MSCDEX_TOO_MANY_DRIVES"));		break;
					case 5  :	WriteOut(MSG_Get("MSCDEX_LIMITED_SUPPORT"));		break;
					case 6  :	WriteOut(MSG_Get("MSCDEX_INVALID_FILEFORMAT"));		break;
					default :	WriteOut(MSG_Get("MSCDEX_UNKNOWN_ERROR"));			break;
					}
					// error: clean up and leave
					if (error) {
						for(ct = 0; ct < isoDisks.size(); ct++) {
							delete isoDisks[ct];
						}
						return;
					}
				}
				// Update DriveManager
				for(ct = 0; ct < isoDisks.size(); ct++) {
					DriveManager::AppendDisk(drive - 'A', isoDisks[ct]);
				}
				DriveManager::InitializeDrive(drive - 'A');

				// Set the correct media byte in the table 
				mem_writeb(Real2Phys(dos.tables.mediaid) + (drive - 'A') * 2, mediaid);

				// Print status message (success)
				WriteOut(MSG_Get("MSCDEX_SUCCESS"));
				std::string tmp(paths[0]);
				for (i = 1; i < paths.size(); i++) {
					tmp += "; " + paths[i];
				}
				WriteOut(MSG_Get("PROGRAM_MOUNT_STATUS_2"), drive, tmp.c_str());

			} else {
				DBFILE*newDisk = dbfopen(temp_line.c_str(), "rb+");
				dbfseek(newDisk,0L, SEEK_END);
				imagesize = ( dbftell(newDisk) / 1024);

				newImage = new imageDisk(newDisk, (Bit8u *)temp_line.c_str(), imagesize, (imagesize > 2880));
				if(imagesize>2880) newImage->Set_Geometry(sizes[2],sizes[3],sizes[1],sizes[0]);
			}
		} else {
			WriteOut(MSG_Get("PROGRAM_IMGMOUNT_TYPE_UNSUPPORTED"),type.c_str());
			return;
		}

		if (fstype=="none") {
			if(imageDiskList[drive-'0'] != NULL) delete imageDiskList[drive-'0'];
			imageDiskList[drive-'0'] = newImage;
			updateDPT();
			WriteOut(MSG_Get("PROGRAM_IMGMOUNT_MOUNT_NUMBER"),drive-'0',temp_line.c_str());
		}

		// check if volume label is given. becareful for cdrom
		//if (cmd->FindString("-label",label,true)) newdrive->dirCache.SetLabel(label.c_str());
		return;
	}
};

void IMGMOUNT_ProgramStart(Program * * make) {
	*make=new IMGMOUNT;
}


Bitu DOS_SwitchKeyboardLayout(const char* new_layout, Bit32s& tried_cp);
Bitu DOS_LoadKeyboardLayout(const char * layoutname, Bit32s codepage, const char * codepagefile);
const char* DOS_GetLoadedLayout(void);

class KEYB : public Program {
public:
	void Run(void);
};

void KEYB::Run(void) {
	if (cmd->FindCommand(1,temp_line)) {
		if (cmd->FindString("?",temp_line,false)) {
			WriteOut(MSG_Get("PROGRAM_KEYB_SHOWHELP"));
		} else {
			/* first parameter is layout ID */
			Bitu keyb_error=0;
			std::string cp_string;
			Bit32s tried_cp = -1;
			if (cmd->FindCommand(2,cp_string)) {
				/* second parameter is codepage number */
				tried_cp=atoi(cp_string.c_str());
				char cp_file_name[256];
				if (cmd->FindCommand(3,cp_string)) {
					/* third parameter is codepage file */
					strcpy(cp_file_name, cp_string.c_str());
				} else {
					/* no codepage file specified, use automatic selection */
					strcpy(cp_file_name, "auto");
				}

				keyb_error=DOS_LoadKeyboardLayout(temp_line.c_str(), tried_cp, cp_file_name);
			} else {
				keyb_error=DOS_SwitchKeyboardLayout(temp_line.c_str(), tried_cp);
			}
			switch (keyb_error) {
			case KEYB_NOERROR:
				WriteOut(MSG_Get("PROGRAM_KEYB_NOERROR"),temp_line.c_str(),dos.loaded_codepage);
				break;
			case KEYB_FILENOTFOUND:
				WriteOut(MSG_Get("PROGRAM_KEYB_FILENOTFOUND"),temp_line.c_str());
				WriteOut(MSG_Get("PROGRAM_KEYB_SHOWHELP"));
				break;
			case KEYB_INVALIDFILE:
				WriteOut(MSG_Get("PROGRAM_KEYB_INVALIDFILE"),temp_line.c_str());
				break;
			case KEYB_LAYOUTNOTFOUND:
				WriteOut(MSG_Get("PROGRAM_KEYB_LAYOUTNOTFOUND"),temp_line.c_str(),tried_cp);
				break;
			case KEYB_INVALIDCPFILE:
				WriteOut(MSG_Get("PROGRAM_KEYB_INVCPFILE"),temp_line.c_str());
				WriteOut(MSG_Get("PROGRAM_KEYB_SHOWHELP"));
				break;
			default:
				LOG(LOG_DOSMISC,LOG_ERROR)("KEYB:Invalid returncode %x",keyb_error);
				break;
			}
		}
	} else {
		/* no parameter in the command line, just output codepage info and possibly loaded layout ID */
		const char* layout_name = DOS_GetLoadedLayout();
		if (layout_name==NULL) {
			WriteOut(MSG_Get("PROGRAM_KEYB_INFO"),dos.loaded_codepage);
		} else {
			WriteOut(MSG_Get("PROGRAM_KEYB_INFO_LAYOUT"),dos.loaded_codepage,layout_name);
		}
	}
}

static void KEYB_ProgramStart(Program * * make) {
	*make=new KEYB;
}


void DOS_SetupPrograms(void) {
	/*Add Messages */

	MSG_Add("PROGRAM_MOUNT_CDROMS_FOUND","CDROMs found: %d\n");
	MSG_Add("PROGRAM_MOUNT_STATUS_FORMAT","%-5s  %-58s %-12s\n");
	MSG_Add("PROGRAM_MOUNT_STATUS_2","Drive %c is mounted as %s\n");
	MSG_Add("PROGRAM_MOUNT_STATUS_1","The currently mounted drives are:\n");
	MSG_Add("PROGRAM_MOUNT_ERROR_1","Directory %s doesn't exist.\n");
	MSG_Add("PROGRAM_MOUNT_ERROR_2","%s isn't a directory\n");
	MSG_Add("PROGRAM_MOUNT_ILL_TYPE","Illegal type %s\n");
	MSG_Add("PROGRAM_MOUNT_ALREADY_MOUNTED","Drive %c already mounted with %s\n");
	MSG_Add("PROGRAM_MOUNT_USAGE",
			"Usage \033[34;1mMOUNT Drive-Letter Local-Directory\033[0m\n"
			"For example: MOUNT c %s\n"
			"This makes the directory %s act as the C: drive inside DOSBox.\n"
			"The directory has to exist.\n");
	MSG_Add("PROGRAM_MOUNT_UMOUNT_NOT_MOUNTED","Drive %c isn't mounted.\n");
	MSG_Add("PROGRAM_MOUNT_UMOUNT_SUCCESS","Drive %c has successfully been removed.\n");
	MSG_Add("PROGRAM_MOUNT_UMOUNT_NO_VIRTUAL","Virtual Drives can not be unMOUNTed.\n");
	MSG_Add("PROGRAM_MOUNT_WARNING_WIN","\033[31;1mMounting c:\\ is NOT recommended. Please mount a (sub)directory next time.\033[0m\n");
	MSG_Add("PROGRAM_MOUNT_WARNING_OTHER","\033[31;1mMounting / is NOT recommended. Please mount a (sub)directory next time.\033[0m\n");

	MSG_Add("PROGRAM_MEM_CONVEN","%10d Kb free conventional memory\n");
	MSG_Add("PROGRAM_MEM_EXTEND","%10d Kb free extended memory\n");
	MSG_Add("PROGRAM_MEM_EXPAND","%10d Kb free expanded memory\n");
	MSG_Add("PROGRAM_MEM_UPPER","%10d Kb free upper memory in %d blocks (largest UMB %d Kb)\n");

	MSG_Add("PROGRAM_LOADFIX_ALLOC","%d kb allocated.\n");
	MSG_Add("PROGRAM_LOADFIX_DEALLOC","%d kb freed.\n");
	MSG_Add("PROGRAM_LOADFIX_DEALLOCALL","Used memory freed.\n");
	MSG_Add("PROGRAM_LOADFIX_ERROR","Memory allocation error.\n");

	MSG_Add("MSCDEX_SUCCESS","MSCDEX installed.\n");
	MSG_Add("MSCDEX_ERROR_MULTIPLE_CDROMS","MSCDEX: Failure: Drive-letters of multiple CDRom-drives have to be continuous.\n");
	MSG_Add("MSCDEX_ERROR_NOT_SUPPORTED","MSCDEX: Failure: Not yet supported.\n");
	MSG_Add("MSCDEX_ERROR_OPEN","MSCDEX: Failure: Invalid file or unable to open.\n");
	MSG_Add("MSCDEX_TOO_MANY_DRIVES","MSCDEX: Failure: Too many CDRom-drives (max: 5). MSCDEX Installation failed.\n");
	MSG_Add("MSCDEX_LIMITED_SUPPORT","MSCDEX: Mounted subdirectory: limited support.\n");
	MSG_Add("MSCDEX_INVALID_FILEFORMAT","MSCDEX: Failure: File is either no iso/cue image or contains errors.\n");
	MSG_Add("MSCDEX_UNKNOWN_ERROR","MSCDEX: Failure: Unknown error.\n");

	MSG_Add("PROGRAM_RESCAN_SUCCESS","Drive cache cleared.\n");

	MSG_Add("PROGRAM_BOOT_NOT_EXIST","Bootdisk file does not exist.  Failing.\n");
	MSG_Add("PROGRAM_BOOT_NOT_OPEN","Cannot open bootdisk file.  Failing.\n");
	MSG_Add("PROGRAM_BOOT_WRITE_PROTECTED","Image file is read-only! Might create problems.\n");
	MSG_Add("PROGRAM_BOOT_PRINT_ERROR","This command boots DOSBox from either a floppy or hard disk image.\n\n"
			"For this command, one can specify a succession of floppy disks swappable\n"
			"by pressing <>, and -l specifies the mounted drive to boot from.  If\n"
			"no drive letter is specified, this defaults to booting from the A drive.\n"
			"The only bootable drive letters are A, C, and D.  For booting from a hard\n"
			"drive (C or D), the image should have already been mounted using the\n"
			"\033[34;1mIMGMOUNT\033[0m command.\n\n"
			"The syntax of this command is:\n\n"
			"\033[34;1mBOOT [diskimg1.img diskimg2.img] [-l driveletter]\033[0m\n"
	);
	MSG_Add("PROGRAM_BOOT_UNABLE","Unable to boot off of drive %c");
	MSG_Add("PROGRAM_BOOT_IMAGE_OPEN","Opening image file: %s\n");
	MSG_Add("PROGRAM_BOOT_IMAGE_NOT_OPEN","Cannot open %s");
	MSG_Add("PROGRAM_BOOT_BOOT","Booting from drive %c...\n");
	MSG_Add("PROGRAM_BOOT_CART_WO_PCJR","PCjr cartridge found, but machine is not PCjr");
	MSG_Add("PROGRAM_BOOT_CART_LIST_CMDS","Available PCjr cartridge commandos:%s");
	MSG_Add("PROGRAM_BOOT_CART_NO_CMDS","No PCjr cartridge commandos found");

	MSG_Add("PROGRAM_IMGMOUNT_SPECIFY_DRIVE","Must specify drive letter to mount image at.\n");
	MSG_Add("PROGRAM_IMGMOUNT_SPECIFY2","Must specify drive number (0 or 3) to mount image at (0,1=fda,fdb;2,3=hda,hdb).\n");
	MSG_Add("PROGRAM_IMGMOUNT_SPECIFY_GEOMETRY",
			"For \033[33mCD-ROM\033[0m images:   \033[34;1mIMGMOUNT drive-letter location-of-image -t iso\033[0m\n"
			"\n"
			"For \033[33mhardrive\033[0m images: Must specify drive geometry for hard drives:\n"
			"bytes_per_sector, sectors_per_cylinder, heads_per_cylinder, cylinder_count.\n"
			"\033[34;1mIMGMOUNT drive-letter location-of-image -size bps,spc,hpc,cyl\033[0m\n");
	MSG_Add("PROGRAM_IMGMOUNT_INVALID_IMAGE","Could not load image file.\n"
			"Check that the path is correct and the image is accessible.\n");
	MSG_Add("PROGRAM_IMGMOUNT_INVALID_GEOMETRY","Could not extract drive geometry from image.\n"
			"Use parameter -size bps,spc,hpc,cyl to specify the geometry.\n");
	MSG_Add("PROGRAM_IMGMOUNT_TYPE_UNSUPPORTED","Type \"%s\" is unsupported. Specify \"hdd\" or \"floppy\" or\"iso\".\n");
	MSG_Add("PROGRAM_IMGMOUNT_FORMAT_UNSUPPORTED","Format \"%s\" is unsupported. Specify \"fat\" or \"iso\" or \"none\".\n");
	MSG_Add("PROGRAM_IMGMOUNT_SPECIFY_FILE","Must specify file-image to mount.\n");
	MSG_Add("PROGRAM_IMGMOUNT_FILE_NOT_FOUND","Image file not found.\n");
	MSG_Add("PROGRAM_IMGMOUNT_MOUNT","To mount directories, use the \033[34;1mMOUNT\033[0m command, not the \033[34;1mIMGMOUNT\033[0m command.\n");
	MSG_Add("PROGRAM_IMGMOUNT_ALREADY_MOUNTED","Drive already mounted at that letter.\n");
	MSG_Add("PROGRAM_IMGMOUNT_CANT_CREATE","Can't create drive from file.\n");
	MSG_Add("PROGRAM_IMGMOUNT_MOUNT_NUMBER","Drive number %d mounted as %s\n");
	MSG_Add("PROGRAM_IMGMOUNT_NON_LOCAL_DRIVE", "The image must be on a host or local drive.\n");
	MSG_Add("PROGRAM_IMGMOUNT_MULTIPLE_NON_CUEISO_FILES", "Using multiple files is only supported for cue/iso images.\n");

	MSG_Add("PROGRAM_KEYB_INFO","Codepage %i has been loaded\n");
	MSG_Add("PROGRAM_KEYB_INFO_LAYOUT","Codepage %i has been loaded for layout %s\n");
	MSG_Add("PROGRAM_KEYB_SHOWHELP",
			"\033[32;1mKEYB\033[0m [keyboard layout ID[ codepage number[ codepage file]]]\n\n"
			"Some examples:\n"
			"  \033[32;1mKEYB\033[0m: Display currently loaded codepage.\n"
			"  \033[32;1mKEYB\033[0m sp: Load the spanish (SP) layout, use an appropriate codepage.\n"
			"  \033[32;1mKEYB\033[0m sp 850: Load the spanish (SP) layout, use codepage 850.\n"
			"  \033[32;1mKEYB\033[0m sp 850 mycp.cpi: Same as above, but use file mycp.cpi.\n");
	MSG_Add("PROGRAM_KEYB_NOERROR","Keyboard layout %s loaded for codepage %i\n");
	MSG_Add("PROGRAM_KEYB_FILENOTFOUND","Keyboard file %s not found\n\n");
	MSG_Add("PROGRAM_KEYB_INVALIDFILE","Keyboard file %s invalid\n");
	MSG_Add("PROGRAM_KEYB_LAYOUTNOTFOUND","No layout in %s for codepage %i\n");
	MSG_Add("PROGRAM_KEYB_INVCPFILE","None or invalid codepage file for layout %s\n\n");

	/*regular setup*/
	PROGRAMS_MakeFile("MOUNT.COM",MOUNT_ProgramStart);
	PROGRAMS_MakeFile("MEM.COM",MEM_ProgramStart);
	PROGRAMS_MakeFile("LOADFIX.COM",LOADFIX_ProgramStart);
	PROGRAMS_MakeFile("RESCAN.COM",RESCAN_ProgramStart);
//	PROGRAMS_MakeFile("INTRO.COM",INTRO_ProgramStart);
	PROGRAMS_MakeFile("BOOT.COM",BOOT_ProgramStart);
#if C_DEBUG
	PROGRAMS_MakeFile("LDGFXROM.COM", LDGFXROM_ProgramStart);
#endif
	PROGRAMS_MakeFile("IMGMOUNT.COM", IMGMOUNT_ProgramStart);
	PROGRAMS_MakeFile("KEYB.COM", KEYB_ProgramStart);
}

}
