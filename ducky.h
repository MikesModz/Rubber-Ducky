#ifndef __DUCKY_H__
#define __DUCKY_H__

#define CPU_PRESCALE(n)			(CLKPR = 0x80, CLKPR = (n))
#define CPU_16MHz				0x00
#define CPU_8MHz				0x01
#define CPU_4MHz				0x02
#define CPU_2MHz				0x03
#define CPU_1MHz				0x04
#define CPU_500kHz				0x05
#define CPU_250kHz				0x06
#define CPU_125kHz				0x07
#define CPU_62kHz				0x08

#define CMD_CTRL		    	0xF0
#define CMD_ALT				    0xF1
#define CMD_GUI				    0xF2
#define CMD_SHIFT			    0xF3
#define CMD_ENTER			    0xF4
#define CMD_STRING			    0xF5
#define CMD_DELAY			    0xF6
#define CMD_DELAYS			    0xF7
#define CMD_MENU                0xF8
#define CMD_ESCAPE              0xF9
#define CMD_CAPSLOCK            0xFA
#define CMD_UPARROW             0xFB
#define CMD_DOWNARROW           0xFC
#define CMD_RIGHTARROW          0xFD
#define CMD_LEFTARROW           0xFE
#define CMD_TAB					0xFF

#define CMD_F1                  0xE0
#define CMD_F2                  0xE1
#define CMD_F3                  0xE2
#define CMD_F4                  0xE3
#define CMD_F5                  0xE4
#define CMD_F6                  0xE5
#define CMD_F7                  0xE6
#define CMD_F8                  0xE7
#define CMD_F9                  0xE8
#define CMD_F10                 0xE9
#define CMD_F11                 0xEA
#define CMD_F12                 0xEB
#define CMD_DELAYR              0xEC
#define CMD_PRINTSCREEN			0xED

#define NULL                    ((void *)0)

#if defined(__AVR_ATmega32U4__)
 #define LED1_DIR				DDRE
 #define LED1_PORT				PORTE
 #define LED1_PIN				PE6
 #define LED2_DIR				DDRB
 #define LED2_PORT				PORTB
 #define LED2_PIN				PB5
 #define HWB_DIR				DDRE
 #define HWB_PORT				PORTE
 #define HWB_PINS				PINE
 #define HWB_PIN				PE2
#elif defined(__AVR_AT90USB162__)
 #define LED1_DIR				DDRD
 #define LED1_PORT				PORTD
 #define LED1_PIN				PD6
 #define LED2_DIR				DDRD
 #define LED2_PORT				PORTD
 #define LED2_PIN				PD5
 #define HWB_DIR				DDRD
 #define HWB_PORT				PORTD
 #define HWB_PINS				PIND
 #define HWB_PIN				PD7
#else
 #error "No LED definitions for MCU available"
#endif

#endif
