APP := xshell
INCLUDES := -I. -I.. -I../box/include
CXXFLAGS := -g -O2 -Wall $(INCLUDES)
LD_STAT := -ldosbox -lSDL2
LD_BDYN := -lX11 -lpthread -lrt
LD_LIBS := -L../SDL2/build/.libs -L../box/src -Wl,-static $(LD_STAT) -Wl,-Bdynamic $(LD_BDYN)
SUPPORTS := fake_threading.o

all:	$(SUPPORTS) $(APP).o
	g++ -g -o $(APP) $(SUPPORTS) $(APP).o $(LD_LIBS)
.PHONY:	all

%.o:	%.cpp
	g++ $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(APP) *.o