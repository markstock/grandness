CC=gcc
CXX=g++
#DEBUG=-g -ggdb -O0
#DEBUG=-Ofast
DEBUG=-O3 -ffast-math -fopenmp
CFLAGS=-std=c99
CXXFLAGS=-std=c++11
INC=-I/usr/include/eigen3
OBJS=inout.o grandness.o
EXE=grandness

all : $(EXE)

%.o : %.cpp
	${CXX} ${CXXFLAGS} ${DEBUG} ${INC} -c $<

%.o : %.c
	${CC} $(CFLAGS) ${DEBUG} -c $<

$(EXE) : grandness.cpp $(OBJS)
	${CXX} $(CXXFLAGS) ${DEBUG} -o $@ $(OBJS) $(LDFLAGS) -lm -lpng

clean :
	rm -f *.o $(EXE)
