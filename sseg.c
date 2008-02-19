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

void sseg_output(void) {
	PORTC = ~sseg_buffer[0];
}
