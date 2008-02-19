#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>

#include <usb.h>

#include "../ops.h"

typedef struct {
	unsigned char cmdid;
	const char *command;
	unsigned char type;
	unsigned char rbytes;
} GenIO_Command;

enum {
	TYPE_NULL,
	TYPE_BOOL,
	TYPE_UINT8,
	TYPE_DEC1,
	TYPE_FLOAT8,
	TYPE_CHAR
};

static GenIO_Command commands[] = {
	{ GENIO_SET_PORT_A_DIR,   "set_a_dir",   TYPE_BOOL, 0 },
	{ GENIO_SET_PORT_C_DIR,   "set_c_dir",   TYPE_BOOL, 0 },
	{ GENIO_SET_PORT_A,       "set_a",       TYPE_UINT8, 0 },
	{ GENIO_SET_PORT_C,       "set_c",       TYPE_UINT8, 0 },
	{ GENIO_SET_PORT_A_PIN_D, "set_a_pin",   TYPE_BOOL, 0 },
	{ GENIO_SET_PORT_C_PIN_D, "set_c_pin",   TYPE_BOOL, 0 },
	{ GENIO_GET_PORT_A_PIN_D, "get_a_pin",   TYPE_NULL, 1 },
	{ GENIO_GET_PORT_C_PIN_D, "get_c_pin",   TYPE_NULL, 1 },
	{ GENIO_GET_PORT_A_PIN_A, "get_a_pin_a", TYPE_NULL, 2 },
	{ GENIO_SET_PORT_A_7SEG,  "set_a_7seg",  TYPE_CHAR, 0 },
	{ GENIO_SET_PORT_C_7SEG,  "set_c_7seg",  TYPE_CHAR, 0 },
	{ GENIO_SET_PWM,          "set_pwm",     TYPE_FLOAT8, 0 },

	{ TEST_DEMO,              "demo",        TYPE_NULL, 0 },
	{ TEST_IRTO7SEG,          "irto7seg",    TYPE_NULL, 0 }
};

static unsigned int n_commands = sizeof(commands) / sizeof(GenIO_Command);

int genio_exec(usb_dev_handle *udh, const char *tcmd,
	const char *tpin, const char *tval)
{
	GenIO_Command *command = NULL;
	int i, r, val;
	unsigned char *bytes = NULL;

	for(i = 0; i < n_commands; i ++)
		if(strcmp(commands[i].command, tcmd) == 0) {
			command = &(commands[i]);
			break;
		}

	if(command == NULL)
		return EXIT_FAILURE;

	switch(command->type)
	{
		case TYPE_BOOL:
			val = (atoi(tval) ? 1 : 0);
			break;
		case TYPE_UINT8:
			val = strtol(tval, NULL, 0) & 0x0FF;
			break;
		case TYPE_DEC1:
			val = strtol(tval, NULL, 0) % 10;
			break;
		case TYPE_FLOAT8:
			val = (int)(atof(tval) * 255);
			break;
		case TYPE_CHAR:
			val = tval[0];
			break;
		default:
			val = 0;
			break;
	}
	fprintf(stderr,
		"DEBUG: usb_control_msg: cmd=0x%02X, value=0x%02X, index=0x%02X\n",
		command->cmdid, val, atoi(tpin));

	if(command->rbytes > 0)
	{
		bytes = (unsigned char *)calloc(command->rbytes, sizeof(char));
		r = usb_control_msg(udh,
			USB_TYPE_VENDOR | USB_RECIP_DEVICE | USB_ENDPOINT_IN,
			command->cmdid, val, atoi(tpin),
			(char *)bytes, command->rbytes, 500);
		printf("returned bytes: ");
		for(i = 0; i < command->rbytes; i ++)
			printf("0x%02X ", bytes[i] & 0x0FF);
		printf("\n");
	}
	else
	{
		r = usb_control_msg(udh,
			USB_TYPE_VENDOR | USB_RECIP_DEVICE | USB_ENDPOINT_IN,
			command->cmdid, val, atoi(tpin),
			NULL, 0, 500);
	}
	if(r < 0)
		fprintf(stderr, "Error: %s\n", usb_strerror());

	if(bytes)
		free(bytes);

	return EXIT_SUCCESS;
}

static struct usb_device *get_device(void)
{
	struct usb_bus *usbbus;
	struct usb_device *usbdev;

	usb_find_busses();
	usb_find_devices();

	usbbus = usb_get_busses();
	while(usbbus != NULL) {
		usbdev = usbbus->devices;
		while(usbdev != NULL) {
			if(usbdev->descriptor.idVendor == 0x6666)
				return usbdev;
			usbdev = usbdev->next;
		}
		usbbus = usbbus->next;
	}
	return NULL;
}

int main(int argc, char *argv[])
{
	struct usb_device *usbdev;
	usb_dev_handle *udh;

	setlocale(LC_NUMERIC, "C");

	usb_init();
	usb_set_debug(0);

	usbdev = get_device();
	if(usbdev != NULL) {
		udh = usb_open(usbdev);
		if(udh != NULL) {
			if(argc > 3)
				genio_exec(udh, argv[1], argv[2], argv[3]);
			else
				fprintf(stderr, "usage: %s <command> <pin> <value>\n",
					argv[0]);
			usb_close(udh);
		}
		else {
			fprintf(stderr, "Error: %s\n", usb_strerror());
		}
	}
	else {
		fprintf(stderr, "ERROR: no AVR device found\n");
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}
