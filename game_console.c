/*************************************************************************
Title:    Game Console Template Header File
Inital Author:  TENGWEI LAI  664420 
Software: AVR-GCC 
Hardware: ATMEGA16 @ 8Mhz 

DESCRIPTION:
	Macros for Game Console

*************************************************************************/

#include "game_console.h"
#include <util/delay.h>


byte frameBuffer[MAX_COLUMN][MAX_PAGE];
int currentRow = ROW;
int currentColumn = COLUMN;
byte currentPage, currentPixel, currentdote;
byte keepPixel = FALSE;

int constrain(int x, int a, int b) {
	if (x<a) {
		return a;
	}
	if (x>b) {
		return b;
	}
	return x;
}

void delay(int n) {
	int i;
	for (i=0; i<n; i++) {
		_delay_ms(1);
	}
}


void pinInit(void) {
	BUTTON_UP_DIR(IN);
	PULL_UP_UP;

	BUTTON_DN_DIR(IN);
	PULL_DN_UP;

	BUTTON_LT_DIR(IN);
	PULL_LT_UP;

	BUTTON_RT_DIR(IN);
	PULL_RT_UP;

	BUTTON_A_DIR(IN);
	PULL_A_UP;

	BUTTON_B_DIR(IN);
	PULL_B_UP;

	BUTTON_C_DIR(IN);
	PULL_C_UP;

	SS_DIR(OUT);
	SS_STATE(HIGH);
}

void interruptInit(void) {
	sei();
	GICR = _BV(INT2);
	MCUCSR = _BV(ISC2);

}

void LOW_LED_INIT(void) {
	LOW_LED_STATE(OFF);
	_delay_ms(10);
	LOW_LED_DIR(OUT);
}


void SPI_MasterInit(void) {
	MOSI_DIR(OUT);
	SCK_DIR(OUT);


	SPCR = _BV(SPE) | _BV(MSTR) | _BV(SPR0);
}

void SPI_MasterTransmit(char cData) {
	SPDR = cData;
	while(!( SPSR & _BV(SPIF) ));
}


void resetLCD(void) {
	RSTLCD_DIR(OUT);
	
	RSTLCD_STATE(LOW);	
	_delay_ms(1);

	RSTLCD_STATE(HIGH);
	_delay_ms(5);
}

void LCD_Tx(char cd, char cData) {
	CDLCD_DIR(OUT);
	CDLCD_STATE(cd);

	CSLCD_DIR(OUT);
	CSLCD_STATE(LOW);

	SPI_MasterTransmit(cData);

	CSLCD_STATE(HIGH);
}

byte LCD_Init(void) {
	resetLCD();
/*LCD_command_tx(0x40);//Display start line 0
LCD_command_tx(0xA1);//SEG reverse
LCD_command_tx(0xC0);//Normal COM0~COM63
LCD_command_tx(0xA4);//Disable -> Set All Pixel to ON
LCD_command_tx(0xA6);//Display inverse off
_delay_ms(120);
LCD_command_tx(0xA2);//Set LCD Bias Ratio A2/A3
LCD_command_tx(0x2F);//Set Power Control 28...2F
LCD_command_tx(0x27);//Set VLCD Resistor Ratio 20...27
LCD_command_tx(0x81);//Set Electronic Volume
LCD_command_tx(0x10);//Set Electronic Volume 00...3F
LCD_command_tx(0xFA);//Set Adv. Program Control
LCD_command_tx(0x90);//Set Adv. Program Control x00100yz yz column wrap x Temp Comp
*///LCD_command_tx(0xAF);//Display on *
/*	LCD_Tx(COMMAND, 0xE4);
		
	LCD_Tx(COMMAND, 0x40);

	LCD_Tx(COMMAND, 0xA0);

	LCD_Tx(COMMAND, 0xC0);

	LCD_Tx(COMMAND, 0xA4);

	LCD_Tx(COMMAND, 0xA6);*/
	LCD_Tx(COMMAND, 0x40);

	LCD_Tx(COMMAND, 0xA0);

	LCD_Tx(COMMAND, 0xC0);

	LCD_Tx(COMMAND, 0xA4);

	LCD_Tx(COMMAND, 0xA6);
	_delay_ms(32);

//	LCD_Tx(COMMAND, 0xA4);
	LCD_Tx(COMMAND, 0xA2);
//	LCD_Tx(COMMAND, 0x2E);
	LCD_Tx(COMMAND, 0x2F);
	LCD_Tx(COMMAND, 0x27);

	LCD_Tx(COMMAND, 0x81);

	LCD_Tx(COMMAND, 0x10);

	LCD_Tx(COMMAND, 0xFA);

	LCD_Tx(COMMAND, 0x90);

	LCD_Tx(COMMAND, 0xAF);

	return(TRUE);
}

byte selectPage(byte page) {
	byte pageAddress = (CMD_PAGE | page);
	LCD_Tx(COMMAND, pageAddress);
	return(TRUE);
}

byte selectColumn(byte column) {
	column += 30;
	byte colAddressLSB = (CMD_COL_LSB | (column & 0x0F));
	byte colAddressMSB = (CMD_COL_MSB | (column >> 4));
	LCD_Tx(COMMAND, colAddressLSB);
	LCD_Tx(COMMAND, colAddressMSB);
	return(TRUE);
}

byte LCD_Clean(void) {
	byte page;
	for (page=0; page<MAX_PAGE; page++) {

		selectPage(page);

		byte column;
		for (column=0; column<MAX_COLUMN; column++) {

			selectColumn(column);
			LCD_Tx(DATA, LCD_CLEAN);
			frameBuffer[column][page] = 0;

		}

	}
	return(TRUE);
}

byte LCD_Ready(void) {
	selectPage(7);
	selectColumn(30);
	LCD_Tx(DATA, 0b00000001);
	return(TRUE);
}

void PWM_Init(void) {
	PWM_DIR(OUT);
	TCCR0 = _BV(WGM00) | _BV(COM01) | _BV(COM00) | _BV(WGM01) | _BV(CS00);
	OCR0 = 15;
}





ISR(INT2_vect) {

//INTERRUPT INTI
	char columnChange = 0;
	char pageChange = 0;
//Judge whether up down left right	
	if (BUTTON_UP || BUTTON_DN || BUTTON_LT || BUTTON_RT) {
		if (keepPixel) {
			frameBuffer[currentColumn][currentPage] = currentdote;
		}
		
		if (BUTTON_RT) {
			if (currentRow%8 == 0) {
				pageChange++;
			}
			currentRow++;
		} else if (BUTTON_LT) {
			currentRow--;
			if (currentRow%8 == 0) {
				pageChange--;
			}
		} else if (BUTTON_UP) {
			currentColumn++;
			columnChange++;
		} else if (BUTTON_DN) {
			currentColumn--;
			columnChange--;
		}
		currentColumn =  constrain(currentColumn, 0, MAX_COLUMN - 1);
		currentRow =  constrain(currentRow, 0, 64 - 1);

		currentPage = currentRow / 8;
		currentPixel = currentRow % 8;

		currentdote = _BV(currentPixel) | frameBuffer[currentColumn][currentPage];
		selectPage(currentPage);
		selectColumn(currentColumn);
		LCD_Tx(DATA, currentdote);

		if (columnChange != 0 || pageChange != 0) {
			byte changeRegister = frameBuffer[currentColumn-columnChange][currentPage-pageChange];
			selectPage(currentPage-pageChange);
			selectColumn(currentColumn-columnChange);
			LCD_Tx(DATA, changeRegister);
		}
	}
//DEFINE FUNCTION OF SWITCH A B C 
//A-----DRAW PIXELS   B C------SETUP THE BACKLIGHT
	if (BUTTON_A) 
	{
	  keepPixel = ~keepPixel;
	 } 
	else if (BUTTON_B) 
	{
	if(OCR0 < 245)
	OCR0 += 20;
	LOW_LED_STATE(ON);
    } 
	else if (BUTTON_C) 
	{
	if(OCR0 > 20)
	 OCR0 -= 20;
	}
	 _delay_ms(32);
}


int main(void) {
//PININTI
	pinInit();
//SPI INTI
	SPI_MasterInit();
//LCD INTI
	LCD_Init();
	_delay_ms(32);
	LCD_Clean();
//PWM BACKLIGHT INTI
	PWM_Init();
//INTERRUPT SWITCH INTI
	interruptInit();
	LOW_LED_INIT();
	currentPage = currentRow / 8;
	currentPixel = currentRow % 8;
	currentdote = _BV(currentPixel);

	while (TRUE) {

	}
	return(TRUE);
}
