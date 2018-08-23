WL_PROGRAM=wl

CC= gcc -std=gnu99
CFLAGS= -O2 -Wall
LDFLAGS= 
LIBS= 

all: $(WL_PROGRAM)

.SUFFIXES: .c .o
.o.c:
	$(CC) $(CFLAGS) -c $<

clean:
	$(RM) *.o $(WL_PROGRAM)
