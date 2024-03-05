;******************** (C) Yifeng ZHU *******************************************
; @file    main.s
; @author  
; @date    Jan-17-2023
;*******************************************************************************

	INCLUDE core_cm4_constants.s		; Load Constant Definitions
	INCLUDE stm32l476xx_constants.s      

	IMPORT 	System_Clock_Init
	IMPORT 	UART2_Init
	IMPORT	USART2_Write
	
; Green LED <--> PA.5
LED_PIN	EQU	5
	
	AREA    main, CODE, READONLY
	EXPORT	__main				; make __main visible to linker
	ENTRY			
				
__main	PROC
		
	BL System_Clock_Init
	BL UART2_Init
	
	LDR r0, =str   ; First argument
	MOV r1, #11    ; Second argument
	BL USART2_Write
  
stop 	B 		stop     		; dead loop & program hangs here

	ENDP
					
	ALIGN			

	AREA    myData, DATA, READWRITE
	ALIGN
str	DCB   "123456789\r\n", 0
	END
