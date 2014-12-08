## DosCard Project Top Level MAKEFILE
## Copyright (C) 2013-2014 Dmitry Soloviov

all:
	echo -e "Targets available:\n \
	ldb - libDosBox\n \
	ldc - libDosCard\n \
	xsh1 - XShell 1\n \
	xsh2 - XShell 2"
.PHONY: all

##libdosbox (native)
ldb:
	cd box; make all
.PHONY: ldb

##libdoscard (native + llvm)
ldc:
	cd box; make clean
	cd libdoscard; make all
.PHONY: ldc

##XShell 1 (native, single-instance)
xsh1: ldb
	cd xshell; make all
.PHONY: xsh1

##XShell 2 (native, multi-instance)
xsh2: ldc
	cd xshell2; make all
.PHONY: xsh2

##Headless Mode Test
headless: ldc
	cd xheadless; make all
.PHONY: headless

clean:
	cd box; make clean
	cd libdoscard; make clean
	cd xshell; make clean
	cd xshell2; make clean
	cd xheadless; make clean
