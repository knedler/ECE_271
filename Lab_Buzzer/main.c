#include "stm32l476xx.h"
#include "SysClock.h"

// PA.5  <--> Green LED
// PC.13 <--> Blue user button
#define LED_PIN    5
#define BUTTON_PIN 13
#define BUZZER_PIN 1

#define SysClock 4000000 // 4 MHz
#define PWM_FREQUENCY 100000 // 100 kHz

// Define note frequencies
#define C4  262
#define D4  294
#define E4  330
#define F4  349
#define G4  392
#define A4  440
#define B4  494

#define C3  131
#define D3  147
#define E3  165
#define F3  175
#define G3  196
#define A3  220
#define B3  247

int melody[] = {
	B3, 1100,
	A3, 500,
	E3, 1250,
	G3, 1700,
	B3, 1100,
	A3, 500,
	E3, 1250,
	D3, 1700,
	B3, 1100,
	A3, 500,
	E3, 1250,
	G3, 1700,
	B3, 1100,
	A3, 500,
	E3, 1250,
	D3, 1700,
	B3, 1100,
	A3, 500,
	E3, 1250,
	G3, 1700,
	B3, 1100,
	A3, 500,
	E3, 1250,
	D3, 1700,
	B3, 1100,
	A3, 500,
	E3, 1250,
	G3, 1700,
	B3, 1100,
	A3, 500,
	E3, 500,
	G3, 500,
	D3, 1950,
	
	B3, 1100,
	D3, 500,
	E3, 1100,
	G3, 1100,
	A3, 500,
	B3, 1100,
	A3, 500,
	B3, 1100,
	D3, 1750,
	
	B3, 1100,
	D3, 500,
	E3, 1100,
	D3, 500,
	B3, 500,
	A3, 500,
	B3, 1100,
	A3, 500,
	B3, 1100,
	G3, 1750,
	D3, 1750,
	E3, 1100,
	G3, 1750,
	A3, 1100,
	B3, 500,
	G3, 1100,
	D3, 1750
};
	
volatile int TimeDelay;

void configure_BUZZER_pin()
{ 
	// Enable the clock to GPIO Port A	
  RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN;
	
	// GPIO Mode: Input(00), Output(01), AlterFunc(10), Analog(11, reset)
	GPIOA->MODER &= ~(3UL<<(2*BUZZER_PIN));  
	GPIOA->MODER |=   2UL<<(2*BUZZER_PIN);

	// GPIO Speed: Low speed (00), Medium speed (01), Fast speed (10), High speed (11)
	GPIOA->OSPEEDR &= ~(3UL<<(2*BUZZER_PIN));
	GPIOA->OSPEEDR |=  2UL<<(2*BUZZER_PIN);  // Fast speed
	
	// GPIO Output Type: Output push-pull (0, reset), Output open drain (1) 
	GPIOA->OTYPER &= ~(1UL<<BUZZER_PIN);      // Push-pull
	
	// GPIO Push-Pull: No pull-up, pull-down (00), Pull-up (01), Pull-down (10), Reserved (11)
	GPIOA->PUPDR  &= ~(3UL<<(2*BUZZER_PIN));  // No pull-up, no pull-down
	
	// AFR
	GPIOA->AFR[0] &= (3UL<<(4*BUZZER_PIN));
	GPIOA->AFR[0] |= 1UL<<(4*BUZZER_PIN);
}

void SysTick_Initialize(int ticks)
{
	// Disable SysTick
	SysTick->CTRL = 0;
	
	// Reload register
	SysTick->LOAD = ticks - 1;
	
	// Set interrupt
	NVIC_SetPriority (SysTick_IRQn, ( 1 << __NVIC_PRIO_BITS) - 1);
	
	// Reset counter
	SysTick->VAL = 0;
	
	// Select clock
	SysTick->CTRL |= SysTick_CTRL_CLKSOURCE_Msk;
	
	// Enable exception
	SysTick->CTRL |= SysTick_CTRL_TICKINT_Msk;
	
	// Enable SysTick timer
	SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;
}

void delay (uint32_t nTime)
{
	TimeDelay = nTime;
	while(TimeDelay != 0);
}

void SysTick_Handler(void)
{
	if (TimeDelay > 0)
		TimeDelay--;
}

void TIM2_Init()
{
	// Enable TIMER 2 clock
	RCC->APB1ENR1 |= RCC_APB1ENR1_TIM2EN;
	
	// Counting direction: 0 = Up-counting, 1 = Down-counting
	TIM2->CR1 &= ~TIM_CR1_DIR; // Select up-counting
	
	// Prescaler, slow down the input clock by a factor of (1 + prescaler)
	TIM2->PSC = 39; // 4 MHz / (1 + 39) = 100 KHz
	
	// Auto-reload
	TIM2->ARR = 227;
	
	// Clear output compare mode bits for channel 1
	TIM2->CCMR1 &= ~TIM_CCMR1_OC2M;
	
	// Select PWM Mode 1 output on channel 2 (OC1M = 110)
	TIM2->CCMR1 |= TIM_CCMR1_OC2M_1 | TIM_CCMR1_OC2M_2;
	
	// Output 1 preload enable
	TIM2->CCMR1 |= TIM_CCMR1_OC2PE;
	
	// Select output polarity: 0 = Active high, 1 = Active Low
	TIM2->CCER &= ~TIM_CCER_CC2NP; // OC1N = OCREF + CC1NP
	
	// Enable complementary output of channel 2 (CH2N)
	TIM2->CCER |= TIM_CCER_CC2E;
}

void playNote (uint16_t frequency, uint16_t duration)
{
	TIM2->ARR = PWM_FREQUENCY / frequency;
	
	TIM2->CCR2 = TIM2->ARR / 2;
	
	TIM2->CR1 |= TIM_CR1_CEN;
	
	delay(duration);
	
	TIM2->CR1 &= ~TIM_CR1_CEN;
	TIM2->CNT &= ~(0xFFFF);
}

void playSong ()
{
	int i;
	for (i = 0; i < (sizeof(melody) / sizeof(melody[0])); i += 2)
	{
		uint16_t frequency = melody[i];
		uint16_t duration = melody[i+1];
		playNote(frequency, duration);
		delay(10);
	}
}

int main(void){
	configure_BUZZER_pin();
	SysTick_Initialize(3999);
	TIM2_Init();
	
	playSong();

	while(1);
}
