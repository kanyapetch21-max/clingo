CC=gcc
CFLAGS=-std=c99 -Wall

OBJS=main.o lesson.o fileio.o user.o ui.o util.o

clingo: $(OBJS)
	$(CC) $(CFLAGS) -o clingo $(OBJS)

%.o: %.c
	$(CC) $(CFLAGS) -c $<

clean:
	del /Q *.o clingo.exe 2>NUL || rm -f *.o clingo
