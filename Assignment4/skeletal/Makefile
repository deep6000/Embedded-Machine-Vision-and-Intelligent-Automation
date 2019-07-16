INCLUDE_DIRS = 
LIB_DIRS = 
CC=g++

CDEFS=
CFLAGS= -O0 -g $(INCLUDE_DIRS) $(CDEFS)
LIBS= -lrt
CPPLIBS= -L/usr/lib -lopencv_core -lopencv_flann -lopencv_video

HFILES= 
CFILES= 
CPPFILES= skeletal.cpp

SRCS= ${HFILES} ${CFILES}
CPPOBJS= ${CPPFILES:.cpp=.o}

all:	skeletal 

clean:
	-rm -f *.o *.d
	-rm -f skeletal

distclean:
	-rm -f *.o *.d

skeletal: skeletal.o
	$(CC) $(LDFLAGS) $(CFLAGS) -o $@ $@.o `pkg-config --libs opencv` $(CPPLIBS)

depend:

.c.o:
	$(CC) $(CFLAGS) -c $<

.cpp.o:
	$(CC) $(CFLAGS) -c $<

