all:
	gcc -g -O0 -finstrument-functions -c test.c -o test.o
	g++ -g -O2 -c ins.cc -o ins.o
	g++ -g test.o ins.o -o test.elf -lpthread

clean:
	rm -f *.o *.elf
