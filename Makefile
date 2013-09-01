CFLAGS=-g

PROG=zatsudan
SRC=zatsudan.cc client.cc conninfo.cc socket.cc switchboard.cc
OBJ=$(SRC:.cc=.o)

.cc.o:
	g++ -c $(CFLAGS) $<

all: .depend $(PROG)

$(PROG): $(OBJ) 
	g++ -o $(PROG) $(CFLAGS) $(OBJ)

include .depend

clean:
	rm -rf *.o

.depend: Makefile
	g++ -MM $(SRC) >.depend


