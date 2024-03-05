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
	GPIOA->MODER |=   2UL<<(2*LED_PIN);      // AlterFunc(10)
	
	// GPIO Speed: Low speed (00), Medium speed (01), Fast speed (10), High speed (11)
	GPIOA->OSPEEDR &= ~(3<<(2*LED_PIN));
	GPIOA->OSPEEDR |=   2<<(2*LED_PIN);  // Fast speed
	
	// GPIO Output Type: Output push-pull (0, reset), Output open drain (1) 
	GPIOA->OTYPER &= ~(1<<LED_PIN);      // Push-pull
	
	// GPIO Push-Pull: No pull-up, pull-down (00), Pull-up (01), Pull-down (10), Reserved (11)
	GPIOA->PUPDR  &= ~(3<<(2*LED_PIN));  // No pull-up, no pull-down
	
	// GPIO AFR: 
	GPIOA->AFR[0] &= ~(3UL<<(4*LED_PIN));
	GPIOA->AFR[0] |= 1UL<<(4*LED_PIN);
}

void configure_servo_pin()
{
	// GPIO Mode: Input(00), Output(01), AlterFunc(10), Analog(11, reset)
	GPIOA->MODER &= ~(3UL);  
	GPIOA->MODER |=   2UL;      // AlterFunc(10)
	
	// GPIO Speed: Low speed (00), Medium speed (01), Fast speed (10), High speed (11)
	GPIOA->OSPEEDR &= ~(3);
	GPIOA->OSPEEDR |=   2;  // Fast speed
	
	// GPIO Output Type: Output push-pull (0, reset), Output open drain (1) 
	GPIOA->OTYPER &= ~(1);      // Push-pull
	
	// GPIO Push-Pull: No pull-up, pull-down (00), Pull-up (01), Pull-down (10), Reserved (11)
	GPIOA->PUPDR  &= ~(3);  // No pull-up, no pull-down
	
	// GPIO AFR: 
	GPIOA->AFR[0] &= ~(3);
	GPIOA->AFR[0] |= 2;
}

void configure_button() 
	{
	// Enable the clock to GPIO Port C
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOCEN;
	
	// GPIO Mode: Input(00), Output(01), AlterFunc(10), Analog(11, reset)
	GPIOC->MODER &= ~(3UL<<(2*BUTTON_PIN));

	// GPIO Push-Pull: No pull-up, pull-down (00), Pull-up (01), Pull-down (10), Reserved (11)
	GPIOC->PUPDR  &= ~(3<<(2*BUTTON_PIN)); // No pull-up, no pull-down
}

void configure_exti() 
	{
	
	// Select PC.13 as the trigger source of EXTI 13
	RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;
	
	SYSCFG->EXTICR[3] &= ~SYSCFG_EXTICR4_EXTI13;
	SYSCFG->EXTICR[3] |= SYSCFG_EXTICR4_EXTI13_PC;
	SYSCFG->EXTICR[3] &= ~(0x000F);
	
	// Rising trigger selection
	// 0 = trigger disabled, 1 = trigger enabled
	EXTI->RTSR1 &= ~EXTI_RTSR1_RT13;
	
	// Falling trigger selection
	// 0 = trigger disabled, 1 = trigger enabled
	EXTI->FTSR1 |= EXTI_FTSR1_FT13;
	
	// Interupt Mask Register
	// 0 = masked, 1 = not masked (enabled)
	EXTI->IMR1 |= EXTI_IMR1_IM13;
	
	// Set priority
	NVIC_SetPriority(EXTI15_10_IRQn, 1);
	
	//Enable Interupt
	NVIC_EnableIRQ(EXTI15_10_IRQn);
}

void TIM2_Init()
{
	// Enable TIMER 2 clock
	RCC->APB1ENR1 |= RCC_APB1ENR1_TIM2EN;
	
	// Counting direction: 0 = Up-counting, 1 = Down-counting
	TIM2->CR1 &= ~TIM_CR1_DIR; // Select up-counting
	
	// Prescaler, slow down the input clock by a factor of (1 + prescaler)
	TIM2->PSC = 39; // 4 MHz | (1 + 39) = 100 KHz
	
	// Auto-reload
	TIM2->ARR = 999; // PWM period = (999 + 1) * 1/100KHz = 0.01s
	
	// Clear output compare mode bits for channel 1
	TIM2->CCMR1 &= ~TIM_CCMR1_OC1M;
	
	// Select PWM Mode 1 output on channel 1 (OC1M = 110)
	TIM2->CCMR1 |= TIM_CCMR1_OC1M_1 | TIM_CCMR1_OC1M_2;
	
	// Output 1 preload enable
	TIM2->CCMR1 |= TIM_CCMR1_OC1PE;
	
	// Select output polarity: 0 = Active high, 1 = Active Low
	TIM2->CCER &= ~TIM_CCER_CC1NP; // OC1N = OCREF + CC1NP
	
	// Enable complementary output of channel 1 (CH1N)
	TIM2->CCER |= TIM_CCER_CC1E;
	
	// Output Compare Register for channel 1
	TIM2->CCR1 = 500; // Initial duty cycle 50%
	
	// Enable counter
	TIM2->CR1 |= TIM_CR1_CEN; 
}

void TIM5_Init()
{
	// Enable TIMER 5 clock
	RCC->APB1ENR1 |= RCC_APB1ENR1_TIM5EN;
	
	// Counting direction: 0 = Up-counting, 1 = Down-counting
	TIM5->CR1 &= ~TIM_CR1_DIR; // Select up-counting
	
	// Prescaler, slow down the input clock by a factor of (1 + prescaler)
	TIM5->PSC = 39; // 4 MHz | (1 + 39) = 100 KHz
	
	// Auto-reload
	TIM5->ARR = 1999; // PWM period = (1999 + 1) * 1/100kHz = 0.02s
	
	// Clear output compare mode bits for channel 1
	TIM5->CCMR1 &= ~TIM_CCMR1_OC1M;
	
	// Select PWM Mode 1 output on channel 1 (OC1M = 110)
	TIM5->CCMR1 |= TIM_CCMR1_OC1M_1 | TIM_CCMR1_OC1M_2;
	
	// Output 1 preload enable
	TIM5->CCMR1 |= TIM_CCMR1_OC1PE;
	
	// Select output polarity: 0 = Active high, 1 = Active Low
	TIM5->CCER &= ~TIM_CCER_CC1NP; // OC1N = OCREF + CC1NP
	
	// Enable complementary output of channel 1 (CH1N)
	TIM5->CCER |= TIM_CCER_CC1E;
}

void servo()
{
	int i;
	
	// Output Compare Register for channel 1
	TIM5->CCR1 = 160; // Initial duty cycle 5%
	
	// Enable counter
	TIM5->CR1 |= TIM_CR1_CEN; // 0 degrees
	
	for(i = 0; i < 1000000; i++);
	
	// Disable counter
	TIM5->CR1 &= ~TIM_CR1_CEN;
	
	// Output Compare Register for channel 1
	TIM5->CCR1 = 260; // Initial duty cycle 5%
	
	// Enable counter
	TIM5->CR1 |= TIM_CR1_CEN; // 90 degrees
	
	for(i = 0; i < 1000000; i++);
	
	// Disable counter
	TIM5->CR1 &= ~TIM_CR1_CEN;
	
	// Output Compare Register for channel 1
	TIM5->CCR1 = 60; // Initial duty cycle 5%
	
	// Enable counter
	TIM5->CR1 |= TIM_CR1_CEN; // -90 degrees
	
	for(i = 0; i < 1000000; i++);
	
	// Disable counter
	TIM5->CR1 &= ~TIM_CR1_CEN;
	
	// Output Compare Register for channel 1
	TIM5->CCR1 = 160; // Initial duty cycle 5%
	
	// Enable counter
	TIM5->CR1 |= TIM_CR1_CEN; // 0 degrees
}

void EXTI15_10_IRQHandler(void){
	int i;
	//Check for EXTI 13 Interrupt Flag
	if ((EXTI->PR1 & EXTI_PR1_PIF13) == EXTI_PR1_PIF13) {
		if(TIM5->CCR1 == 160){
			TIM5->CCR1 = 260;
			
			TIM5->CR1 |= TIM_CR1_CEN; //0 Degrees
			
			for(i=0; i< 250000; i++);
			
			TIM5->CR1 |= TIM_CR1_CEN; //90 Degrees
			
			TIM5->CCR1 = 60;
			
			// Enable counter
			TIM5->CR1 |= TIM_CR1_CEN; // -90 degrees
			
			for(i = 0; i < 250000; i++);
			
			// Disable counter
			TIM5->CR1 &= ~TIM_CR1_CEN;
			
			TIM5->CCR1 = 260;
			
			// Enable counter
			TIM5->CR1 |= TIM_CR1_CEN; // 90 degrees
			
			for(i = 0; i < 250000; i++);
			
			// Disable counter
			TIM5->CR1 &= ~TIM_CR1_CEN;	

			// Output Compare Register for channel 1
			TIM5->CCR1 = 160; // Initial duty cycle 5%
	
			// Enable counter
			TIM5->CR1 |= TIM_CR1_CEN; // 0 degrees
		}
	}
		EXTI->PR1 |= EXTI_PR1_PIF13; //Write 1 to clear
}


int main(void)
{
	configure_LED_pin();
	configure_servo_pin();
	configure_button();
	configure_exti();
	TIM2_Init();
	TIM5_Init();
	
	servo();
	
	int i;
	int brightness = 1;
	int stepSize = 1;
	
	while(1)
	{
		if ((brightness >= 99) || (brightness <= 0))
			stepSize = -stepSize;
		
		brightness += stepSize;
		
		TIM2->CCR1 = brightness;
		
		for(i = 0; i < 1000; i++);
	}
	
  // Dead loop & program hangs here
	while(1);
}
