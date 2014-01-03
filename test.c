#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

int a(int x);
void b(int x);
void c();
void* d(void* x);

int a(int x)
{
	int i;
	printf("a(%d)\n",x);
	for (i=0; i<x; i++) b(i);
	return 0;
}

void b(int x)
{
	printf("b(%d)\n",x);
	c();
}

void c()
{
	int i,r;
	pthread_t ar[3];
	printf("c() creating...\n");
	for (i=0; i<3; i++) {
		r = pthread_create(&ar[i],NULL,d,(void*)i);
		if (r) {
			printf("pthread_create()=%d\n",r);
			abort();
		}
	}
	for (i=0; i<3; i++) pthread_join(ar[i],NULL);
	printf("c(): joined!\n");
}

void* d(void* x)
{
	printf("d(%p)\n",x);
	//pthread_exit(NULL);
}

int main()
{
	a(3);
	printf("main(): done\n");
	return 0;
}
