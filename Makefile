CC = avr-gcc
OBJCOPY = avr-objcopy
CFLAGS = -Wall -mmcu=atmega32 -Os -DF_CPU=12000000UL -g
OBJS = main.o crc.o usb.o int.o
BINARY = usbtest

SUFFIXES = .S .c .o

all: ${BINARY}.hex ${BINARY}.bin

${BINARY}: ${OBJS}
	${CC} -o $@ ${CFLAGS} ${OBJS}

.c.o:
	${CC} -c -o $@ ${CFLAGS} $<

.S.o:
	${CC} -c -o $@ ${CFLAGS} $<

%.hex: %
	${OBJCOPY} -O ihex $< $@

%.bin: %
	${OBJCOPY} -O binary $< $@

flash: ${BINARY}.hex
	avrdude -c usbtiny -p atmega32 -U flash:w:${BINARY}.hex

clean:
	rm -f ${OBJS} ${BINARY} ${BINARY}.hex
