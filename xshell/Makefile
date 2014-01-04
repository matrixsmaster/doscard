APP := xshell
INCLUDES := -I.. -I.
CXXFLAGS := -g -O2 -Wall $(INCLUDES)
LD_STAT := -lSDL2
LD_BDYN := -lX11 -lpthread -lrt
LD_LIBS := -L../SDL2/build/.libs -Wl,-static $(LD_STAT) -Wl,-Bdynamic $(LD_BDYN)

all:	$(APP).o
	g++ -g -o $(APP) $(APP).o $(LD_LIBS)
.PHONY:	all

%.o:	%.cpp
	g++ $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(APP) *.o