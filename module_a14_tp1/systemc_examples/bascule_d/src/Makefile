CC      = g++
CXX     = g++
RM      = rm

BINS  	= ../bin
SYSTEMC =  /usr/ensta/pack/virtualsoc/systemc-2.2.0

CFLAGS 	= -O3 -pipe -Wno-deprecated -I$(SYSTEMC)/include
LDFLAGS = -L$(SYSTEMC)/lib-linux64 -lsystemc

SRCS	= main.cpp stimulus.cpp display.cpp
OBJS	= $(SRCS:.cpp=.o)
PRG	= ${BINS}/example

all : ${OBJS}
	${CC} ${OBJS} -o ${PRG} ${LDFLAGS}

.cpp.o:
	$(CC) $(CFLAGS) -c $<

clean:
	$(RM) -f $(BINS)/example ../wave/waveform.vcd ${OBJS}


