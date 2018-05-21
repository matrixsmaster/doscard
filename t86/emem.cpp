// (C) Dmitry 'MatrixS_Master' Soloviov, 2015-2016

#include "emem.h"
#include <stdio.h>

static uch memory[1200000];

uch mem_rd(const int a)
{
	printf("RAM: RD @ %i\n",a);
	return memory[a];
}

void mem_wr(const int a, const uch x)
{
	printf("RAM: WR @ %i <- %hhu\n",a,x);
	memory[a] = x;
}
