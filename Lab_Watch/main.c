#include <string.h>
#include <stdio.h>
#include <math.h>
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

#define Pi 	3.14159265358979323846

void I2C_GPIO_init(void);

void LCD_RTC_Clock_Enable(void)
{
	// Enable write access to the backup domain
	if ((RCC->APB1ENR1 & RCC_APB1ENR1_PWREN) == 0)
	{
		// Enable power interface clock
		RCC->APB1ENR1 |= RCC_APB1ENR1_PWREN;
		// Short delay after enabling an RCC peripheral clock
		(void) RCC->APB1ENR1;
	}
	
	// Select LSE as RTC clock source
	// RTCILCD Clock: (1) LSE is in the backup domain (2) HSE and LSI are not
	if ((PWR->CR1 & PWR_CR1_DBP) == 0) 
	{
		// Enable write access to RTC and registers in backup domain
		PWR->CR1 |= PWR_CR1_DBP;
	
		// Wait until the backup domain write protection has been disabled
		while((PWR->CR1 & PWR_CR1_DBP) == 0);
	}
	
	// Reset LSEON and LSEBYP bits before configuring LSE
	// BDCR = Backup Domain Control Register
	RCC->BDCR &= ~(RCC_BDCR_LSEON | RCC_BDCR_LSEBYP);
	
	// RTC Clock selection can be changed only if the backup domain is reset
	RCC->BDCR |= RCC_BDCR_BDRST;
	RCC->BDCR &= ~RCC_BDCR_BDRST;
	
	// Wait untill LSE clock is ready
	while((RCC->BDCR & RCC_BDCR_LSERDY) == 0) 
	{
		RCC->BDCR |= RCC_BDCR_LSEON; // Enable LSE oscillator
	}
	
	// Select LSE as RTC clock source
	// RTCSEL[l:B]: ee = No Clock, Bl = LSE, 1e = LSI, 11 = HSE
	RCC->BDCR &= ~RCC_BDCR_RTCSEL; // Clear RTCSEL bits
	RCC->BDCR |= RCC_BDCR_RTCSEL_0; // Select LSE as RTC clock
	
	// Disable power interface clock
	RCC->APB1ENR1 &= ~RCC_APB1ENR1_PWREN;
	
	// Enable RTC Clock
	RCC->BDCR |= RCC_BDCR_RTCEN;
}

void RTC_Init(void)
{
	// Enable RTC Clock
	LCD_RTC_Clock_Enable();
	
	// Disable write protection of RTC registers
	RTC->WPR = 0xCA;
	RTC->WPR = 0x53;
	
	// Enter initialization mode to program TR and DR registers
	RTC->ISR |= RTC_ISR_INIT;
	
	// Wait until INITF has been set
	while((RTC->ISR & RTC_ISR_INITF) == 0);
	
	// Hour format: 0 = 24 hour/day; 1 = AM/PM hour
	RTC->CR &= ~RTC_CR_FMT;
	
	// Generate a lHz clock for the RTC time counter
	// LSE = 32.768 kHz= 2^15 Hz
	RTC->PRER |= (127 << 16); // Asynch_Prescaler = 127
	RTC->PRER |= (255); // Synch_Prescaler = 255
	
	// Set time as 11:32:00 am
	RTC->TR = 0U<<22 | 1U<<20 | 1U<<16 | 3U<<12 | 2U<<8;
	
	// Set date as 2023/04/19
	RTC->DR = 2U<<20 | 3U<<16 | 0U<<12 | 4U<<8 | 1U<<4 | 9U;
	
	// Exit initialization mode
	RTC->ISR &= ~RTC_ISR_INIT;
	
	// Enable write protection for RTC registers
	RTC->WPR = 0xFF;
}

void drawClock(void)
{
		ssd1306_Fill(White);
		
		// Clock Shape
		ssd1306_DrawCircle(64,32,30,Black);
		
		// Clock Face
		ssd1306_SetCursor(88,30);
		ssd1306_WriteString("3", Font_6x8, Black);
		ssd1306_SetCursor(62,52);
		ssd1306_WriteString("6", Font_6x8, Black);
		ssd1306_SetCursor(36,30);
		ssd1306_WriteString("9", Font_6x8, Black);
		ssd1306_SetCursor(60,6);
		ssd1306_WriteString("12", Font_6x8, Black);
}


void drawHands(void)
{
	// Get hours
	int hours = (((RTC->TR >> 20) & 0xF) * 10) + ((RTC->TR >> 16) & 0xF);

	int hourX = 64 + 15 * cos(hours * (Pi / 6) - (Pi / 2));
	int hourY = 32 + 15 * sin(hours * (Pi / 6) - (Pi / 2));
	
	// Draw hour hand
	ssd1306_Line(64,32,hourX,hourY,Black);
	
	// Get minutes
	int minutes = (((RTC->TR >> 12) & 0xF) * 10) + ((RTC->TR >> 8) & 0xF);

	int minuteX = 64 + 22 * cos(minutes * (Pi / 30) - (Pi / 2));
	int minuteY = 32 + 22 * sin(minutes * (Pi / 30) - (Pi / 2));
	
	// Draw minute hand
	ssd1306_Line(64,32,minuteX,minuteY,Black);
	
	// Get seconds 
	int seconds = (((RTC->TR >> 4) & 0xF) * 10)+ (RTC->TR & 0xF);
	
	int secondX = 64 + 30 * cos(seconds * (Pi / 30) - Pi / 2);
	int secondY = 32 + 30 * sin(seconds * (Pi / 30) - Pi / 2);
	
	// Draw seconds hand
	ssd1306_Line(64,32,secondX,secondY,Black);
}
	
int main(void){
	System_Clock_Init(); // Switch System Clock = 80 MHz
	I2C_GPIO_init();
	I2C_Initialization(I2C1);
	ssd1306_Init();
	RTC_Init();
	
	while(1)
	{
		drawClock();
		
		drawHands();

		ssd1306_UpdateScreen();	
	}
	
	while(1);	 // Deadloop
}

