APP = doscard

ARMCPU = -mcpu=cortex-m3 -mthumb
ARMCXX = arm-linux-gnueabi-g++
ARMCXXFLAGS = -g -Os -Wall $(ARMCPU)

all:	$(APP)
.PHONY:	all

$(APP):	main.o
	$(ARMCXX) $(ARMCXXFLAGS) -o $(APP) main.o

%.o:	%.cpp
	$(ARMCXX) $(ARMCXXFLAGS) -c -o $@ $<

clean:
	rm -f *.o
	rm -f $(APP)
