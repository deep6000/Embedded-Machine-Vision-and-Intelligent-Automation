INCLUDE_DIRS = 
LIB_DIRS = 
CC=g++


CDEFS=
CFLAGS= -O0 -g $(INCLUDE_DIRS) $(CDEFS)
LIBS= -lrt
CPPLIBS= -L/usr/lib -lopencv_core -lopencv_flann -lopencv_video

HFILES= 
CFILES= 
CPPFILES= ped_detect.cpp

SRCS= ${HFILES} ${CFILES}
CPPOBJS= ${CPPFILES:.cpp=.o}

all:	ped_detect

run: 
	./ped_detect Input.mp4 --show --store OutputVideo.avi --log

clean:
	
	-rm -f ped_detect
	-rm -f *.o
	-rm -f *.txt
	-rm -f *.avi

distclean:
	-rm -f *.o *.d

ped_detect: ped_detect.o
	$(CC) $(LDFLAGS) $(CFLAGS) -o $@ $@.o `pkg-config --libs opencv` $(CPPLIBS)

depend:

.c.o:
	$(CC) $(CFLAGS) -c $<

.cpp.o:
	$(CC) $(CFLAGS) -c $<
