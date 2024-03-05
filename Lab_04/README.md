##  Lab Report ##

What is something cool you did in this lab?
-----------
Your answer: Made the led pause blinking when button is pressed once. Made it resume when it was pressed again.


Post-lab Questions
-------

* Answers to Question 1: MOV moves the bytes of the address/value to the address. It does not changed the address itself.

* Answers to Question 2: You would have to resize the constant into multiple ORR actions. i.e. it could be broken into the top half and bottom half and then each half could be ORRed on there own and then used later in the code.

* Answers to Question 3: CMP works the same as SUB but CMP does not save the value where SUB does.

* Answers to Question 4: Yes we can, you would load GPIOA_BASE to r0, load GPIO_ODR onto the GPIOA_BASE and store to r1, and then `BIC r1, r1, #(1<<LED_PIN)`


Lab Demo Questions
-------

* Why did we configure the pin that drives the LED as push-pull instead of open-drain? Open drain never supplies the LED with power.


* What is GPIO output speed? What is the default speed? Did you notice any difference if different speeds were used in this lab assignment? GPIO speed is the speed as to witch the signa rises or falls. Faster the speed the faster the switch. The defualt speed is low. I did not notice a difference.


Suggestions and Comments
-------

* Do you have comments or suggestions for the lab and course?


* Do you have comments or suggestions for the textbook and lab handouts? Any typos or errors?



Lab Credits
-------
Did you received any help from someone other than the instructor and lab teaching assistants?

Your answer: 


