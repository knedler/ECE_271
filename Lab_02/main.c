#include "stm32l476xx.h"
#include "SysClock.h"

// PA.5  <--> Green LED
// PC.13 <--> Blue user button
#define LED_PIN    5
#define BUTTON_PIN 13
#define IN1 5
#define	IN2 8
#define	IN3 6
#define	IN4 9


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

void full_step (uint32_t deg, uint32_t speed)
{
	uint32_t i;
	uint32_t steps;
	//Convert degrees
	steps = degToStep(deg);
	//Loop for total steps 
	for (i = 0; i < steps; i++) 
	{
		//If button not is pressed
		if (GPIOC->IDR & GPIO_IDR_IDR_13)
		{ //Clockwise
			//Step 1
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
		} else 
		{ //Counter Clockwise
			//Step 1
			delay(speed);
			GPIOC->ODR |= 1 << IN1;
			GPIOC->ODR &= ~(1 << IN2);
			GPIOC->ODR &= ~(1 << IN3);
			GPIOC->ODR |= 1 << IN4;
			//Step 2
			delay(speed);
			GPIOC->ODR |= 1 << IN1;
			GPIOC->ODR |= 1 << IN2;
			GPIOC->ODR &= ~(1 << IN3);
			GPIOC->ODR &= ~(1 << IN4);
			//Step 3
			delay(speed);
			GPIOC->ODR &= ~(1 << IN1);
			GPIOC->ODR |= 1 << IN2;
			GPIOC->ODR |= 1 << IN3;
			GPIOC->ODR &= ~(1 << IN4);
			//Step 4
			delay(speed);
			GPIOC->ODR &= ~(1 << IN1);
			GPIOC->ODR &= ~(1 << IN2);
			GPIOC->ODR |= 1 << IN3;
			GPIOC->ODR |= 1 << IN4;
		}
	}
}

void half_step (uint32_t deg, uint32_t speed)
{
	uint32_t i;
	uint32_t steps;
	//Convert degrees
	steps = degToStep(deg);
	//Loop for steps
	for (i = 0; i < steps; i++) 
	{
		//If button is pressed
		if (GPIOC->IDR & GPIO_IDR_IDR_13)
		{ //Clockwise
			//Step 1
			delay(speed);
			GPIOC->ODR |= 1 << IN1;
			GPIOC->ODR &= ~(1 << IN2);
			GPIOC->ODR &= ~(1 << IN3);
			GPIOC->ODR |= 1 << IN4;
			//Step 2
			delay(speed);
			GPIOC->ODR &= ~(1 << IN1);
			GPIOC->ODR &= ~(1 << IN2);
			GPIOC->ODR &= ~(1 << IN3);
			GPIOC->ODR |= 1 << IN4;
			//Step 3
			delay(speed);
			GPIOC->ODR &= ~(1 << IN1);
			GPIOC->ODR &= ~(1 << IN2);
			GPIOC->ODR |= 1 << IN3;
			GPIOC->ODR |= 1 << IN4;
			//Step 4
			delay(speed);
			GPIOC->ODR &= ~(1 << IN1);
			GPIOC->ODR &= ~(1 << IN2);
			GPIOC->ODR |= 1 << IN3;
			GPIOC->ODR &= ~(1 << IN4);
			//Step 5
			delay(speed);
			GPIOC->ODR &= ~(1 << IN1);
			GPIOC->ODR |= 1 << IN2;
			GPIOC->ODR |= 1 << IN3;
			GPIOC->ODR &= ~(1 << IN4);
			//Step 6
			delay(speed);
			GPIOC->ODR &= ~(1 << IN1);
			GPIOC->ODR |= 1 << IN2;
			GPIOC->ODR &= ~(1 << IN3);
			GPIOC->ODR &= ~(1 << IN4);
			//Step 7
			delay(speed);
			GPIOC->ODR |= 1 << IN1;
			GPIOC->ODR |= 1 << IN2;
			GPIOC->ODR &= ~(1 << IN3);
			GPIOC->ODR &= ~(1 << IN4);
			//Step 8
			delay(speed);
			GPIOC->ODR |= 1 << IN1;
			GPIOC->ODR &= ~(1 << IN2);
			GPIOC->ODR &= ~(1 << IN3);
			GPIOC->ODR &= ~(1 << IN4);
		} else 
		{ //Counter Clockwise
			//Step 1
			delay(speed);
			GPIOC->ODR |= 1 << IN1;
			GPIOC->ODR &= ~(1 << IN2);
			GPIOC->ODR &= ~(1 << IN3);
			GPIOC->ODR |= 1 << IN4;
			//Step 2
			delay(speed);
			GPIOC->ODR |= 1 << IN1;
			GPIOC->ODR &= ~(1 << IN2);
			GPIOC->ODR &= ~(1 << IN3);
			GPIOC->ODR &= ~(1 << IN4);
			//Step 3
			delay(speed);
			GPIOC->ODR |= 1 << IN1;
			GPIOC->ODR |= 1 << IN2;
			GPIOC->ODR &= ~(1 << IN3);
			GPIOC->ODR &= ~(1 << IN4);
			//Step 4
			delay(speed);
			GPIOC->ODR &= ~(1 << IN1);
			GPIOC->ODR |= 1 << IN2;
			GPIOC->ODR &= ~(1 << IN3);
			GPIOC->ODR &= ~(1 << IN4);
			//Step 5
			delay(speed);
			GPIOC->ODR &= ~(1 << IN1);
			GPIOC->ODR |= 1 << IN2;
			GPIOC->ODR |= 1 << IN3;
			GPIOC->ODR &= ~(1 << IN4);
			//Step 6
			delay(speed);
			GPIOC->ODR &= ~(1 << IN1);
			GPIOC->ODR &= ~(1 << IN2);
			GPIOC->ODR |= 1 << IN3;
			GPIOC->ODR &= ~(1 << IN4);
			//Step 7
			delay(speed);
			GPIOC->ODR &= ~(1 << IN1);
			GPIOC->ODR &= ~(1 << IN2);
			GPIOC->ODR |= 1 << IN3;
			GPIOC->ODR |= 1 << IN4;
			//Step 8
			delay(speed);
			GPIOC->ODR &= ~(1 << IN1);
			GPIOC->ODR &= ~(1 << IN2);
			GPIOC->ODR &= ~(1 << IN3);
			GPIOC->ODR |= 1 << IN4;
		}
	}
}

int main(void)
{
	System_Clock_Init(); // Switch System Clock = 80 MHz
	configure_LED_pin();
	configure_button();
	configure_stepperMotor_pins();
	
	// Turn on the LED
	GPIOA->ODR |= 1 << LED_PIN;
	
	//Using full step, turn 360 degrees with speed 20000
	full_step((uint32_t) 360, (uint32_t) 20000);
	//Turn off all pins
	GPIOC->ODR &= ~(1 << IN1);
	GPIOC->ODR &= ~(1 << IN2); 
	GPIOC->ODR &= ~(1 << IN3);
	GPIOC->ODR &= ~(1 << IN4);
	//Using half step, turn 360 degrees with speed 20000
	delay((uint32_t) 10000000);
	half_step((uint32_t) 360, (uint32_t) 20000);
	//Turn off all pins
	GPIOC->ODR &= ~(1 << IN1);
	GPIOC->ODR &= ~(1 << IN2);
	GPIOC->ODR &= ~(1 << IN3);
	GPIOC->ODR &= ~(1 << IN4);
	
	while(1)
	{
		//Flip LED when done turning
		delay((uint32_t) 10000000);
		GPIOA->ODR ^= 1 << LED_PIN;
	}
	
  // Dead loop & program hangs here
	while(1);
}
