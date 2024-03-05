#include <string.h>
#include <stdio.h>
#include "stm32l476xx.h"
#include "SysClock.h"
#include "I2C.h"
#include "ssd1306.h"
#include "ssd1306_tests.h"
#include "keypad.h"

// Note: The code gets Warning L6989W
// Report: this is an known issue for STM32L4xx.
// https://www.keil.com/support/docs/3853.htm
// We can ignore this warning because we are not using
// external memory.

uint8_t Data_Receive[6];
uint8_t Data_Send[6];


void I2C_GPIO_init(void);

int iterate_rows (char message[64], uint8_t col) 
{
	uint8_t row;
	//Set the column to the defined pin
	switch (col) {
		case 1: 
			col = PC4;
			break;
		case 2:
			col = PC10;
			break;
		case 3:
			col = PC11;
			break;
		case 4:
			col = PC12;
			break;
	}
	//Clear and set pins for row 1
	GPIOC->ODR &= ~(1 << PC0);
	GPIOC->ODR |= ~(1 << PC0);
	
	delay(1);
	//If value is 0
	if (!(GPIOC->IDR & (1 << col)))
	{
		//Set the row
		row = 1;
		return row;
	}
	//Clear and set pins for row 2
	GPIOC->ODR &= ~(1 << PC1);
	GPIOC->ODR |= ~(1 << PC1);
	
	delay(1);
	//If value is 0
	if (!(GPIOC->IDR & (1 << col)))
	{
		//Set the row
		row = 2;
		return row;
	}
	//Clear and set pins for row 3
	GPIOC->ODR &= ~(1 << PC2);
	GPIOC->ODR |= ~(1 << PC2);
	
	delay(1);
	//If value is 0
	if (!(GPIOC->IDR & (1 << col)))
	{
		//Set the row
		row = 3;
		return row;
	}
	//Clear and set pins for row 4
	GPIOC->ODR &= ~(1 << PC3);
	GPIOC->ODR |= ~(1 << PC3);
	
	delay(1);
	//If value is 0
	if (!(GPIOC->IDR & (1 << col)))
	{
		//Set the row
		row = 4;
		return row;
	}
	//Return 0 if error
	return 0;
}

void add_keypress (uint8_t *pressed, char msg[64], char message[64], char val) 
{	
	if (*pressed == 0) 
	{	//If a key was not previously pressed
		if (val == '*') 
		{	//Delete the last value if '*' was hit
			msg[strlen(msg)-1] = '\0';
		}
		else if (val == '#')
		{ //Clear the screen if '#' was hit
			msg[0] = '\0';
		}
		else 
		{ //Add on the new val to the string
			strncat(msg, &val, 1);
		}
		//Print the string to the screen
		sprintf(message, "%s", msg);
		ssd1306_SetCursor(2,0);
		ssd1306_WriteString(message, Font_11x18, White);
		ssd1306_UpdateScreen();
		//Set pressed flag
		*pressed = 1;
	}
}

void keypad_scan (char message[64], char msg[64], uint8_t *pressed)
{
	uint32_t c1, c2, c3, c4;
	uint8_t col, row;
	char val;
	//Clear the bits
	GPIOC->ODR &= ~0xF;
	
	delay(1);
	//Get the state of the columns
	c1 = (GPIOC->IDR & (1 << PC4));
	c2 = (GPIOC->IDR & (1 << PC10));
	c3 = (GPIOC->IDR & (1 << PC11));
	c4 = (GPIOC->IDR & (1 << PC12));
	
	ssd1306_Fill(Black);
	
	if (c1 == 0)
	{ //If column 1 is pressed
		col = 1;
		row = iterate_rows(message, col);
		//Switch for the value associated with the row
		switch (row) {
			case 1:
				val = '1';
				break;
			case 2:
				val = '4';
				break;
			case 3:
				val = '7';
				break;
			case 4:
				val = '*';
				break;
		}

		add_keypress (pressed, msg, message, val);
	} 
	else if (c2 == 0)
	{ //If column 2 is pressed
		col = 2;
		row = iterate_rows(message, col);
		//Switch for the value associated with the row
		switch (row) {
			case 1:
				val = '2';
				break;
			case 2:
				val = '5';
				break;
			case 3:
				val = '8';
				break;
			case 4:
				val = '0';
				break;
		}
		
		add_keypress (pressed, msg, message, val);
	}
	else if (c3 == 0)
	{ //If column 3 is pressed
		col = 3;
		row = iterate_rows(message, col);
		//Switch for the value associated with the row
		switch (row) {
			case 1:
				val = '3';
				break;
			case 2:
				val = '6';
				break;
			case 3:
				val = '9';
				break;
			case 4:
				val = '#';
				break;
		}
		
		add_keypress (pressed, msg, message, val);
	}
	else if (c4 == 0)
	{ //If column 4 is pressed
		col = 4;
		row = iterate_rows(message, col);
		//Switch for the value associated with the row
		switch (row) {
			case 1:
				val = 'A';
				break;
			case 2:
				val = 'B';
				break;
			case 3:
				val = 'C';
				break;
			case 4:
				val = 'D';
				break;
		}
		
		add_keypress (pressed, msg, message, val);
	}
	else
	{ //If no key pressed, clear pressed flag
		*pressed = 0;
	}	
}
	
int main(void){
	char message[64] = "";
	char msg[64] = "";
	uint8_t pressed = 0;
	
	System_Clock_Init(); // Switch System Clock = 80 MHz
	I2C_GPIO_init();
	I2C_Initialization(I2C1);
	ssd1306_Init();
	Keypad_Pin_Init();
	
	while(1)
	{
		keypad_scan(message, msg, &pressed);
	}
	
	while(1);	 // Deadloop
}

