CXX = `wx-config --cxx`
CXXFLAGS = `wx-config --cxxflags` -mno-cygwin
LIBS = `wx-config --libs` -mno-cygwin
EXTRALIBS = /usr/local/lib/libwxjpeg-2.6.a /usr/local/lib/libwxtiff-2.6.a /usr/local/lib/libwxpng-2.6.a /usr/local/lib/libwxzlib-2.6.a

all: ss_gui

ss_gui: ss_gui.o
	${CXX} -o ss_gui $< ${LIBS} ${EXTRALIBS}

ss_gui.o: ss_gui.cpp
	${CXX} -c -o $@ $< ${CXXFLAGS}

clean:
	rm -f *.exe *.o
