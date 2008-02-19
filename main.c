#define F_CPU 12000000UL

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include "usbtiny.h"
#include "usb.h"
#include "ops.h"

static unsigned char decto7seg[10] = {
	0x02, /* 0: 00000010 */
	0xCB, /* 1: 11001011 */
	0x18, /* 2: 00011000 */
	0x88, /* 3: 10001000 */
	0xC1, /* 4: 11000001 */
	0x84, /* 5: 10000100 */
	0x04, /* 6: 00000100 */
	0xCA, /* 7: 11001010 */
	0x00, /* 8: 00000000 */
	0x80  /* 9: 10000000 */
};

static unsigned char decto7segr[10] = {
	0x40, 0xD3, 0x18, 0x11, 0x83,
	0x21, 0x20, 0x53, 0x00, 0x01
};

static unsigned char moving_line[6] = {
	0xFE, /*0b11111110,*/
	0xEF, /*0b11101111,*/
	0x7F, /*0b01111111,*/
	0xDF, /*0b11011111,*/
	0xF7, /*0b11110111,*/
	0xFD, /*0b11111101*/
};

static unsigned char moving_line_r[6] = {
	0x7F, 0xF7, 0xFE, 0xFB, 0xEF, 0xBF
};

static char demo = 0;
static char irto7seg = 1;

static uint16_t get_analog(unsigned char port)
{
	int i;
	uint16_t result;

	ADMUX &= ~(0x1F); /* clear all other channels */
	ADMUX |= (port & 0x1F); /* set ADC channel */
	ADCSRA |= (1 << ADEN); /* enable ADC */
#if 1
	ADCSRA |= (1 << ADSC); /* start conversion */
	while(ADCSRA & (1 << ADSC)); /* wait for completion */
	result = ADCW;
#endif
	result = 0;
	for(i = 0; i < 3; i ++) {
		ADCSRA |= (1 << ADSC);
		while(ADCSRA & (1 << ADSC));
		result += ADCW;
	}
	ADCSRA &= ~(1 << ADEN);
	result /= 3;

	return result;
}

typedef struct {
	unsigned char pin;
	void (*action)(void);
} M32Key;

void pwm_0_up(void) {
	if(OCR1A < 256)
		OCR1A *= 1.5;
	PORTC = decto7seg[0];
}

void pwm_0_down(void) {
	if(OCR1A > 1)
		OCR1A /= 1.5;
}

#define N_KEYS 2
static M32Key keys[N_KEYS] = {
	{ PIN0, pwm_0_up },
	{ PIN1, pwm_0_down },
};

static void handle_keys(void)
{
	static unsigned char key_state[N_KEYS] = { 0, 0 };
	unsigned char i;

	for(i = 0; i < N_KEYS; i ++) {
		if((PORTB & (1 << keys[i].pin))) {
			if(key_state[i] == 0) {
				keys[i].action();
				key_state[i] = 1;
				irto7seg = 0;
			} else if(key_state[i] == 1) { key_state[i] = 2; }
		} else if(!PORTB & (1 << keys[i].pin)) {
			if(key_state[i] == 2) { key_state[i] = 3; }
			else if(key_state[i] == 3) { key_state[i] = 0; }
		}
	}
}

extern byte_t usb_setup(byte_t data[8])
{
	byte_t req;
	uint16_t result;

	req = data[1];
	demo = 0;

	switch(req)
	{
		case USBTINY_ECHO:
			return 8;
			break;
		case TEST_DEMO:
			demo = 1;
			irto7seg = 0;
			break;
		case TEST_IRTO7SEG:
			demo = 0;
			irto7seg = 1;
			DDRA |= (1 << PIN0);
			DDRA &= ~(1 << PIN1);
			/*PORTA |= (1 << PIN0);*/
			break;
		case GENIO_SET_PORT_A_DIR:
			if(data[2])
				/* input */
				DDRA |= (1 << data[4]);
			else
				/* output */
				DDRA &= ~(1 << data[4]);
			return 0;
		case GENIO_SET_PORT_C_DIR:
			if(data[2])
				DDRC |= (1 << data[4]);
			else
				DDRC &= ~(1 << data[4]);
			return 0;
		case GENIO_SET_PORT_A_PIN_D:
			if(data[2])
				PORTA |= (1 << data[4]);
			else
				PORTA &= ~(1 << data[4]);
			return 0;
		case GENIO_SET_PORT_C_PIN_D:
			if(data[2])
				PORTC |= (1 << data[4]);
			else
				PORTC &= ~(1 << data[4]);
			return 0;
		case GENIO_SET_PORT_A:
			PORTA = data[2];
			return 0;
		case GENIO_SET_PORT_C:
			PORTC = data[2];
			return 0;
		case GENIO_GET_PORT_A_PIN_D:
			data[0] = (PINA & (1 << data[4])) ? 1 : 0;
			return 1;
		case GENIO_GET_PORT_C_PIN_D:
			data[0] = (PINC & (1 << data[4])) ? 1 : 0;
			return 1;
		case GENIO_GET_PORT_A_PIN_A:
			result = get_analog(data[4]);
			data[0] = result & 0x0FF;
			data[1] = result >> 8;
			return 2;
		case GENIO_SET_PORT_A_7SEG:
			PORTA = decto7segr[data[2]];
			return 0;
		case GENIO_SET_PORT_C_7SEG:
			PORTC = decto7seg[data[2]];
			return 0;
		case GENIO_SET_PWM:
			if(data[4] == 0)
				OCR1A = data[2] << 2;
			else if(data[4] == 1)
				OCR1B = data[2] << 2;
			return 0;
		default:
			return 0;
	}
	return 0;
}

#if 0
extern byte_t usb_in(byte_t* data, byte_t len)
{
	return len;
}

extern void usb_out(byte_t *data, byte_t len)
{
}
#endif


ISR(TIMER0_OVF_vect)
{
	static unsigned char cnt = 0;
	uint16_t result;

#if 0
	handle_keys();
#endif

#if 0
	if(PORTB & (1 << PIN1))
		PORTC = decto7seg[1];
	else
		PORTC = decto7seg[0];
#endif

	if(irto7seg) {
		if((cnt % 10) == 0) {
			result = get_analog(1);
			PORTC = decto7seg[((result & 0x0FF) * 10 / 256) & 0x0FF];
		}
	}
	else if(demo) {
		PORTA = moving_line[cnt / 10];
		PORTC = moving_line_r[cnt / 10];
	}
	cnt ++;
	if(cnt >= 60) cnt = 0;
}

int main() {
	usb_init();

	/* disable JTAG interface */
	MCUCSR |= (1 << JTD);
	MCUCSR |= (1 << JTD);

	/* default port settings */
	DDRC = 0xFF; /* Port C -> output */
	DDRA = 0x00; /* Port A -> input */

	/* initialize timer */
	TCCR0 = (1 << CS00) | (1 << CS02);
	TIMSK |= (1 << TOIE0);

	/* initialize PWM */
	TCCR1A = (1<<WGM10)|(1<<WGM11)|(1<<COM1A1)|(1<<COM1B1);
	TCCR1B =  (1<<CS11) |(1<<CS10);

	PORTD |= (1 << PIN5) | (1 << PIN4);
	DDRD |= (1 << PIN5) | (1 << PIN4);

	OCR1A = 1 << 7;
	OCR1B = 1 << 7;

	/* initialize ADC */
	ADCSRA = (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0); /* CLK / 128 */
	ADMUX = (1 << REFS0); /* AVCC as reference */

	/* init IR to 7Seg */
	DDRA |= (1 << PIN0);
	DDRA &= ~(1 << PIN1);
	PORTA |= (1 << PIN0);

	DDRB &= ~((1 << PIN0) | (1 << PIN1)); /* PB0, PB1 -> input */
	PORTB |= (1 << PIN0) | (1 << PIN1);

	for(;;)	{
		usb_poll();
	}
	return 0;
}
