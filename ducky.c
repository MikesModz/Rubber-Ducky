//*****************************************************************************
// Title		: USB Rubber Ducky
// Author		: Michael Williamson <mikesmodz@gmail.com>
// Created		: 28/05/2019
// Revised		: 28/05/2019
// Version		: 0.1
// License		: GNU General Public License
//
// DESCRIPTION:
// 	When run the program emulates a USB keyboard that is capable of sending
//  keystrokes to the computer. Can be used to deploy various payloads in order
//  to carry out automated tasks. In this case its used a keystroke injection
//  attack tool for social engineering purposes.
//
// NOTES:
//
// LICENSE:
//  Permission is hereby granted, free of charge, to any person obtaining a copy
//  of this software and associated documentation files (the "Software"), to deal
//  in the Software without restriction, including without limitation the rights
//  to use, copy, modify, merge, publish, distribute, sub license, and/or sell
//  copies of the Software, and to permit persons to whom the Software is
//  furnished to do so, subject to the following conditions:
//
//  The above copyright notice and this permission notice shall be included in
//  all copies or substantial portions of the Software.
//
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
//  THE SOFTWARE.
//
// CHANGELOG:
//  Date			Description
// =========================================================================
//  28/05/19		Initial version.
//**************************************************************************

/****************************************************************************
* Included Files
****************************************************************************/
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>
#include <avr/eeprom.h>
#include <util/delay.h>
#include <stdio.h>
#include <stdlib.h>
#include "ducky.h"
#include "usb_keyboard.h"
#include "my_payloads.h"

//*****************************************************************************
// Private Data
//*****************************************************************************
const uint8_t *payloads[MAX_NUMBER_PAYLOADS] =
{
	keyboard_payload_1,
	keyboard_payload_2,
	keyboard_payload_3,
	keyboard_payload_4,
	keyboard_payload_5,
	keyboard_payload_6,
	keyboard_payload_7,
	keyboard_payload_8,
	keyboard_payload_9,
	keyboard_payload_10,
	keyboard_payload_11,
	keyboard_payload_12,
	keyboard_payload_13,
	keyboard_payload_14,
	keyboard_payload_15,
	keyboard_payload_16
};

/* Payload count saved in EEPROM */
uint8_t EEMEM nvCount = 0;
//*****************************************************************************
// Private Functions
//*****************************************************************************
static void send_payload(const uint8_t *payload);
static void delay_secs(uint8_t secs);
static void delay_msecs(uint8_t msec);
static void delay_mins(uint8_t mins);

//*****************************************************************************
// Public Functions
//*****************************************************************************
int main(void)
{
	uint8_t count = 0;
	uint8_t i = 0;
	
	/* Set clock to max speed */
	CPU_PRESCALE(CPU_16MHz);

	/* LED pins output */
	LED1_DIR |= (1<<LED1_PIN);
	LED2_DIR |= (1<<LED2_PIN);
	
	/* HWB pin input */
	HWB_DIR &= ~(1<<HWB_PIN);
	
	/* Turn both LEDs OFF */
	LED1_PORT &= ~(1<<LED1_PIN);
	LED2_PORT &= ~(1<<LED2_PIN);
	
	/* Read count value from EEPROM */
	count = eeprom_read_byte(&nvCount);
	
	/* Startup delay */
	_delay_ms(500);
	
	/* If button pressed during power up enter settings */
	if ( !(HWB_PINS & (1<<HWB_PIN)) )
	{
		/* Reset count */
		count = 0;
		
		/* Save count to EEPROM */
		eeprom_write_byte(&nvCount, count);
		
		/* Flash LED a few times to indicate in settings */
		for(i=0; i<2; i++)
		{
			LED1_PORT |= (1<<LED1_PIN);
			_delay_ms(100);
			
			LED1_PORT &= ~(1<<LED1_PIN);
			_delay_ms(100);
		}
		
		_delay_ms(1000);
		
		while(1)
		{
			/* Button pressed ? */
			if ( !(HWB_PINS & (1<<HWB_PIN)) )
			{
				/* Yes wait for release */
				while(!(HWB_PINS & (1<<HWB_PIN)));
				
				/* If payload count not exceeded */
				if (count < (MAX_NUMBER_PAYLOADS-1))
				{
					/* Increment payload count */
					count++;
					
					/* Save count to EEPROM */				
					eeprom_write_byte(&nvCount, count);
					
					/* Flash LED to indicate payload count increased */
					LED1_PORT |= (1<<LED1_PIN);
					_delay_ms(100);
					
					LED1_PORT &= ~(1<<LED1_PIN);
					_delay_ms(100);
				}
				else
				{
					/* Payload count maxed out so stop now */
					LED1_PORT |= (1<<LED1_PIN);
					while (1);
				}
			}
		}
	}

	/* Initialize the USB, and then wait for the host to set configuration */
	usb_init();
	
	/* Wait */
	while (!usb_configured());
	
	/* Turn LED on during payloads execution */
	LED1_PORT |= (1<<LED1_PIN);

	/* Wait an extra second for the PC's operating system to load drivers
	   and do whatever it does to actually be ready for input */
	_delay_ms(1000);
	
	/* Enable global interrupts */
	sei();
	
	/* Send the payload */
	send_payload(payloads[count]);
	
	/* Payload complete. Keep flashing LED */
	while (1)
	{	
		LED1_PORT |= (1<<LED1_PIN);
		_delay_ms(100);
		
		LED1_PORT &= ~(1<<LED1_PIN);
		_delay_ms(100);
	}
}

//*****************************************************************************
// Private Functions
//*****************************************************************************

//*****************************************************************************
// Function:     send_payload
// Called with:  *payload = Pointer to payload to send.
// Returns:      Nothing.
// Purpose:      Sends a payload via the USB keyboard.
//*****************************************************************************
static void send_payload(const uint8_t *payload)
{
	uint8_t cmds = 0;
	uint8_t key = 0;
	uint8_t cmd = 0;
	uint8_t rep = 0;
	uint8_t len = 0;
	uint8_t i = 0;
	uint8_t loop = 0;
	uint8_t msecs = 0;
	uint8_t secs = 0;
	uint8_t mins = 0;
	uint8_t max = 0;
	uint8_t *start = NULL;

	/* Check payload is valid */
	if (payload == NULL)
	{
		return;
	}

	/* Get repeat */
	rep = pgm_read_byte(payload);

	/* Repeat a number of times */
	for (loop=0; loop<rep; loop++)
	{
		/* Get pointer to start of interesting stuff */
		start = (uint8_t*)payload+1;

		/* Get number of commands in payload */
		cmds = pgm_read_byte(start++);

		/* Process each command */
		while(cmds--)
		{
			/* Get command */
			cmd = pgm_read_byte(start++);

			/* Process command */
			switch(cmd)
			{
				/* Delay for a number of milliseconds */
				case CMD_DELAY:
					msecs = pgm_read_byte(start++);
					delay_msecs(msecs);
				break;

				/* Delay for a number of seconds */
				case CMD_DELAYS:
					secs = pgm_read_byte(start++);
					delay_secs(secs);
				break;

				/* Delay for a random number of minutes */
				case CMD_DELAYR:
					max = pgm_read_byte(start++);
					mins = rand() % max;
					delay_mins(mins);
				break;

				/* Press the CTRL key */
				case CMD_CTRL:
					key = pgm_read_byte(start++);
					usb_keyboard_press(KEY_LEFT_CTRL);
					if (key != KEY_NONE)
					{
						usb_keyboard_press(key);
					}
					_delay_ms(KEY_PRESS_TIME);
					usb_keyboard_release_all();
				break;

				/* Press the ALT key */
				case CMD_ALT:
					key = pgm_read_byte(start++);
					usb_keyboard_press(KEY_LEFT_ALT);
					if (key != KEY_NONE)
					{
						usb_keyboard_press(key);
					}
					_delay_ms(KEY_PRESS_TIME);
					usb_keyboard_release_all();
				break;

				/* Press the SHIFT key */
				case CMD_SHIFT:
					key = pgm_read_byte(start++);
					usb_keyboard_press(KEY_LEFT_SHIFT);
					if (key != KEY_NONE)
					{
						usb_keyboard_press(key);
					}
					_delay_ms(KEY_PRESS_TIME);
					usb_keyboard_release_all();
				break;

				/* Press the WINDOWS key */
				case CMD_GUI:
					key = pgm_read_byte(start++);
					usb_keyboard_press(KEY_LEFT_GUI);
					if (key != KEY_NONE)
					{
						usb_keyboard_press(key);
					}
					_delay_ms(KEY_PRESS_TIME);
					usb_keyboard_release_all();
				break;

				/* Send a string of character keys */
				case CMD_STRING:
					len = pgm_read_byte(start++);
					for (i=0; i<len; i++)
					{
						key = pgm_read_byte(start++);
						usb_keyboard_write(key);
					}
				break;

				/* Press the SHIFT+F10 keys */
				case CMD_MENU:
					usb_keyboard_press(KEY_LEFT_SHIFT);
					usb_keyboard_press(KEY_F10);
					_delay_ms(KEY_PRESS_TIME);
					usb_keyboard_release_all();
				break;

				/* Press the ENTER key */
				case CMD_ENTER:
					usb_keyboard_press(KEY_RETURN);
					_delay_ms(KEY_PRESS_TIME);
					usb_keyboard_release_all();		
				break;

				/* Press the escape key */
				case CMD_ESCAPE:
					usb_keyboard_press(KEY_ESC);
					_delay_ms(KEY_PRESS_TIME);
					usb_keyboard_release_all();		
				break;

				/* Press the caps lock key */
				case CMD_CAPSLOCK:
					usb_keyboard_press(KEY_CAPS_LOCK);
					_delay_ms(KEY_PRESS_TIME);
					usb_keyboard_release_all();		
				break;

				/* Press the up arrow key */
				case CMD_UPARROW:
					usb_keyboard_press(KEY_UP_ARROW);
					_delay_ms(KEY_PRESS_TIME);
					usb_keyboard_release_all();		
				break;

				/* Press the down arrow key */
				case CMD_DOWNARROW:
					usb_keyboard_press(KEY_DOWN_ARROW);
					_delay_ms(KEY_PRESS_TIME);
					usb_keyboard_release_all();		
				break;

				/* Press the left arrow key */
				case CMD_LEFTARROW:
					usb_keyboard_press(KEY_LEFT_ARROW);
					_delay_ms(KEY_PRESS_TIME);
					usb_keyboard_release_all();		
				break;

				/* Press the right arrow key */
				case CMD_RIGHTARROW:
					usb_keyboard_press(KEY_RIGHT_ARROW);
					_delay_ms(KEY_PRESS_TIME);
					usb_keyboard_release_all();		
				break;
				
				/* Press the tab key */
				case CMD_TAB:
					usb_keyboard_press(KEY_TAB);
					_delay_ms(KEY_PRESS_TIME);
					usb_keyboard_release_all();
				break;
				
				/* Press the print screen key */
				case CMD_PRINTSCREEN:
					usb_keyboard_press(KEY_PRINTSCREEN);
					_delay_ms(KEY_PRESS_TIME);
					usb_keyboard_release_all();
				break;
				
				/* Press the function keys */
				case CMD_F1:
				case CMD_F2:
				case CMD_F3:
				case CMD_F4:
				case CMD_F5:
				case CMD_F6:
				case CMD_F7:
				case CMD_F8:
				case CMD_F9:
				case CMD_F10:
				case CMD_F11:
				case CMD_F12:
					usb_keyboard_press((cmd-CMD_F1)+KEY_F1);
					_delay_ms(KEY_PRESS_TIME);
					usb_keyboard_release_all();		
				break;

				default:		
				break;
			}
		}
	}
}

//*****************************************************************************
// Function:     delay_secs
// Called with:  secs = Number of seconds to wait.
// Returns:      Nothing.
// Purpose:      Delays for a number of seconds.
//*****************************************************************************
static void delay_secs(uint8_t secs)
{
   	uint8_t i,j;
	for (i = 0; i < secs; i++)
	{
		for (j = 0; j < 100; j++)
   		{
	   		_delay_ms(10);
   		}		
	}
}

//*****************************************************************************
// Function:     delay_msecs
// Called with:  msec = Number of milliseconds to wait.
// Returns:      Nothing.
// Purpose:      Delays for a number of milliseconds.
//*****************************************************************************
static void delay_msecs(uint8_t msec)
{
	uint8_t i;
	for (i = 0; i < msec; i++)
	{
		_delay_ms(1);
	}		
}

//*****************************************************************************
// Function:     delay_mins
// Called with:  min = Number of minutes to wait.
// Returns:      Nothing.
// Purpose:      Delays for a number of minutes.
//*****************************************************************************
static void delay_mins(uint8_t mins)
{
   	uint8_t i;
	for (i = 0; i < mins; i++)
	{
		delay_secs(60);	
	}
}
