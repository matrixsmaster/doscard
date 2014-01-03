all:
	gcc -g -O2 -finstrument-functions -c test.c -o test.o
	g++ -g -O2 -c ins.cc -o ins.o
	g++ -g -O2 test.o ins.o -o test.elf -lpthread

clean:
	rm -f *.o *.elf
