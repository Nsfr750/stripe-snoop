CXX=c++
#CXXFLAGS = -O -Wall
CXXFLAGS = -O
CC=cc



SSOBJECTS=main.o ssflags.o reader.o sxmlp.o loader.o card.o track.o bitstream.o misc.o testfuncs.o testresult.o database.o cardtest.o 
RDOBJECTS=rdetect.o ssflags.o reader.o sxmlp.o loader.o card.o track.o bitstream.o misc.o testfuncs.o

OBJECTS=$(SSOBJECTS) $(RDOBJECTS)

APPLICATIONS=ss bitgen mod10 rdetect

all: ss bitgen mod10 rdetect

ss: $(SSOBJECTS)
	@echo Linking ss
	$(CXX) $(CXXFLAGS) $(SSOBJECTS) -o ss


%.o: %.cpp %.h
	@echo Compling $*
	@rm -f $*.o
	$(CXX) $(CXXFLAGS) -c $*.cpp

bitgen: bitgen.c
	@echo Building bitgen
	$(CC) $(CXXFLAGS) -o bitgen bitgen.c

mod10: mod10.c
	@echo Building mod10
	$(CC) $(CXXFLAGS) -o mod10 mod10.c

rdetect: $(RDOBJECTS)
	@echo Linking rdetect
	$(CXX) $(CXXFLAGS) $(RDOBJECTS) -o rdetect

ports: ports.cpp
	$(CXX) $(CXXFLAGS) ports.cpp -o ports

clean:
	@rm -f $(OBJECTS) $(APPLICATIONS)
	@echo All Clean!

