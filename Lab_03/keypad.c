#include "keypad.h"
#include "stm32l476xx.h"

void Keypad_Pin_Init(void) 
{
	//Enable the clock to GPIO Port C
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOCEN;
	
	//GPIO MODE: Input(00), Output(01), AlterFunc(10), Analog(11, reset)
	GPIOC->MODER &= ~(3UL<<(2*PC0));
	GPIOC->MODER |= 1UL<<(2*PC0);
	
	//GPIO MODE: Input(00), Output(01), AlterFunc(10), Analog(11, reset)
	GPIOC->MODER &= ~(3UL<<(2*PC1));
	GPIOC->MODER |= 1UL<<(2*PC1);
	
	//GPIO MODE: Input(00), Output(01), AlterFunc(10), Analog(11, reset)
	GPIOC->MODER &= ~(3UL<<(2*PC2));
	GPIOC->MODER |= 1UL<<(2*PC2);
	
	//GPIO MODE: Input(00), Output(01), AlterFunc(10), Analog(11, reset)
	GPIOC->MODER &= ~(3UL<<(2*PC3));
	GPIOC->MODER |= 1UL<<(2*PC3);
	
	//GPIO MODE: Input(00), Output(01), AlterFunc(10), Analog(11, reset)
	GPIOC->MODER &= ~(3UL<<(2*PC4));
	GPIOC->MODER |= 0UL<<(2*PC4);
	
	//GPIO MODE: Input(00), Output(01), AlterFunc(10), Analog(11, reset)
	GPIOC->MODER &= ~(3UL<<(2*PC10));
	GPIOC->MODER |= 0UL<<(2*PC10);
	
	//GPIO MODE: Input(00), Output(01), AlterFunc(10), Analog(11, reset)
	GPIOC->MODER &= ~(3UL<<(2*PC11));
	GPIOC->MODER |= 0UL<<(2*PC11);
	
	//GPIO MODE: Input(00), Output(01), AlterFunc(10), Analog(11, reset)
	GPIOC->MODER &= ~(3UL<<(2*PC12));
	GPIOC->MODER |= 0UL<<(2*PC12);
}