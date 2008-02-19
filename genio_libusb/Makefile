CC = gcc
CFLAGS = -Wall -ansi -pedantic
LIBS = -lusb
OBJS = main.o
BINARY = avrusb

all: ${BINARY}

${BINARY}: ${OBJS}
	${CC} -o $@ ${OBJS} ${LIBS}

.c.o:
	${CC} -c ${CFLAGS} -o $@ $<

clean:
	rm -f ${OBJS} ${BINARY}
