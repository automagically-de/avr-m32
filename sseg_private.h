#ifndef _SSEG_PRIVATE_H
#define _SSEG_PRIVATE_H

/*
 *   =A=
 * B|   |C
 *   =D=
 * E|   |F
 *   =G=  dp
 */

static unsigned char sseg_map[8] = {
  0, /* 0x01 dp */
  1, /* 0x02 A */
  2, /* 0x04 B */
  3, /* 0x08 C */
  4, /* 0x10 D */
  5, /* 0x20 E */
  6, /* 0x40 F */
  7  /* 0x80 G */ };

static unsigned char sseg_charset[128] = {
	/* 0x00..0x0F */ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	/* 0x10..0x1F */ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	/* 0x20:   */ 0x00,

	/* 0x27: ( */ 0xA6,

	/* 0x29: ) */ 0xAC,
	/* 0x2D: - */ 0x10,

	/* 0x30: 0 */ 0xEE,
	/* 0x31: 1 */ 0x48,
	/* 0x32: 2 */ 0xBA,
	/* 0x33: 3 */ 0xDA,

	/* 0x35: 5 */ 0xD6,

	/* 0x38: 8 */ 0xFE,
	/* 0x39: 9 */ 0xDE,

	/* 0x41: A */ 0x7E,
	/* 0x42: B */ 0xFE,
	/* 0x49: I */ 0x48,

	/* 0x53: S */ 0xD6,

	/* 0x5A: Z */ 0xBA,
	/* 0x5B: [ */ 0xA6,
	/* 0x5D: ] */ 0xAC,
	/* 0x5F: _ */ 0x80,

	/* 0x61: a */ 0xBE,
	/* 0x69: i */ 0x48,

	/* 0x73: s */ 0xD6,

	/* 0x7A: z */ 0xBA,
	/* 0x7B: { */ 0xA6,
	/* 0x7C: | */ 0x48,
	/* 0x7D: } */ 0xAC,

};

#endif /* _SSEG_PRIVATE_H */
