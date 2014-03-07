/*
 *  Copyright (C) 2002-2013  The DOSBox Team
 *  Copyright (C) 2013-2014  Soloviov Dmitry
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

#ifndef MESSAGES_H_
#define MESSAGES_H_

#define PROGRAM_MOUNT_CDROMS_FOUND	"CDROMs found: %d\n"
#define PROGRAM_MOUNT_STATUS_FORMAT	"%-5s  %-58s %-12s\n"
#define PROGRAM_MOUNT_STATUS_2	"Drive %c is mounted as %s\n"
#define PROGRAM_MOUNT_STATUS_1	"The currently mounted drives are:\n"
#define PROGRAM_MOUNT_ERROR_1	"Directory %s doesn't exist.\n"
#define PROGRAM_MOUNT_ERROR_2	"%s isn't a directory\n"
#define PROGRAM_MOUNT_ILL_TYPE	"Illegal type %s\n"
#define PROGRAM_MOUNT_ALREADY_MOUNTED	"Drive %c already mounted with %s\n"
#define PROGRAM_MOUNT_USAGE "Usage \033[34;1mMOUNT Drive-Letter Local-Directory\033[0m\n" \
			"For example: MOUNT c %s\n" \
			"This makes the directory %s act as the C: drive inside DOSCARD.\n" \
			"The directory has to exist.\n"
#define PROGRAM_MOUNT_UMOUNT_NOT_MOUNTED	"Drive %c isn't mounted.\n"
#define PROGRAM_MOUNT_UMOUNT_SUCCESS	"Drive %c has successfully been removed.\n"
#define PROGRAM_MOUNT_UMOUNT_NO_VIRTUAL	"Virtual Drives can not be unmounted.\n"
#define PROGRAM_MOUNT_WARNING_WIN	"\033[31;1mMounting c:\\ is NOT recommended. Please mount a (sub)directory next time.\033[0m\n"
#define PROGRAM_MOUNT_WARNING_OTHER	"\033[31;1mMounting / is NOT recommended. Please mount a (sub)directory next time.\033[0m\n"

#define PROGRAM_MEM_CONVEN	"%10d Kb free conventional memory\n"
#define PROGRAM_MEM_EXTEND	"%10d Kb free extended memory\n"
#define PROGRAM_MEM_EXPAND	"%10d Kb free expanded memory\n"
#define PROGRAM_MEM_UPPER	"%10d Kb free upper memory in %d blocks (largest UMB %d Kb)\n"

#define PROGRAM_LOADFIX_ALLOC	"%d kb allocated.\n"
#define PROGRAM_LOADFIX_DEALLOC	"%d kb freed.\n"
#define PROGRAM_LOADFIX_DEALLOCALL	"Used memory freed.\n"
#define PROGRAM_LOADFIX_ERROR	"Memory allocation error.\n"

#define MSCDEX_SUCCESS	"MSCDEX installed.\n"
#define MSCDEX_ERROR_MULTIPLE_CDROMS	"MSCDEX: Failure: Drive-letters of multiple CDRom-drives have to be continuous.\n"
#define MSCDEX_ERROR_NOT_SUPPORTED	"MSCDEX: Failure: Not yet supported.\n"
#define MSCDEX_ERROR_OPEN	"MSCDEX: Failure: Invalid file or unable to open.\n"
#define MSCDEX_ERROR_PATH	"MSCDEX: Failure: Invalid path.\n"
#define MSCDEX_TOO_MANY_DRIVES	"MSCDEX: Failure: Too many CDRom-drives (max: 5). MSCDEX Installation failed.\n"
#define MSCDEX_LIMITED_SUPPORT	"MSCDEX: Mounted subdirectory: limited support.\n"
#define MSCDEX_INVALID_FILEFORMAT	"MSCDEX: Failure: File is either no iso/cue image or contains errors.\n"
#define MSCDEX_UNKNOWN_ERROR	"MSCDEX: Failure: Unknown error.\n"

#define PROGRAM_RESCAN_SUCCESS	"Drive cache cleared.\n"

#define PROGRAM_BOOT_NOT_EXIST	"Bootdisk file does not exist.  Failing.\n"
#define PROGRAM_BOOT_NOT_OPEN	"Cannot open bootdisk file.  Failing.\n"
#define PROGRAM_BOOT_WRITE_PROTECTED	"Image file is read-only! Might create problems.\n"
#define PROGRAM_BOOT_PRINT_ERROR	"This command boots DOSCARD from either a floppy or hard disk image.\n\n" \
			"For this command, one can specify a succession of floppy disks swappable\n" \
			"by pressing <>, and -l specifies the mounted drive to boot from.  If\n" \
			"no drive letter is specified, this defaults to booting from the A drive.\n" \
			"The only bootable drive letters are A, C, and D.  For booting from a hard\n" \
			"drive (C or D), the image should have already been mounted using the\n" \
			"\033[34;1mIMGMOUNT\033[0m command.\n\n" \
			"The syntax of this command is:\n\n" \
			"\033[34;1mBOOT [diskimg1.img diskimg2.img] [-l driveletter]\033[0m\n"

#define PROGRAM_BOOT_UNABLE	"Unable to boot off of drive %c"
#define PROGRAM_BOOT_IMAGE_OPEN	"Opening image file: %s\n"
#define PROGRAM_BOOT_IMAGE_NOT_OPEN	"Cannot open %s"
#define PROGRAM_BOOT_BOOT	"Booting from drive %c...\n"
#define PROGRAM_BOOT_CART_WO_PCJR	"PCjr cartridge found, but machine is not PCjr"
#define PROGRAM_BOOT_CART_LIST_CMDS	"Available PCjr cartridge commandos:%s"
#define PROGRAM_BOOT_CART_NO_CMDS	"No PCjr cartridge commandos found"

#define PROGRAM_IMGMOUNT_SPECIFY_DRIVE	"Must specify drive letter to mount image at.\n"
#define PROGRAM_IMGMOUNT_SPECIFY2	"Must specify drive number (0 or 3) to mount image at (0,1=fda,fdb;2,3=hda,hdb).\n"
#define PROGRAM_IMGMOUNT_SPECIFY_GEOMETRY "For \033[33mCD-ROM\033[0m images:   \033[34;1mIMGMOUNT drive-letter location-of-image -t iso\033[0m\n" \
			"\nFor \033[33mhardrive\033[0m images: Must specify drive geometry for hard drives:\n" \
			"bytes_per_sector, sectors_per_cylinder, heads_per_cylinder, cylinder_count.\n" \
			"\033[34;1mIMGMOUNT drive-letter location-of-image -size bps,spc,hpc,cyl\033[0m\n"
#define PROGRAM_IMGMOUNT_INVALID_IMAGE	"Could not load image file.\n" \
			"Check that the path is correct and the image is accessible.\n"
#define PROGRAM_IMGMOUNT_INVALID_GEOMETRY	"Could not extract drive geometry from image.\n" \
			"Use parameter -size bps,spc,hpc,cyl to specify the geometry.\n"
#define PROGRAM_IMGMOUNT_TYPE_UNSUPPORTED	"Type \"%s\" is unsupported. Specify \"hdd\" or \"floppy\" or\"iso\".\n"
#define PROGRAM_IMGMOUNT_FORMAT_UNSUPPORTED	"Format \"%s\" is unsupported. Specify \"fat\" or \"iso\" or \"none\".\n"
#define PROGRAM_IMGMOUNT_SPECIFY_FILE	"Must specify file-image to mount.\n"
#define PROGRAM_IMGMOUNT_FILE_NOT_FOUND	"Image file not found.\n"
#define PROGRAM_IMGMOUNT_MOUNT	"To mount directories, use the \033[34;1mMOUNT\033[0m command, not the \033[34;1mIMGMOUNT\033[0m command.\n"
#define PROGRAM_IMGMOUNT_ALREADY_MOUNTED	"Drive already mounted at that letter.\n"
#define PROGRAM_IMGMOUNT_CANT_CREATE	"Can't create drive from file.\n"
#define PROGRAM_IMGMOUNT_MOUNT_NUMBER	"Drive number %d mounted as %s\n"
#define PROGRAM_IMGMOUNT_NON_LOCAL_DRIVE "The image must be on a host or local drive.\n"
#define PROGRAM_IMGMOUNT_MULTIPLE_NON_CUEISO_FILES "Using multiple files is only supported for cue/iso images.\n"

#define PROGRAM_KEYB_INFO	"Codepage %i has been loaded\n"
#define PROGRAM_KEYB_INFO_LAYOUT	"Codepage %i has been loaded for layout %s\n"
#define PROGRAM_KEYB_SHOWHELP "\033[32;1mKEYB\033[0m [keyboard layout ID[ codepage number[ codepage file]]]\n\n" \
			"Some examples:\n" \
			"  \033[32;1mKEYB\033[0m: Display currently loaded codepage.\n" \
			"  \033[32;1mKEYB\033[0m sp: Load the spanish (SP) layout, use an appropriate codepage.\n" \
			"  \033[32;1mKEYB\033[0m sp 850: Load the spanish (SP) layout, use codepage 850.\n" \
			"  \033[32;1mKEYB\033[0m sp 850 mycp.cpi: Same as above, but use file mycp.cpi.\n"
#define PROGRAM_KEYB_NOERROR	"Keyboard layout %s loaded for codepage %i\n"
#define PROGRAM_KEYB_FILENOTFOUND	"Keyboard file %s not found\n\n"
#define PROGRAM_KEYB_INVALIDFILE	"Keyboard file %s invalid\n"
#define PROGRAM_KEYB_LAYOUTNOTFOUND	"No layout in %s for codepage %i\n"
#define PROGRAM_KEYB_INVCPFILE	"None or invalid codepage file for layout %s\n\n"

#define SHELL_ILLEGAL_PATH	"Illegal Path.\n"
#define SHELL_CMD_HELP	"If you want a list of all supported commands type \033[33;1mhelp /all\033[0m .\nA short list of the most often used commands:\n"
#define SHELL_CMD_ECHO_ON	"ECHO is on.\n"
#define SHELL_CMD_ECHO_OFF	"ECHO is off.\n"
#define SHELL_ILLEGAL_SWITCH	"Illegal switch: %s.\n"
#define SHELL_MISSING_PARAMETER	"Required parameter missing.\n"
#define SHELL_CMD_CHDIR_ERROR	"Unable to change to: %s.\n"
#define SHELL_CMD_CHDIR_HINT	"To change to different drive type \033[31m%c:\033[0m\n"
#define SHELL_CMD_CHDIR_HINT_2	"directoryname is longer than 8 characters and/or contains spaces.\nTry \033[31mcd %s\033[0m\n"
#define SHELL_CMD_CHDIR_HINT_3	"You are still on drive Z:, change to a mounted drive with \033[31mC:\033[0m.\n"
#define SHELL_CMD_DATE_HELP	"Displays or changes the internal date.\n"
#define SHELL_CMD_DATE_ERROR	"The specified date is not correct.\n"
#define SHELL_CMD_DATE_DAYS	"3SunMonTueWedThuFriSat"
#define SHELL_CMD_DATE_NOW	"Current date: "
#define SHELL_CMD_DATE_SETHLP	"Type 'date MM-DD-YYYY' to change.\n"
#define SHELL_CMD_DATE_FORMAT	"M/D/Y"
#define SHELL_CMD_DATE_HELP_LONG	"DATE [[/T] [/H] [/S] | MM-DD-YYYY]\n"\
									"  MM-DD-YYYY: new date to set\n"\
									"  /S:         Permanently use host time and date as DOS time\n"\
                                    "  /F:         Switch back to DOSCARD internal time (opposite of /S)\n"\
									"  /T:         Only display date\n"\
									"  /H:         Synchronize with host\n"
#define SHELL_CMD_TIME_HELP	"Displays the internal time.\n"
#define SHELL_CMD_TIME_NOW	"Current time: "
#define SHELL_CMD_TIME_HELP_LONG	"TIME [/T] [/H]\n"\
									"  /T:         Display simple time\n"\
									"  /H:         Synchronize with host\n"
#define SHELL_CMD_MKDIR_ERROR	"Unable to make: %s.\n"
#define SHELL_CMD_RMDIR_ERROR	"Unable to remove: %s.\n"
#define SHELL_CMD_DEL_ERROR	"Unable to delete: %s.\n"
#define SHELL_SYNTAXERROR	"The syntax of the command is incorrect.\n"
#define SHELL_CMD_SET_NOT_SET	"Environment variable %s not defined.\n"
#define SHELL_CMD_SET_OUT_OF_SPACE	"Not enough environment space left.\n"
#define SHELL_CMD_IF_EXIST_MISSING_FILENAME	"IF EXIST: Missing filename.\n"
#define SHELL_CMD_IF_ERRORLEVEL_MISSING_NUMBER	"IF ERRORLEVEL: Missing number.\n"
#define SHELL_CMD_IF_ERRORLEVEL_INVALID_NUMBER	"IF ERRORLEVEL: Invalid number.\n"
#define SHELL_CMD_GOTO_MISSING_LABEL	"No label supplied to GOTO command.\n"
#define SHELL_CMD_GOTO_LABEL_NOT_FOUND	"GOTO: Label %s not found.\n"
#define SHELL_CMD_FILE_NOT_FOUND	"File %s not found.\n"
#define SHELL_CMD_FILE_EXISTS	"File %s already exists.\n"
#define SHELL_CMD_DIR_INTRO	"Directory of %s.\n"
#define SHELL_CMD_DIR_BYTES_USED	"%5d File(s) %17s Bytes.\n"
#define SHELL_CMD_DIR_BYTES_FREE	"%5d Dir(s)  %17s Bytes free.\n"
#define SHELL_EXECUTE_DRIVE_NOT_FOUND	"Drive %c does not exist!\nYou must \033[31mmount\033[0m it first.\n"
#define SHELL_EXECUTE_ILLEGAL_COMMAND	"Illegal command: %s.\n"
#define SHELL_CMD_PAUSE	"Press any key to continue.\n"
#define SHELL_CMD_PAUSE_HELP	"Waits for 1 keystroke to continue.\n"
#define SHELL_CMD_COPY_FAILURE	"Copy failure : %s.\n"
#define SHELL_CMD_COPY_SUCCESS	"   %d File(s) copied.\n"
#define SHELL_CMD_SUBST_NO_REMOVE	"Unable to remove, drive not in use.\n"
#define SHELL_CMD_SUBST_FAILURE	"SUBST failed. You either made an error in your commandline or the target drive is already used.\nIt's only possible to use SUBST on Local drives"

#define SHELL_CMD_CHDIR_HELP	"Displays/changes the current directory.\n"
#define SHELL_CMD_CHDIR_HELP_LONG	"CHDIR [drive:][path]\n" \
	        "CHDIR [..]\nCD [drive:][path]\nCD [..]\n\n" \
	        "  ..   Specifies that you want to change to the parent directory.\n\n" \
	        "Type CD drive: to display the current directory in the specified drive.\n" \
	        "Type CD without parameters to display the current drive and directory.\n"
#define SHELL_CMD_CLS_HELP	"Clear screen.\n"
#define SHELL_CMD_DIR_HELP	"Directory View.\n"
#define SHELL_CMD_ECHO_HELP	"Display messages and enable/disable command echoing.\n"
#define SHELL_CMD_EXIT_HELP	"Exit from the shell.\n"
#define SHELL_CMD_HELP_HELP	"Show help.\n"
#define SHELL_CMD_MKDIR_HELP	"Make Directory.\n"
#define SHELL_CMD_MKDIR_HELP_LONG	"MKDIR [drive:][path]\nMD [drive:][path]\n"
#define SHELL_CMD_RMDIR_HELP	"Remove Directory.\n"
#define SHELL_CMD_RMDIR_HELP_LONG	"RMDIR [drive:][path]\nRD [drive:][path]\n"
#define SHELL_CMD_SET_HELP	"Change environment variables.\n"
#define SHELL_CMD_IF_HELP	"Performs conditional processing in batch programs.\n"
#define SHELL_CMD_GOTO_HELP	"Jump to a labeled line in a batch script.\n"
#define SHELL_CMD_SHIFT_HELP	"Leftshift commandline parameters in a batch script.\n"
#define SHELL_CMD_TYPE_HELP	"Display the contents of a text-file.\n"
#define SHELL_CMD_TYPE_HELP_LONG	"TYPE [drive:][path][filename]\n"
#define SHELL_CMD_REM_HELP	"Add comments in a batch file.\n"
#define SHELL_CMD_REM_HELP_LONG	"REM [comment]\n"
#define SHELL_CMD_NO_WILD	"This is a simple version of the command, no wildcards allowed!\n"
#define SHELL_CMD_RENAME_HELP	"Renames one or more files.\n"
#define SHELL_CMD_RENAME_HELP_LONG	"RENAME [drive:][path]filename1 filename2.\n" \
	        "REN [drive:][path]filename1 filename2.\n\n" \
	        "Note that you can not specify a new drive or path for your destination file.\n"
#define SHELL_CMD_DELETE_HELP	"Removes one or more files.\n"
#define SHELL_CMD_COPY_HELP	"Copy files.\n"
#define SHELL_CMD_CALL_HELP	"Start a batch file from within another batch file.\n"
#define SHELL_CMD_SUBST_HELP	"Assign an internal directory to a drive.\n"
#define SHELL_CMD_LOADHIGH_HELP	"Loads a program into upper memory (requires xms=true,umb=true).\n"
#define SHELL_CMD_CHOICE_HELP	"Waits for a keypress and sets ERRORLEVEL.\n"
#define SHELL_CMD_CHOICE_HELP_LONG	"CHOICE [/C:choices] [/N] [/S] text\n" \
	        "  /C[:]choices  -  Specifies allowable keys.  Default is: yn.\n" \
	        "  /N  -  Do not display the choices at end of prompt.\n" \
	        "  /S  -  Enables case-sensitive choices to be selected.\n" \
	        "  text  -  The text to display as a prompt.\n"
#define SHELL_CMD_ATTRIB_HELP	"Does nothing. Provided for compatibility.\n"
#define SHELL_CMD_PATH_HELP	"Provided for compatibility.\n"
#define SHELL_CMD_VER_HELP	"View and set the reported DOS version.\n"
#define SHELL_CMD_VER_VER	"DOSCARD version %s. Reported DOS version %d.%02d.\n"

#define PROGRAM_CONFIG_SECURE_DISALLOW "This operation is not permitted in secure mode.\n"

#endif /* MESSAGES_H_ */
