// (C) Dmitry 'MatrixS_Master' Soloviov, 2015-2016

#ifndef MRAM_H_
#define MRAM_H_

#include "VM86conf.h"
#include "emem.h"

/* Testing managed RAM unit (quick and dirty) */

template <class T> class RAM
{
private:
	int seg;
	int last;
	T scr,old,tmp;
	bool wr;

	//FIXME: byte/word only!
	T inline _rd(const int a) {
		if (sizeof(T) == 2) {
			tmp = (T)(mem_rd(seg+a*2+1)) << 8;
			tmp |= (T)(mem_rd(seg+a*2));
		} else {
			tmp = mem_rd(seg+a);
		}
		return tmp;
	}

	void inline _wr(const int a, const T x) {
		if (sizeof(T) == 2) {
			mem_wr(seg+a*2,(x&255));
			mem_wr(seg+a*2+1,(x >> 8));
		} else {
			mem_wr(seg+a,x);
		}
	}

public:
	RAM(const int s = 0) {
		seg = s;
		last = 0;
		scr = 0;
		old = 0;
		tmp = 0;
		wr = false;
	}

	void Segment(int s) { seg = s; }
	int Segment(void) { return seg; }

	T & operator [] (const int a) {
		if (old != scr) _wr(last,scr);
		last = a;
		old = scr = _rd(a);
		return scr;
	}
};

template <class T> class RAMptr
{
private:
	RAM<T> * ram;
	int off;

public:
	RAMptr(RAM<T> * p = NULL, int o = 0) { ram = p; off = o; }

	void Ram(RAM<T> * r) { ram = r; }
	RAM<T> * Ram(void) { return ram; }

	void Reset(void) { ram = NULL; off = 0; }

	RAMptr operator + (const int s) { return RAMptr(ram,off+s); }
	RAMptr operator - (const int s) { return RAMptr(ram,off-s); }
	RAMptr & operator += (const int s) { off += s; return *this; }
	RAMptr & operator -= (const int s) { off -= s; return *this; }
	RAMptr & operator = (int a) { off = a; return *this; }
	RAMptr & operator = (const RAMptr & x) { ram = x.ram; off = x.off; return *this; }

	T & operator [] (const int s) { return (*ram)[off+s]; }
	T & operator * (void) { return (*ram)[off]; }
};

#endif /* MRAM_H_ */
