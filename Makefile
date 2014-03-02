## DosCard Project Top Level MAKEFILE
## Copyright (C) 2013-2014 Soloviov Dmitry

all: xsh1
.PHONY: all

ldb:
	cd box; make all
.PHONY: ldb

xsh1: ldb
	cd xshell; make all
.PHONY: xshell

clean:
	cd box; make clean
	cd libdoscard; make clean
	cd xshell; make clean
	cd xshell2; make clean
