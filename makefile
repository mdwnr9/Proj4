CC = gcc
CFLAGS = -g -Wall -I.

oss : oss.c user
	$(CC) -std=c99 -o oss oss.c

user :	user.c 
	$(CC) -std=c99 -o user user.c
EXECS = oss user
all: $(EXECS)

clean:
	rm -f *.o $(EXECS)
