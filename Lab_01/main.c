#include "stm32l476xx.h"
#include "SysClock.h"

// PA.5  <--> Green LED
// PC.13 <--> Blue user button
#define LED_PIN    5
#define BUTTON_PIN 13

void configure_LED_pin()
{
  // Enable the clock to GPIO Port A	
  RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN;  
		
	// GPIO Mode: Input(00), Output(01), AlterFunc(10), Analog(11, reset)
	GPIOA->MODER &= ~(3UL<<(2*LED_PIN));  
	GPIOA->MODER |=   1UL<<(2*LED_PIN);      // Output(01)
	
	// GPIO Speed: Low speed (00), Medium speed (01), Fast speed (10), High speed (11)
	GPIOA->OSPEEDR &= ~(3<<(2*LED_PIN));
	GPIOA->OSPEEDR |=   2<<(2*LED_PIN);  // Fast speed
	
	// GPIO Output Type: Output push-pull (0, reset), Output open drain (1) 
	GPIOA->OTYPER &= ~(1<<LED_PIN);      // Push-pull
	
	// GPIO Push-Pull: No pull-up, pull-down (00), Pull-up (01), Pull-down (10), Reserved (11)
	GPIOA->PUPDR  &= ~(3<<(2*LED_PIN));  // No pull-up, no pull-down
}

void configure_button()
{
	// Enable the clock to GPIO Port C
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOCEN;
	
	// GPIO Mode: Input(00), Output(01), AlterFunc(10), Analog(11, reset)
	GPIOC->MODER &= ~(3UL<<(2*BUTTON_PIN)); 
	//GPIOC->MODER |=   0UL<<(2*BUTTON_PIN);      // Input(00)

	// GPIO Push-Pull: No pull-up, pull-down (00), Pull-up (01), Pull-down (10), Reserved (11)
	GPIOC->PUPDR  &= ~(3<<(2*BUTTON_PIN)); // No pull-up, no pull-down
}

int main(void)
{	
	System_Clock_Init(); // Switch System Clock = 80 MHz
	configure_LED_pin();
	configure_button();
	
	uint32_t timer = 0; // Timer for led flip
	
	// Turn on the LED
	GPIOA->ODR |= 1 << LED_PIN;
	
	// Wait for button press
	while (GPIOC->IDR & GPIO_IDR_IDR_13);
	// Wait for button release
	while (!(GPIOC->IDR & GPIO_IDR_IDR_13));
	
	// Infinite loop for the led flip
	while (1) 
	{
		timer++;
		// About 1 sec switching
		if (0 == (timer % 1000000)) 
		{
			// Toggle LED
			GPIOA->ODR ^= 1 << LED_PIN;
		}
		
		//If button is pressed
		if (!(GPIOC->IDR & GPIO_IDR_IDR_13)) 
		{
			//1st button press to pause
			while (!(GPIOC->IDR & GPIO_IDR_IDR_13));
			
			// 2nd button press to continue
			while (GPIOC->IDR & GPIO_IDR_IDR_13);
			while (!(GPIOC->IDR & GPIO_IDR_IDR_13));
		}
	}
  // Dead loop & program hangs here
	while(1);
}
