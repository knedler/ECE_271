#include <string.h>
#include <stdio.h>
#include "stm32l476xx.h"
#include "SysClock.h"
#include "I2C.h"
#include "ssd1306.h"
#include "ssd1306_tests.h"

// Note: The code gets Warning L6989W
// Report: this is an known issue for STM32L4xx.
// https://www.keil.com/support/docs/3853.htm
// We can ignore this warning because we are not using
// external memory.

uint8_t Data_Receive[6];
uint8_t Data_Send[6];


void I2C_GPIO_init(void);
	
int main(void){
	volatile int i;
	int count = 0;
	char message[64] = "ABCDEFGHIJK";
	
	System_Clock_Init(); // Switch System Clock = 80 MHz
	I2C_GPIO_init();
	I2C_Initialization(I2C1);

	//ssd1306_TestAll();
	ssd1306_Init();
	ssd1306_Fill(White);
	ssd1306_SetCursor(2,0);
	ssd1306_WriteString(message, Font_11x18, Black);		
	ssd1306_UpdateScreen();	
	
	while(1){
		for(i=0; i<100000; i++);
		sprintf(message, "%d", count);
		ssd1306_Fill(White);
		ssd1306_SetCursor(2,0);
		ssd1306_WriteString(message, Font_11x18, Black);
		ssd1306_UpdateScreen();	
		count++;
	}
	
	while(1);	 // Deadloop
}

