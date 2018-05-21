## DosCard Project Top Level Chain Makefile
## Copyright (C) 2013-2015 Dmitry Soloviov

all:
	echo -e "Targets available:\n \
	ldb     - libDosBox\n \
	ldc     - libDosCard multi-thread\n \
	ldcst   - libDosCard single-thread\n \
	mingw   - libDosCard single-thread for MinGW\n \
	xsh1    - XShell 1 native\n \
	xsh2    - XShell 2 multi-thread\n \
	xsh2st  - XShell 2 single-thread\n \
	hdless  - XHeadless testing program"
.PHONY: all

##libdosbox (native)
ldb:
	cd box; make all
.PHONY: ldb

##libdoscard (native + llvm)
ldc:
	cd box; make clean
	cd libdoscard; make mt
.PHONY: ldc

##libdoscard (native + native)
ldcst:
	cd box; make clean
	cd libdoscard; make st
.PHONY: ldcst

##libdoscard (mingw)
mingw:
	cd box; make clean
	cd libdoscard; make mingw
.PHONY: mingw

##XShell 1 (native, single-instance)
xsh1: ldb
	cd xshell; make all
.PHONY: xsh1

##XShell 2 (native, multi-instance)
xsh2: ldc
	cd xshell2; make all
.PHONY: xsh2

##XShell 2 (native, single-instance)
xsh2st: ldcst
	cd xshell2; make all
.PHONY: xsh2st

##Headless Mode Test
hdless: ldc
	cd xheadless; make all
.PHONY: hdless

clean:
	cd box; make clean
	cd libdoscard; make clean
	cd xshell; make clean
	cd xshell2; make clean
	cd xheadless; make clean
