#include <avr/io.h>

#include "sseg.h"
#include "sseg_private.h"

#define BIT_SCLR 4
#define BIT_SCK  3
#define BIT_RCK  2
#define BIT_SI   1

char sseg_buffer[9] = "--------";

void sseg_init(void) {
	unsigned char i;
	DDRC = 0xFF;
	PORTC = (1 << BIT_SCLR) | (1 << 5) | (1 << 6) | (1 << 7);

	for(i = 0; i < 8; i ++)
		sseg_set_char(i, i);
}

void sseg_set_char(unsigned char n, char c)
{
	unsigned char mask, bit, out, code;

	code = sseg_charset[(unsigned char)(c & 0x7F)];

	/* map */
	out = 0;
	for(bit = 0; bit < 8; bit ++) {
		mask = (1 << bit);
		if(code & mask)
			out |= (1 << bit /*sseg_map[bit]*/);
	}
	sseg_buffer[n] = out;
}

void sseg_output(unsigned char n) {
	unsigned char b;

#if 0
	/* clear '595 */
	PORTC &= ~(1 << BIT_SCLR);
	PORTC |= (1 << BIT_SCLR);
#endif

	/* serialize sseg data */
	for(b = 0; b < 8; b ++) {
		if(sseg_buffer[7 - n] & (1 << b))
			PORTC &= ~(1 << BIT_SI);
		else
			PORTC |= (1 << BIT_SI);

		/* shift */
		PORTC |= (1 << BIT_SCK);
		PORTC &= ~(1 << BIT_SCK);
	}

#if 1
	/* set address of 7seg to '238 */
	PORTC &= 0x1F; /* upper 3 bits of port C are address */
	PORTC |= (n << 5);
#endif

#if 1
	/* data -> storage register */
	PORTC |= (1 << BIT_RCK);
	PORTC &= ~(1 << BIT_RCK);
#endif
}
