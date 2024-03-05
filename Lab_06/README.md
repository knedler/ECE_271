##  Lab Report ##

What is something cool you did in this lab?
-----------
Your answer: Made the stepper motor spin 360 degrees every interupt. 


Pre-lab Questions
-------

* Answers to Question 1: Polling is very simple but wastes time and energy checking every so often. Interrupts are less simple but far better in the way that the proccess can do other things untill the interupt is called where it will then switch to handle the interupt.

* Answers to Question 2: To enable an interrupt on pin PC.13
```
EXTI->IMR1 |= EXTI_IMR1_IM13;
NVIC_EnableIRQ(EXTI15_10_IRQn);
```


* Answers to Question 3: `EXTI15_10_IRQHandler`


* Answers to Question 4: Programming SYSCFG
```
SYSCFG->EXTICR[3] &= ~SYSCFG_EXTICR4_EXTI13;
SYSCFG->EXTICR[3] |= SYSCFG_EXTICR4_EXTI13_PC;
SYSCFG->EXTICR[3] &= ~(0x000F);
```


Post-lab Questions
-------

* Answers to Question 1: The pending register (PR) is cleared by writing 1 to the bit corresponding to the EXTI that we are clearing.

* Answers to Question 2: It would cause an infinite loop as the system thinks the instruction has never been completed.

* Answers to Question 3: Yes software can set the EXTI flags.


Suggestions and Comments
-------

* Do you have comments or suggestions for the lab and course?


* Do you have comments or suggestions for the textbook and lab handouts? Any typos or errors?



Lab Credits
-------
Did you received any help from someone other than the instructor and lab teaching assistants?

Your answer: 


