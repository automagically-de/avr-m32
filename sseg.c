#include <avr/io.h>

#include "sseg.h"
#include "sseg_private.h"

char sseg_buffer[9] = "--------";

void sseg_set_char(unsigned char n, char c)
{
	unsigned char mask, bit, out, code;

	code = sseg_charset[(unsigned char)(c & 0x7F)];

	/* map */
	out = 0;
	for(bit = 0; bit < 8; bit ++) {
		mask = (1 << bit);
		if(code & mask)
			out |= (1 << sseg_mapp[bit]);
	}
	sseg_buffer[n] = out;
}

#define BIT_SCLR 4
#define BIT_SCK  3
#define BIT_RCK  2
#define BIT_SI   1

void sseg_output(void) {
	unsigned char c, b;

	for(c = 0; c < 8; c ++) {
		/* clear '595 */
		PORTC &= ~(1 << BIT_SCLR);
		PORTC |= (1 << BIT_SCLR);

		/* serialize sseg data */
		for(b = 0; b < 8; b ++) {
			if(sseg_buffer[c] & (1 << b))
				PORTC |= (1 << BIT_SI);
			else
				PORTC &= ~(1 << BIT_SI);
			PORTC |= (1 << BIT_SCK);
			PORTC &= ~(1 << BIT_SCK);
		}

		/* set address of 7seg to '238 */
		PORTC &= ~(0x07 << 5); /* upper 3 bits of port C are address */
		PORTC |= (c << 5);

		/* data -> storage register */
		PORTC |= (1 << BIT_RCK);
		PORTC &= ~(1 << BIT_RCK);
	}
}
