/*************************************************************************
Title:    Game Console Template Header File
Inital Author:  TENGWEI LAI  664420 
Software: AVR-GCC 
Hardware: ATMEGA16 @ 8Mhz 

DESCRIPTION:
	Macros for Game Console

*************************************************************************/

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdlib.h>

#define byte unsigned char 


//#define F_CPU		8000000UL

#define HIGH		0xFF
#define LOW			0x00

#define TRUE		0xFF
#define FALSE		0x00

#define ON			0xFF
#define OFF			0x00

#define OUT			0xFF
#define IN			0x00

#define DATA		0xFF
#define COMMAND		0x00

#define SELECT		0x00
#define DESELECT	0xFF

// ADC
#define Varef		2.56
#define BAT_MIN		1.2

// LCD
#define MAX_PAGE		8
#define LCD_CLEAN		0x00
#define MAX_COLUMN		102
#define CMD_PAGE		0b10110000
#define CMD_COL_LSB		0b00000000
#define CMD_COL_MSB		0b00010000
#define ROW				32
#define COLUMN			51
#define PIXEL			0b10000000
#define PAGE			4

// FRAM
#define WREN			0b00000110
#define WRITEFM			0b00000010
#define GETFM			0b00000011

// PWM
#define BOTTOM			0x00
#define TOP				0xFF


#define WRITE(REGISTER, MASK, VALUE)	REGISTER = ((VALUE & MASK) | (REGISTER & ~MASK))
#define GET(PIN, MASK)					PIN & MASK


#define ADC_DIR(DIR)			WRITE(DDRA, _BV(PA1), DIR)
#define LOW_LED_DIR(DIR)		WRITE(DDRA, _BV(PA2), DIR)
#define BUTTON_UP_DIR(DIR)		WRITE(DDRC, _BV(PA6), DIR)
#define BUTTON_DN_DIR(DIR)		WRITE(DDRC, _BV(PA7), DIR)
#define BUTTON_LT_DIR(DIR)		WRITE(DDRA, _BV(PC7), DIR)
#define BUTTON_RT_DIR(DIR)		WRITE(DDRA, _BV(PC6), DIR)
#define BUTTON_C_DIR(DIR)		WRITE(DDRA, _BV(PA3), DIR)
#define BUTTON_B_DIR(DIR)		WRITE(DDRA, _BV(PA4), DIR)
#define BUTTON_A_DIR(DIR)		WRITE(DDRA, _BV(PA5), DIR)

#define PWM_DIR(DIR)			WRITE(DDRB, _BV(PB3), DIR)
#define SS_DIR(DIR)				WRITE(DDRB, _BV(PB4), DIR)
#define MOSI_DIR(DIR)			WRITE(DDRB, _BV(PB5), DIR)
#define MISO_DIR(DIR)			WRITE(DDRB, _BV(PB6), DIR)
#define SCK_DIR(DIR)			WRITE(DDRB, _BV(PB7), DIR)

#define CSFM_DIR(DIR)			WRITE(DDRD, _BV(PD4), DIR)
#define CSLCD_DIR(DIR)			WRITE(DDRD, _BV(PD4), DIR)
#define RSTLCD_DIR(DIR)			WRITE(DDRD, _BV(PD6), DIR)
#define CDLCD_DIR(DIR)			WRITE(DDRD, _BV(PD7), DIR)
#define HOLDFM_DIR(DIR)			WRITE(DDRD, _BV(PD5), DIR)
#define WPFM_DIR(DIR)			WRITE(DDRD, _BV(PD3), DIR)


#define LOW_LED_STATE(STATE)	WRITE(PORTA, _BV(PA2), ~STATE)

#define PWM_STATE(STATE)		WRITE(PORTB, _BV(PB3), STATE)
#define SS_STATE(STATE)			WRITE(PORTB, _BV(PB4), STATE)

#define CSFM_STATE(STATE)		WRITE(PORTD, _BV(PD4), STATE)
#define CSLCD_STATE(STATE)		WRITE(PORTD, _BV(PD4), STATE)
#define RSTLCD_STATE(STATE)		WRITE(PORTD, _BV(PD6), STATE)
#define CDLCD_STATE(STATE)		WRITE(PORTD, _BV(PD7), STATE)
#define HOLDFM_STATE(STATE)		WRITE(PORTD, _BV(PD5), STATE)
#define WPFM_STATE(STATE)		WRITE(PORTD, _BV(PD3), STATE)


#define BUTTON_UP		~GET(PINC, _BV(PA6))
#define BUTTON_LT		~GET(PINA, _BV(PC7))
#define BUTTON_DN		~GET(PINC, _BV(PA7))
#define BUTTON_RT		~GET(PINA, _BV(PC6))
#define BUTTON_C		~GET(PINA, _BV(PA3))
#define BUTTON_B		~GET(PINA, _BV(PA4))
#define BUTTON_A		~GET(PINA, _BV(PA5))


#define PULL_UP_UP		WRITE(PORTC, _BV(PA6), HIGH)
#define PULL_LT_UP		WRITE(PORTA, _BV(PC7), HIGH)
#define PULL_DN_UP		WRITE(PORTC, _BV(PA7), HIGH)
#define PULL_RT_UP		WRITE(PORTA, _BV(PC6), HIGH)
#define PULL_C_UP		WRITE(PORTA, _BV(PA3), HIGH)
#define PULL_B_UP		WRITE(PORTA, _BV(PA4), HIGH)
#define PULL_A_UP		WRITE(PORTA, _BV(PA5), HIGH)
