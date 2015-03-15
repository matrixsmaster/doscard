## DosCard Project LibDOSCard Chain MAKEFILE
## Copyright (C) 2013-2015 Dmitry Soloviov

all: mt
.PHONY:	all

st:
	cd src; make -f Makefile.gcc all
.PHONY: st

mt:
	cd src; make -f Makefile.llvm all
.PHONY: mt

mingw:
	cd src; make -f Makefile.mingw all
.PHONY: mingw

clean:
	cd src; make -f Makefile.gcc clean
	cd src; make -f Makefile.llvm clean
	cd src; make -f Makefile.mingw clean
