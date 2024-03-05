#include "stm32l476xx.h"
#include "SysClock.h"

// PA.5  <--> Green LED
// PC.13 <--> Blue user button
#define LED_PIN    5
#define BUTTON_PIN 13

#define voltageLow 1181
#define voltageHigh 2430

#define IN1 5
#define	IN2 8
#define	IN3 6
#define	IN4 9	

volatile int ADCRes;
volatile int counter = 0;
volatile uint16_t ADC_results[4];

void configure_LED_pin(){
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

void configure_stepperMotor_pins () 
{
	//Enable the clock to GPIO Port C
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOCEN;
	
	//GPIO Mode: Input(00), Output(01), AlterFunc(10), Analog(11, reset)
	GPIOC->MODER &= ~(3UL<<(2*IN1));  
	GPIOC->MODER |=   1UL<<(2*IN1); 	//Output(01)
	
	//GPIO Mode: Input(00), Output(01), AlterFunc(10), Analog(11, reset)
	GPIOC->MODER &= ~(3UL<<(2*IN2));  
	GPIOC->MODER |=   1UL<<(2*IN2); 	//Output(01)
	
	//GPIO Mode: Input(00), Output(01), AlterFunc(10), Analog(11, reset)
	GPIOC->MODER &= ~(3UL<<(2*IN3));  
	GPIOC->MODER |=   1UL<<(2*IN3); 	//Output(01)
	
	//GPIO Mode: Input(00), Output(01), AlterFunc(10), Analog(11, reset)
	GPIOC->MODER &= ~(3UL<<(2*IN4));  
	GPIOC->MODER |=   1UL<<(2*IN4); 	//Output(01)
	
	// GPIO Speed: Low speed (00), Medium speed (01), Fast speed (10), High speed (11)
	GPIOC->OSPEEDR &= ~(3<<(2*IN1));
	GPIOC->OSPEEDR |=   2<<(2*IN1);  // Fast speed
	
	// GPIO Speed: Low speed (00), Medium speed (01), Fast speed (10), High speed (11)
	GPIOC->OSPEEDR &= ~(3<<(2*IN2));
	GPIOC->OSPEEDR |=   2<<(2*IN2);  // Fast speed
	
	// GPIO Speed: Low speed (00), Medium speed (01), Fast speed (10), High speed (11)
	GPIOC->OSPEEDR &= ~(3<<(2*IN3));
	GPIOC->OSPEEDR |=   2<<(2*IN3);  // Fast speed
	
	// GPIO Speed: Low speed (00), Medium speed (01), Fast speed (10), High speed (11)
	GPIOC->OSPEEDR &= ~(3<<(2*IN4));
	GPIOC->OSPEEDR |=   2<<(2*IN4);  // Fast speed
	
	// GPIO Output Type: Output push-pull (0, reset), Output open drain (1) 
	GPIOC->OTYPER &= ~(1<<IN1);      // Push-pull
	
	// GPIO Output Type: Output push-pull (0, reset), Output open drain (1) 
	GPIOC->OTYPER &= ~(1<<IN2);      // Push-pull
	
	// GPIO Output Type: Output push-pull (0, reset), Output open drain (1) 
	GPIOC->OTYPER &= ~(1<<IN3);      // Push-pull
	
	// GPIO Output Type: Output push-pull (0, reset), Output open drain (1) 
	GPIOC->OTYPER &= ~(1<<IN4);      // Push-pull
	
	// GPIO Push-Pull: No pull-up, pull-down (00), Pull-up (01), Pull-down (10), Reserved (11)
	GPIOC->PUPDR  &= ~(3<<(2*IN1));  // No pull-up, no pull-down
	
	// GPIO Push-Pull: No pull-up, pull-down (00), Pull-up (01), Pull-down (10), Reserved (11)
	GPIOC->PUPDR  &= ~(3<<(2*IN2));  // No pull-up, no pull-down
	
	// GPIO Push-Pull: No pull-up, pull-down (00), Pull-up (01), Pull-down (10), Reserved (11)
	GPIOC->PUPDR  &= ~(3<<(2*IN3));  // No pull-up, no pull-down
	
	// GPIO Push-Pull: No pull-up, pull-down (00), Pull-up (01), Pull-down (10), Reserved (11)
	GPIOC->PUPDR  &= ~(3<<(2*IN4));  // No pull-up, no pull-down
}

void delay(uint32_t delay)
{
	uint32_t time;
	//For delay, loop
	for (time = 0; time < delay; time++);
}

uint32_t degToStep (uint32_t deg)
{
	uint32_t steps;
	//Convert degress into steps. For 360 degress 512 steps are needed.
	steps = ((deg * 64) / 45);
	return steps;
}

void configure_A1_pin ()
{
		
	RCC->CR |= RCC_CR_HSION;
	
	while((RCC->CR & RCC_CR_HSIRDY) == 0);
	
	// GPIO Mode: Input(00), Output(01), AlterFunc(10), Analog(11, reset)
	GPIOA->MODER |= 3U<<2;
	// GPIO Push-Pull: No pull-up, pull-down (00), Pull-up (01), Pull-down (10), Reserved (11)
	GPIOA->PUPDR &= ~(3U<<2);
	// GPIO port analog switch control register (ASCR)
	// 0 = Disconnect analog switch to the ADC input (reset state)
	// 1 = Connect analog switch to the ADC input 
	GPIOA->ASCR |= 1U<<1;
}

void ADC_Trigger(void)
{
	//Software start 1 ADC Conversion
	ADC1->CR |= ADC_CR_ADSTART;
	
	while((ADC123_COMMON->CSR & ADC_CSR_EOC_MST) == 0);
}

void ADC_Wakeup()
{
	int wait_time;
	
	// To start ADC operations, the following sequence should be applied
	// DEEPPWD = e: ADC not in deep-power down
	// DEEPPWD = 1: ADC in deep-power-down (default reset state)
	if ((ADC1->CR & ADC_CR_DEEPPWD) == ADC_CR_DEEPPWD) 
	{
		// Exit deep power down mode if still in that state
		ADC1->CR &= ~ADC_CR_DEEPPWD;
	}
	
	// Enable the ADC internal voltage regulator
	// Before performing any operation such as Launching a calibration or
	// enabling the ADC, the ADC voltage regulator must first be enabled and
	// the software must wait for the regulator start-up time.
	ADC1->CR |= ADC_CR_ADVREGEN;
	
	// Wait for ADC voltage requlator start-up time. The software must wait for
	// the startup time of the ADC voltage regulator (T_ADCVREG_STUP, i.e., 20
	// us) before Launching a calibration or enabling the ADC.
	wait_time = 20 * (80000000 / 1000000);
	while (wait_time != 0) 
	{
		wait_time--;
	}
}

void configure_ADC()
{
	// Enable the ADC clock bit 
	RCC->AHB2ENR |= RCC_AHB2ENR_ADCEN;
	// Disable ADC1  
	ADC1->CR &= ~(ADC_CR_ADEN);
	// Enable the I/O analog switches voltage booster
	SYSCFG->CFGR1 |= SYSCFG_CFGR1_BOOSTEN;
	// Enable conversion of internal channels
	ADC123_COMMON->CCR |= ADC_CCR_VREFEN;
	// Configure the ADC prescaler to have the clock not divided 
	ADC123_COMMON->CCR &= ~ADC_CCR_PRESC;
	// Select synchronous clock mode 
	ADC123_COMMON->CCR &= ~(3<<16);
	ADC123_COMMON->CCR |= ADC_CCR_CKMODE;
	// Configure all ADCs as independent 
	ADC123_COMMON->CCR &= ~(ADC_CCR_DUAL);
	
	ADC_Wakeup();
	// Configure the ADC to have 12-bit resolution 
	ADC1->CFGR &= ~ADC_CFGR_RES;
	// Set right-alignment of the 12-bit result inside the result register 
	ADC1->CFGR &= ~(ADC_CFGR_ALIGN);
	// Select 1 conversion in the regular channel conversion sequence
	ADC1->SQR1 &= ~(ADC_SQR1_L);
	// Specify channel #6 as the 1st conversion
	ADC1->SQR1 &= ~ADC_SQR1_SQ1;
	ADC1->SQR1 |= (6<<6);
	// Set channel #6 to be single-ended 
	ADC1->DIFSEL &= ~ADC_DIFSEL_DIFSEL_6;
	// Select the ADC sampling time.
	ADC1->SMPR1 |= ADC_SMPR1_SMP6;
	// Select ADC to the discontinuous mode 
	ADC1->CFGR &= ~ADC_CFGR_CONT;
	// Select software trigger
	ADC1->CFGR &= ~ADC_CFGR_EXTEN;
	// Enable ADC1
	ADC1->CR |= ADC_CR_ADEN;
	// Wait until ADC1 is ready
	while ((ADC1->ISR & ADC_ISR_ADRDY) == 0);
}

void full_step (uint32_t deg, uint32_t speed)
{
	uint32_t i;
	uint32_t steps;
	//Convert degrees
	steps = degToStep(deg);
	//Loop for total steps 
	for (i = 0; i < steps; i++) 
	{
		delay(speed);
		GPIOC->ODR |= 1 << IN1;
		GPIOC->ODR &= ~(1 << IN2);
		GPIOC->ODR &= ~(1 << IN3);
		GPIOC->ODR |= 1 << IN4;
		//Step 2
		delay(speed);
		GPIOC->ODR &= ~(1 << IN1);
		GPIOC->ODR &= ~(1 << IN2);
		GPIOC->ODR |= 1 << IN3;
		GPIOC->ODR |= 1 << IN4;
		//Step 3
		delay(speed);
		GPIOC->ODR &= ~(1 << IN1);
		GPIOC->ODR |= 1 << IN2;
		GPIOC->ODR |= 1 << IN3;
		GPIOC->ODR &= ~(1 << IN4);
		//Step 4
		delay(speed);
		GPIOC->ODR |= 1 << IN1;
		GPIOC->ODR |= 1 << IN2;
		GPIOC->ODR &= ~(1 << IN3);
		GPIOC->ODR &= ~(1 << IN4);
	}
}

int main(void)
{
	configure_LED_pin();
	configure_stepperMotor_pins();
	configure_A1_pin();
	configure_ADC();
	
	// Turn on the LED
	// GPIOA->ODR |= 1 << LED_PIN;
	
	while(1)
	{
		ADC_Trigger();

		ADC1->CR |= ADC_CR_ADSTART;
		
		while((ADC123_COMMON->CSR & ADC_CSR_EOC_MST) == 0);
		
		if ((ADC1->ISR & ADC_ISR_EOC) == ADC_ISR_EOC) 
		{ 
			if(counter % 4 == 0) 
			{ 
			counter = 0; // reset counter 
			}
			ADCRes = ADC1->DR;
			ADC_results[counter] = ADC1->DR;
			int speed = ADCRes+500;
			// Rotate motor
			full_step(1, speed);
			if (ADCRes <= voltageLow)
			{
				GPIOA->ODR &= ~(1 << LED_PIN);
			} else if (ADCRes >= voltageHigh)
			{
				GPIOA->ODR |= 1 << LED_PIN;
			}
				
			counter++;
			ADC1->ISR |= ADC_ISR_EOS;
		} else if ((ADC1->ISR & ADC_ISR_JEOS) == ADC_ISR_JEOS) 
		{ 
				// For injected channels, check End of Sequence (JEOS) fLag 
				// Reading injected data registers does not clear the JEOS flag 
				// Each injected channel has a dedicated data register 
				// Injected channel 1
				ADC_results[0] = ADC1->JDR1; 
				// Injected channel 2
				ADC_results[1] = ADC1->JDR2; 
				// Injected channel 3
				ADC_results[2] = ADC1->JDR3;
				// Injected channeL 4
				ADC_results[3] = ADC1->JDR4;  
				// Clear the flab by writing 1 to it
				ADC1->ISR |= ADC_ISR_EOS; 
		}
	}
	
  // Dead loop & program hangs here
	while(1);
}
