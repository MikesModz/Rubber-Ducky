#ifndef __USB_KEYBOARD_H__
#define __USB_KEYBOARD_H__

#include <stdint.h>

#define SHIFT				0x80

#define KEY_LEFT_CTRL		0x80
#define KEY_LEFT_SHIFT		0x81
#define KEY_LEFT_ALT		0x82
#define KEY_LEFT_GUI		0x83
#define KEY_RIGHT_CTRL		0x84
#define KEY_RIGHT_SHIFT		0x85
#define KEY_RIGHT_ALT		0x86
#define KEY_RIGHT_GUI		0x87
#define KEY_UP_ARROW		0xDA
#define KEY_DOWN_ARROW		0xD9
#define KEY_LEFT_ARROW		0xD8
#define KEY_RIGHT_ARROW		0xD7
#define KEY_BACKSPACE		0xB2
#define KEY_TAB				0xB3
#define KEY_RETURN			0xB0
#define KEY_ESC				0xB1
#define KEY_INSERT			0xD1
#define KEY_DELETE			0xD4
#define KEY_PAGE_UP			0xD3
#define KEY_PAGE_DOWN		0xD6
#define KEY_HOME			0xD2
#define KEY_END				0xD5
#define KEY_CAPS_LOCK		0xC1
#define KEY_F1				0xC2
#define KEY_F2				0xC3
#define KEY_F3				0xC4
#define KEY_F4				0xC5
#define KEY_F5				0xC6
#define KEY_F6				0xC7
#define KEY_F7				0xC8
#define KEY_F8				0xC9
#define KEY_F9				0xCA
#define KEY_F10				0xCB
#define KEY_F11				0xCC
#define KEY_F12				0xCD
#define KEY_NONE			0xFF
#define KEY_SPACE			0x20
#define KEY_PRINTSCREEN		0x46

#define KEY_PRESS_TIME		10			// Time key is held down in milliseconds

typedef struct
{
  uint8_t modifier_keys;
  uint8_t reserved;
  uint8_t keys[6];
} KeyReport;

void usb_init(void);
uint8_t usb_configured(void);
int8_t usb_keyboard_send(KeyReport *keys);
int8_t usb_keyboard_press(uint8_t k);
int8_t usb_keyboard_release(uint8_t k) ;
int8_t usb_keyboard_release_all(void);
int8_t usb_keyboard_write(uint8_t c);
int16_t usb_keyboard_print(const uint8_t *buffer, int16_t size);
int16_t usb_keyboard_print_line(const uint8_t *buffer, int16_t size);

// Everything below this point is only intended for usb_keyboard.c
#ifdef USB_KEYBOARD_PRIVATE_INCLUDE

#include <avr/io.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#define EP_TYPE_CONTROL					0x00
#define EP_TYPE_BULK_IN			  	    0x81
#define EP_TYPE_BULK_OUT			    0x80
#define EP_TYPE_INTERRUPT_IN		    0xC1
#define EP_TYPE_INTERRUPT_OUT		    0xC0
#define EP_TYPE_ISOCHRONOUS_IN			0x41
#define EP_TYPE_ISOCHRONOUS_OUT			0x40

#define EP_SINGLE_BUFFER				0x02
#define EP_DOUBLE_BUFFER				0x06

#define EP_SIZE(s)						((s) == 64 ? 0x30 :	\
										((s) == 32 ? 0x20 :	\
										((s) == 16 ? 0x10 :	\
			             				0x00)))

#define MAX_ENDPOINT					4

#define LSB(n) 							(n & 255)
#define MSB(n) 							((n >> 8) & 255)

#if defined(__AVR_AT90USB162__)
#define HW_CONFIG() 
#define PLL_CONFIG() 				    (PLLCSR = ((1<<PLLE)|(1<<PLLP0)))
#define USB_CONFIG() 				    (USBCON = (1<<USBE))
#define USB_FREEZE() 				    (USBCON = ((1<<USBE)|(1<<FRZCLK)))
#elif defined(__AVR_ATmega32U4__)
#define HW_CONFIG() 			        (UHWCON = 0x01)
#define PLL_CONFIG() 				    (PLLCSR = 0x12)
#define USB_CONFIG() 				    (USBCON = ((1<<USBE)|(1<<OTGPADE)))
#define USB_FREEZE() 				    (USBCON = ((1<<USBE)|(1<<FRZCLK)))
#elif defined(__AVR_AT90USB646__)
#define HW_CONFIG() 				    (UHWCON = 0x81)
#define PLL_CONFIG() 				    (PLLCSR = 0x1A)
#define USB_CONFIG() 				    (USBCON = ((1<<USBE)|(1<<OTGPADE)))
#define USB_FREEZE() 				    (USBCON = ((1<<USBE)|(1<<FRZCLK)))
#elif defined(__AVR_AT90USB1286__)
#define HW_CONFIG() 				    (UHWCON = 0x81)
#define PLL_CONFIG() 				    (PLLCSR = 0x16)
#define USB_CONFIG() 				    (USBCON = ((1<<USBE)|(1<<OTGPADE)))
#define USB_FREEZE() 				    (USBCON = ((1<<USBE)|(1<<FRZCLK)))
#endif

// standard control endpoint request types
#define GET_STATUS					    0
#define CLEAR_FEATURE				    1
#define SET_FEATURE					    3
#define SET_ADDRESS					    5
#define GET_DESCRIPTOR				    6
#define GET_CONFIGURATION			    8
#define SET_CONFIGURATION			    9
#define GET_INTERFACE					10
#define SET_INTERFACE				    11
// HID (human interface device)
#define HID_GET_REPORT				    1
#define HID_GET_IDLE				    2
#define HID_GET_PROTOCOL			    3
#define HID_SET_REPORT				    9
#define HID_SET_IDLE			  	    10
#define HID_SET_PROTOCOL			    11
// CDC (communication class device)
#define CDC_SET_LINE_CODING				0x20
#define CDC_GET_LINE_CODING			    0x21
#define CDC_SET_CONTROL_LINE_STATE		0x22
#endif
#endif
