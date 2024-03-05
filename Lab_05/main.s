;******************** (C) Yifeng ZHU *******************************************
; @file    main.s
; @author  Yifeng Zhu
; @date    Jan-17-2023
;*******************************************************************************

	INCLUDE core_cm4_constants.s		; Load Constant Definitions
	INCLUDE stm32l476xx_constants.s      

; Green LED <--> PA.5
LED_PIN	EQU	5
BUTTON_PIN EQU 13
IN1 EQU 5
IN2 EQU 8
IN3 EQU 6
IN4 EQU 9
	
	AREA    main, CODE, READONLY
	EXPORT	__main				; make __main visible to linker
	ENTRY			
		
__configLedPin PROC
	; Enable the clock to GPIO Port A	
	LDR r0, =RCC_BASE
	LDR r1, [r0, #RCC_AHB2ENR]
	ORR r1, r1, #RCC_AHB2ENR_GPIOAEN
	STR r1, [r0, #RCC_AHB2ENR]
	
	; MODE: 00: Input mode, 01: General purpose output mode
    ;       10: Alternate function mode, 11: Analog mode (reset state)
	LDR r0, =GPIOA_BASE
	LDR r1, [r0, #GPIO_MODER]
	BIC r1, r1, #(3<<(2*LED_PIN))
	ORR r1, r1, #(1<<(2*LED_PIN))
	STR r1, [r0, #GPIO_MODER]
	
	; GPIO Speed: Low speed (00), Medium speed (01), 
	;			  Fast speed (10), High speed (11)
	LDR r1, [r0, #GPIO_OTYPER]
	BIC r1, r1, #(3<<(2*LED_PIN))
	STR r1, [r0, #GPIO_OTYPER]
	
	; GPIO Output Type: Output push-pull (0, reset), Output open drain (1) 
	LDR r1, [r0, #GPIO_PUPDR]
	BIC r1, r1, #(3<<(2*LED_PIN))
	STR r1, [r0, #GPIO_PUPDR]
	
	; GPIO Push-Pull: No pull-up, pull-down (00), Pull-up (01), 
	;				  Pull-down (10), Reserved (11)
	LDR r1, [r0, #GPIO_ODR]
	ORR r1, r1, #(1<<LED_PIN)
	STR r1, [r0, #GPIO_ODR]

	LDR r1, [r0, #GPIO_ODR]
	ORR r1, r1, #(1<<LED_PIN)
	STR r1, [r0, #GPIO_ODR]
	
	BX LR
	ENDP
	LTORG
	
__configButton PROC
	
	; Enable the clock to GPIO Port C
	LDR r0, =RCC_BASE
	LDR r1, [r0, #RCC_AHB2ENR]
	ORR r1, r1, #RCC_AHB2ENR_GPIOCEN
	STR r1, [r0, #RCC_AHB2ENR]
	
	; MODE: 00: Input mode, 01: General purpose output mode
    ;       10: Alternate function mode, 11: Analog mode (reset state)
	LDR r0, =GPIOC_BASE
	LDR r1, [r0, #GPIO_MODER]
	BIC r1, r1, #(3<<(2*BUTTON_PIN))
	STR r1, [r0, #GPIO_MODER]
	
	; GPIO Output Type: Output push-pull (0, reset), Output open drain (1) 
	LDR r1, [r0, #GPIO_PUPDR]
	BIC r1, r1, #(3<<(2*BUTTON_PIN))
	STR r1, [r0, #GPIO_PUPDR]
	
	BX LR
	ENDP

__configMotor PROC
	
	; Enable the clock to GPIO Port C
	LDR r0, =RCC_BASE
	LDR r1, [r0, #RCC_AHB2ENR]
	ORR r1, r1, #RCC_AHB2ENR_GPIOCEN
	STR r1, [r0, #RCC_AHB2ENR]
	
	; MODE: 00: Input mode, 01: General purpose output mode
    ;       10: Alternate function mode, 11: Analog mode (reset state)
	LDR r0, =GPIOC_BASE
	LDR r1, [r0, #GPIO_MODER]
	BIC r1, r1, #(3<<(2*IN1))
	ORR r1, r1, #(1<<(2*IN1))
	STR r1, [r0, #GPIO_MODER]
	
	LDR r1, [r0, #GPIO_MODER]
	BIC r1, r1, #(3<<(2*IN2))
	ORR r1, r1, #(1<<(2*IN2))
	STR r1, [r0, #GPIO_MODER]
	
	LDR r1, [r0, #GPIO_MODER]
	BIC r1, r1, #(3<<(2*IN3))
	ORR r1, r1, #(1<<(2*IN3))
	STR r1, [r0, #GPIO_MODER]
	
	LDR r1, [r0, #GPIO_MODER]
	BIC r1, r1, #(3<<(2*IN4))
	ORR r1, r1, #(1<<(2*IN4))
	STR r1, [r0, #GPIO_MODER]
	
	; GPIO Speed: Low speed (00), Medium speed (01), 
	;			  Fast speed (10), High speed (11)
	LDR r1, [r0, #GPIO_OSPEEDR]
	BIC r1, r1, #(3<<(2*IN1))
	ORR r1, r1, #(2<<(2*IN1))
	STR r1, [r0, #GPIO_OSPEEDR]
	
	LDR r1, [r0, #GPIO_OSPEEDR]
	BIC r1, r1, #(3<<(2*IN2))
	ORR r1, r1, #(2<<(2*IN2))
	STR r1, [r0, #GPIO_OSPEEDR]
	
	LDR r1, [r0, #GPIO_OSPEEDR]
	BIC r1, r1, #(3<<(2*IN3))
	ORR r1, r1, #(2<<(2*IN3))
	STR r1, [r0, #GPIO_OSPEEDR]
	
	LDR r1, [r0, #GPIO_OSPEEDR]
	BIC r1, r1, #(3<<(2*IN4))
	ORR r1, r1, #(2<<(2*IN4))
	STR r1, [r0, #GPIO_OSPEEDR]
	
	; GPIO Output Type: Output push-pull (0, reset), Output open drain (1)
	LDR r1, [r0, #GPIO_OTYPER]
	BIC r1, r1, #(1<<IN1)
	STR r1, [r0, #GPIO_OTYPER]
	
	LDR r1, [r0, #GPIO_OTYPER]
	BIC r1, r1, #(1<<IN2)
	STR r1, [r0, #GPIO_OTYPER]
	
	LDR r1, [r0, #GPIO_OTYPER]
	BIC r1, r1, #(1<<IN3)
	STR r1, [r0, #GPIO_OTYPER]
	
	LDR r1, [r0, #GPIO_OTYPER]
	BIC r1, r1, #(1<<IN4)
	STR r1, [r0, #GPIO_OTYPER]
	
	; GPIO Push-Pull: No pull-up, pull-down (00), Pull-up (01), 
	; 				  Pull-down (10), Reserved (11)
	LDR r1, [r0, #GPIO_PUPDR]
	BIC r1, r1, #(3<<(2*IN1))
	STR r1, [r0, #GPIO_PUPDR]
	
	LDR r1, [r0, #GPIO_PUPDR]
	BIC r1, r1, #(3<<(2*IN2))
	STR r1, [r0, #GPIO_PUPDR]
	
	LDR r1, [r0, #GPIO_PUPDR]
	BIC r1, r1, #(3<<(2*IN3))
	STR r1, [r0, #GPIO_PUPDR]
	
	LDR r1, [r0, #GPIO_PUPDR]
	BIC r1, r1, #(3<<(2*IN4))
	STR r1, [r0, #GPIO_PUPDR]
	
	BX LR
	ENDP

__delay PROC ; delay function
	
	MOV r4, #0 ; i
	MOV r5, #2500
	
loop
	CMP r4, r5
	BGE endLoop ; if i >= 2500
	ADD r4, r4, #1 ; i++
	B loop
	
endLoop	
	BX LR
	ENDP

__fullStep PROC
	
	PUSH {r14}
	MOV r0, #0 ; i
	MOV r1, #512 ; steps

FSLoop
	CMP r0, r1
	BGE.W FSendLoop ; i >= steps
	ADD r0, r0, #1;
	
	; Button Pressed Check
	LDR r6, =GPIOC_BASE
	LDR r7, [r6, #GPIO_IDR]
	LDR r8, =GPIO_IDR_IDR_13
	
	AND r9, r7, r8
	CMP r9, r8
	BNE.W FSCC ; If pressed go counter clockwise
	
	; Step 1
	BL __delay
	LDR r2, =GPIOC_BASE
	LDR r3, [r2, #GPIO_ODR]
	ORR r3, r3, #(1 << IN1)
	STR r3, [r2, #GPIO_ODR]
	
	LDR r3, [r2, #GPIO_ODR]
	BIC r3, r3, #(1 << IN2)
	STR r3, [r2, #GPIO_ODR]
	
	LDR r3, [r2, #GPIO_ODR]
	BIC r3, r3, #(1 << IN3)
	STR r3, [r2, #GPIO_ODR]
	
	LDR r3, [r2, #GPIO_ODR]
	ORR r3, r3, #(1 << IN4)
	STR r3, [r2, #GPIO_ODR]
	
	; Step 2
	BL __delay
	LDR r3, [r2, #GPIO_ODR]
	BIC r3, r3, #(1 << IN1)
	STR r3, [r2, #GPIO_ODR]
	
	LDR r3, [r2, #GPIO_ODR]
	BIC r3, r3, #(1 << IN2)
	STR r3, [r2, #GPIO_ODR]
	
	LDR r3, [r2, #GPIO_ODR]
	ORR r3, r3, #(1 << IN3)
	STR r3, [r2, #GPIO_ODR]
	
	LDR r3, [r2, #GPIO_ODR]
	ORR r3, r3, #(1 << IN4)
	STR r3, [r2, #GPIO_ODR]
	
	; Step 3
	BL __delay
	LDR r3, [r2, #GPIO_ODR]
	BIC r3, r3, #(1 << IN1)
	STR r3, [r2, #GPIO_ODR]
	
	LDR r3, [r2, #GPIO_ODR]
	ORR r3, r3, #(1 << IN2)
	STR r3, [r2, #GPIO_ODR]
	
	LDR r3, [r2, #GPIO_ODR]
	ORR r3, r3, #(1 << IN3)
	STR r3, [r2, #GPIO_ODR]
	
	LDR r3, [r2, #GPIO_ODR]
	BIC r3, r3, #(1 << IN4)
	STR r3, [r2, #GPIO_ODR]
	
	; Step 4
	BL __delay
	LDR r3, [r2, #GPIO_ODR]
	ORR r3, r3, #(1 << IN1)
	STR r3, [r2, #GPIO_ODR]
	
	LDR r3, [r2, #GPIO_ODR]
	ORR r3, r3, #(1 << IN2)
	STR r3, [r2, #GPIO_ODR]
	
	LDR r3, [r2, #GPIO_ODR]
	BIC r3, r3, #(1 << IN3)
	STR r3, [r2, #GPIO_ODR]
	
	LDR r3, [r2, #GPIO_ODR]
	BIC r3, r3, #(1 << IN4)
	STR r3, [r2, #GPIO_ODR]
	
	B FSLoop

FSCC
	; Step 1
	BL __delay
	LDR r2, =GPIOC_BASE
	LDR r3, [r2, #GPIO_ODR]
	ORR r3, r3, #(1 << IN1)
	STR r3, [r2, #GPIO_ODR]
	
	LDR r3, [r2, #GPIO_ODR]
	BIC r3, r3, #(1 << IN2)
	STR r3, [r2, #GPIO_ODR]
	
	LDR r3, [r2, #GPIO_ODR]
	BIC r3, r3, #(1 << IN3)
	STR r3, [r2, #GPIO_ODR]
	
	LDR r3, [r2, #GPIO_ODR]
	ORR r3, r3, #(1 << IN4)
	STR r3, [r2, #GPIO_ODR]
	
	; Step 2
	BL __delay
	LDR r3, [r2, #GPIO_ODR]
	ORR r3, r3, #(1 << IN1)
	STR r3, [r2, #GPIO_ODR]
	
	LDR r3, [r2, #GPIO_ODR]
	ORR r3, r3, #(1 << IN2)
	STR r3, [r2, #GPIO_ODR]
	
	LDR r3, [r2, #GPIO_ODR]
	BIC r3, r3, #(1 << IN3)
	STR r3, [r2, #GPIO_ODR]
	
	LDR r3, [r2, #GPIO_ODR]
	BIC r3, r3, #(1 << IN4)
	STR r3, [r2, #GPIO_ODR]
	
	; Step 3
	BL __delay
	LDR r3, [r2, #GPIO_ODR]
	BIC r3, r3, #(1 << IN1)
	STR r3, [r2, #GPIO_ODR]
	
	LDR r3, [r2, #GPIO_ODR]
	ORR r3, r3, #(1 << IN2)
	STR r3, [r2, #GPIO_ODR]
	
	LDR r3, [r2, #GPIO_ODR]
	ORR r3, r3, #(1 << IN3)
	STR r3, [r2, #GPIO_ODR]
	
	LDR r3, [r2, #GPIO_ODR]
	BIC r3, r3, #(1 << IN4)
	STR r3, [r2, #GPIO_ODR]
	
	; Step 4
	BL __delay
	LDR r3, [r2, #GPIO_ODR]
	BIC r3, r3, #(1 << IN1)
	STR r3, [r2, #GPIO_ODR]
	
	LDR r3, [r2, #GPIO_ODR]
	BIC r3, r3, #(1 << IN2)
	STR r3, [r2, #GPIO_ODR]
	
	LDR r3, [r2, #GPIO_ODR]
	ORR r3, r3, #(1 << IN3)
	STR r3, [r2, #GPIO_ODR]
	
	LDR r3, [r2, #GPIO_ODR]
	ORR r3, r3, #(1 << IN4)
	STR r3, [r2, #GPIO_ODR]
	
	B FSLoop

FSendLoop
	POP {r14}
	BX LR
	ENDP
	LTORG
	
__halfStep PROC
	
	PUSH {r14}
	MOV r0, #0 ; i
	MOV r1, #512 ; steps

HSLoop
	CMP r0, r1
	BGE.W HSendLoop ; i >= steps
	ADD r0, r0, #1;
	
	; Button Pressed Check
	LDR r6, =GPIOC_BASE
	LDR r7, [r6, #GPIO_IDR]
	LDR r8, =GPIO_IDR_IDR_13
	
	AND r9, r7, r8
	CMP r9, r8
	BNE.W HSCC ; If pressed go counter clockwise
	
	; Step 1
	BL __delay
	LDR r2, =GPIOC_BASE
	LDR r3, [r2, #GPIO_ODR]
	ORR r3, r3, #(1 << IN1)
	STR r3, [r2, #GPIO_ODR]
	
	LDR r3, [r2, #GPIO_ODR]
	BIC r3, r3, #(1 << IN2)
	STR r3, [r2, #GPIO_ODR]
	
	LDR r3, [r2, #GPIO_ODR]
	BIC r3, r3, #(1 << IN3)
	STR r3, [r2, #GPIO_ODR]
	
	LDR r3, [r2, #GPIO_ODR]
	ORR r3, r3, #(1 << IN4)
	STR r3, [r2, #GPIO_ODR]
	
	; Step 2
	BL __delay
	LDR r3, [r2, #GPIO_ODR]
	BIC r3, r3, #(1 << IN1)
	STR r3, [r2, #GPIO_ODR]
	
	LDR r3, [r2, #GPIO_ODR]
	BIC r3, r3, #(1 << IN2)
	STR r3, [r2, #GPIO_ODR]
	
	LDR r3, [r2, #GPIO_ODR]
	BIC r3, r3, #(1 << IN3)
	STR r3, [r2, #GPIO_ODR]
	
	LDR r3, [r2, #GPIO_ODR]
	ORR r3, r3, #(1 << IN4)
	STR r3, [r2, #GPIO_ODR]
	
	; Step 3
	BL __delay
	LDR r3, [r2, #GPIO_ODR]
	BIC r3, r3, #(1 << IN1)
	STR r3, [r2, #GPIO_ODR]
	
	LDR r3, [r2, #GPIO_ODR]
	BIC r3, r3, #(1 << IN2)
	STR r3, [r2, #GPIO_ODR]
	
	LDR r3, [r2, #GPIO_ODR]
	ORR r3, r3, #(1 << IN3)
	STR r3, [r2, #GPIO_ODR]
	
	LDR r3, [r2, #GPIO_ODR]
	ORR r3, r3, #(1 << IN4)
	STR r3, [r2, #GPIO_ODR]
	
	; Step 4
	BL __delay
	LDR r3, [r2, #GPIO_ODR]
	BIC r3, r3, #(1 << IN1)
	STR r3, [r2, #GPIO_ODR]
	
	LDR r3, [r2, #GPIO_ODR]
	BIC r3, r3, #(1 << IN2)
	STR r3, [r2, #GPIO_ODR]
	
	LDR r3, [r2, #GPIO_ODR]
	ORR r3, r3, #(1 << IN3)
	STR r3, [r2, #GPIO_ODR]
	
	LDR r3, [r2, #GPIO_ODR]
	BIC r3, r3, #(1 << IN4)
	STR r3, [r2, #GPIO_ODR]
	
	; Step 5
	BL __delay
	LDR r3, [r2, #GPIO_ODR]
	BIC r3, r3, #(1 << IN1)
	STR r3, [r2, #GPIO_ODR]
	
	LDR r3, [r2, #GPIO_ODR]
	ORR r3, r3, #(1 << IN2)
	STR r3, [r2, #GPIO_ODR]
	
	LDR r3, [r2, #GPIO_ODR]
	ORR r3, r3, #(1 << IN3)
	STR r3, [r2, #GPIO_ODR]
	
	LDR r3, [r2, #GPIO_ODR]
	BIC r3, r3, #(1 << IN4)
	STR r3, [r2, #GPIO_ODR]
	
	; Step 6
	BL __delay
	LDR r3, [r2, #GPIO_ODR]
	BIC r3, r3, #(1 << IN1)
	STR r3, [r2, #GPIO_ODR]
	
	LDR r3, [r2, #GPIO_ODR]
	ORR r3, r3, #(1 << IN2)
	STR r3, [r2, #GPIO_ODR]
	
	LDR r3, [r2, #GPIO_ODR]
	BIC r3, r3, #(1 << IN3)
	STR r3, [r2, #GPIO_ODR]
	
	LDR r3, [r2, #GPIO_ODR]
	BIC r3, r3, #(1 << IN4)
	STR r3, [r2, #GPIO_ODR]
	
	; Step 7
	BL __delay
	LDR r3, [r2, #GPIO_ODR]
	ORR r3, r3, #(1 << IN1)
	STR r3, [r2, #GPIO_ODR]
	
	LDR r3, [r2, #GPIO_ODR]
	ORR r3, r3, #(1 << IN2)
	STR r3, [r2, #GPIO_ODR]
	
	LDR r3, [r2, #GPIO_ODR]
	BIC r3, r3, #(1 << IN3)
	STR r3, [r2, #GPIO_ODR]
	
	LDR r3, [r2, #GPIO_ODR]
	BIC r3, r3, #(1 << IN4)
	STR r3, [r2, #GPIO_ODR]
	
	; Step 8
	BL __delay
	LDR r3, [r2, #GPIO_ODR]
	ORR r3, r3, #(1 << IN1)
	STR r3, [r2, #GPIO_ODR]
	
	LDR r3, [r2, #GPIO_ODR]
	BIC r3, r3, #(1 << IN2)
	STR r3, [r2, #GPIO_ODR]
	
	LDR r3, [r2, #GPIO_ODR]
	BIC r3, r3, #(1 << IN3)
	STR r3, [r2, #GPIO_ODR]
	
	LDR r3, [r2, #GPIO_ODR]
	BIC r3, r3, #(1 << IN4)
	STR r3, [r2, #GPIO_ODR]
	
	B HSLoop
	
HSCC
	; Step 1
	BL __delay
	LDR r2, =GPIOC_BASE
	LDR r3, [r2, #GPIO_ODR]
	ORR r3, r3, #(1 << IN1)
	STR r3, [r2, #GPIO_ODR]
	
	LDR r3, [r2, #GPIO_ODR]
	BIC r3, r3, #(1 << IN2)
	STR r3, [r2, #GPIO_ODR]
	
	LDR r3, [r2, #GPIO_ODR]
	BIC r3, r3, #(1 << IN3)
	STR r3, [r2, #GPIO_ODR]
	
	LDR r3, [r2, #GPIO_ODR]
	ORR r3, r3, #(1 << IN4)
	STR r3, [r2, #GPIO_ODR]
	
	; Step 2
	BL __delay
	LDR r3, [r2, #GPIO_ODR]
	ORR r3, r3, #(1 << IN1)
	STR r3, [r2, #GPIO_ODR]
	
	LDR r3, [r2, #GPIO_ODR]
	BIC r3, r3, #(1 << IN2)
	STR r3, [r2, #GPIO_ODR]
	
	LDR r3, [r2, #GPIO_ODR]
	BIC r3, r3, #(1 << IN3)
	STR r3, [r2, #GPIO_ODR]
	
	LDR r3, [r2, #GPIO_ODR]
	BIC r3, r3, #(1 << IN4)
	STR r3, [r2, #GPIO_ODR]
	
	; Step 3
	BL __delay
	LDR r3, [r2, #GPIO_ODR]
	ORR r3, r3, #(1 << IN1)
	STR r3, [r2, #GPIO_ODR]
	
	LDR r3, [r2, #GPIO_ODR]
	ORR r3, r3, #(1 << IN2)
	STR r3, [r2, #GPIO_ODR]
	
	LDR r3, [r2, #GPIO_ODR]
	BIC r3, r3, #(1 << IN3)
	STR r3, [r2, #GPIO_ODR]
	
	LDR r3, [r2, #GPIO_ODR]
	BIC r3, r3, #(1 << IN4)
	STR r3, [r2, #GPIO_ODR]
	
	; Step 4
	BL __delay
	LDR r3, [r2, #GPIO_ODR]
	BIC r3, r3, #(1 << IN1)
	STR r3, [r2, #GPIO_ODR]
	
	LDR r3, [r2, #GPIO_ODR]
	ORR r3, r3, #(1 << IN2)
	STR r3, [r2, #GPIO_ODR]
	
	LDR r3, [r2, #GPIO_ODR]
	BIC r3, r3, #(1 << IN3)
	STR r3, [r2, #GPIO_ODR]
	
	LDR r3, [r2, #GPIO_ODR]
	BIC r3, r3, #(1 << IN4)
	STR r3, [r2, #GPIO_ODR]
	
	; Step 5
	BL __delay
	LDR r3, [r2, #GPIO_ODR]
	BIC r3, r3, #(1 << IN1)
	STR r3, [r2, #GPIO_ODR]
	
	LDR r3, [r2, #GPIO_ODR]
	ORR r3, r3, #(1 << IN2)
	STR r3, [r2, #GPIO_ODR]
	
	LDR r3, [r2, #GPIO_ODR]
	ORR r3, r3, #(1 << IN3)
	STR r3, [r2, #GPIO_ODR]
	
	LDR r3, [r2, #GPIO_ODR]
	BIC r3, r3, #(1 << IN4)
	STR r3, [r2, #GPIO_ODR]
	
	; Step 6
	BL __delay
	LDR r3, [r2, #GPIO_ODR]
	BIC r3, r3, #(1 << IN1)
	STR r3, [r2, #GPIO_ODR]
	
	LDR r3, [r2, #GPIO_ODR]
	BIC r3, r3, #(1 << IN2)
	STR r3, [r2, #GPIO_ODR]
	
	LDR r3, [r2, #GPIO_ODR]
	ORR r3, r3, #(1 << IN3)
	STR r3, [r2, #GPIO_ODR]
	
	LDR r3, [r2, #GPIO_ODR]
	BIC r3, r3, #(1 << IN4)
	STR r3, [r2, #GPIO_ODR]
	
	; Step 7
	BL __delay
	LDR r3, [r2, #GPIO_ODR]
	BIC r3, r3, #(1 << IN1)
	STR r3, [r2, #GPIO_ODR]
	
	LDR r3, [r2, #GPIO_ODR]
	BIC r3, r3, #(1 << IN2)
	STR r3, [r2, #GPIO_ODR]
	
	LDR r3, [r2, #GPIO_ODR]
	ORR r3, r3, #(1 << IN3)
	STR r3, [r2, #GPIO_ODR]
	
	LDR r3, [r2, #GPIO_ODR]
	ORR r3, r3, #(1 << IN4)
	STR r3, [r2, #GPIO_ODR]
	
	; Step 8
	BL __delay
	LDR r3, [r2, #GPIO_ODR]
	BIC r3, r3, #(1 << IN1)
	STR r3, [r2, #GPIO_ODR]
	
	LDR r3, [r2, #GPIO_ODR]
	BIC r3, r3, #(1 << IN2)
	STR r3, [r2, #GPIO_ODR]
	
	LDR r3, [r2, #GPIO_ODR]
	BIC r3, r3, #(1 << IN3)
	STR r3, [r2, #GPIO_ODR]
	
	LDR r3, [r2, #GPIO_ODR]
	ORR r3, r3, #(1 << IN4)
	STR r3, [r2, #GPIO_ODR]
	
	B HSLoop

HSendLoop
	POP {r14}
	BX LR
	ENDP
	LTORG

__disablePins PROC
	
	; Turn off pins
	LDR r0, =GPIOC_BASE
	LDR r1, [r0, #GPIO_ODR]
	BIC r1, r1, #(1 << IN1)
	STR r1, [r0, #GPIO_ODR]
	
	LDR r1, [r0, #GPIO_ODR]
	BIC r1, r1, #(1 << IN2)
	STR r1, [r0, #GPIO_ODR]
	
	LDR r1, [r0, #GPIO_ODR]
	BIC r1, r1, #(1 << IN3)
	STR r1, [r0, #GPIO_ODR]
	
	LDR r1, [r0, #GPIO_ODR]
	BIC r1, r1, #(1 << IN4)
	STR r1, [r0, #GPIO_ODR]
	
	BX LR
	ENDP

__main	PROC
	
	BL __configLedPin
	BL __configButton
	BL __configMotor

	BL __fullStep
	BL __disablePins
	
	BL __halfStep
	BL __disablePins
	
	; Toggle LED off when done turning	
	LDR r0, =GPIOA_BASE
	LDR r1, [r0, #GPIO_ODR]
	BIC r1, r1, #(1<<LED_PIN)
	STR r1, [r0, #GPIO_ODR]	
	
stop 	B 		stop     		; dead loop & program hangs here

	ENDP
					
	ALIGN			

	AREA    myData, DATA, READWRITE
	ALIGN
array	DCD   1, 2, 3, 4
	END