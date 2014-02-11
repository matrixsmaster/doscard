## DosCard Project Top Level MAKEFILE
## Copyright (C) 2013-2014 Soloviov Dmitry

all: xshell
.PHONY: all

ldb:
	cd box; make all
.PHONY: ldb

xshell: ldb
	cd xshell; make all
.PHONY: xshell

clean:
	cd box; make clean
	cd xshell; make clean
