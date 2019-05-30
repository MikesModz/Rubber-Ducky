#ifndef _MY_PAYLOADS_H_
#define _MY_PAYLOADS_H_

#define MAX_NUMBER_PAYLOADS		16

/* Starts, waits for 5 seconds before locking the PC. Repeats once. */
const uint8_t keyboard_payload_1[] PROGMEM =
{
	1,
	2,
	CMD_DELAYS,	5,
	CMD_GUI,	'l',
};

/* Starts, waits for up to 10 minutes before locking the PC. Repeats 10 times. */
const uint8_t keyboard_payload_2[] PROGMEM =
{
	10,
	2,
	CMD_DELAYR,	10,
	CMD_GUI,	'l',
};

/* Empty payload. Does nothing. */
const uint8_t keyboard_payload_3[] PROGMEM =
{
	0
};

/* Empty payload. Does nothing. */
const uint8_t keyboard_payload_4[] PROGMEM =
{
	0
};

/* Empty payload. Does nothing. */
const uint8_t keyboard_payload_5[] PROGMEM =
{
	0
};

/* Empty payload. Does nothing. */
const uint8_t keyboard_payload_6[] PROGMEM =
{
	0
};

/* Empty payload. Does nothing. */
const uint8_t keyboard_payload_7[] PROGMEM =
{
	0
};

/* Empty payload. Does nothing. */
const uint8_t keyboard_payload_8[] PROGMEM =
{
	0
};

/* Empty payload. Does nothing. */
const uint8_t keyboard_payload_9[] PROGMEM =
{
	0
};

/* Empty payload. Does nothing. */
const uint8_t keyboard_payload_10[] PROGMEM =
{
	0
};

/* Empty payload. Does nothing. */
const uint8_t keyboard_payload_11[] PROGMEM =
{
	0
};

/* Empty payload. Does nothing. */
const uint8_t keyboard_payload_12[] PROGMEM =
{
	0
};

/* Empty payload. Does nothing. */
const uint8_t keyboard_payload_13[] PROGMEM =
{
	0
};

/* Empty payload. Does nothing. */
const uint8_t keyboard_payload_14[] PROGMEM =
{
	0
};

/* Empty payload. Does nothing. */
const uint8_t keyboard_payload_15[] PROGMEM =
{
	0
};

/* Empty payload. Does nothing. */
const uint8_t keyboard_payload_16[] PROGMEM =
{
	0
};

#endif
