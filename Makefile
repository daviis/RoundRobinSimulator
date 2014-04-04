ARCH = $(shell uname)

ifeq ($(ARCH),Darwin)
	CPP	 = g++
	LINK = g++
else
	CPP	 = g++
	LINK     = g++
endif

CPPFLAGS = -std=c++11 -g -Wno-deprecated
LDFLAGS  =
LDSTATIC =


OBJS = program.o

all:	simulator

%.o:	%.cpp %.h
	$(CPP) $(CPPFLAGS) -c $<

simulator:	$(OBJS) simulator.cpp
	$(LINK) $(CPPFLAGS) simulator.cpp $(LDFLAGS) $(OBJS) -o simulator	

clean:
	rm *.o simulator 
