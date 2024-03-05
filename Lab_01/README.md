##  Lab Report ##

What is something cool you did in this lab?
-----------
Your answer: I was able to get the LED to blink on and off about every 1 second. The blinking is also controlled by the press of the button. After the initial start press, the button will stop and start the blinking.


Post-lab Questions
-------

* Answers to Question 1: Hardware debouncing works with the use of a capacitor to even out the signal into a smooth curve, whereas without it, the singal can "bounce" up and down causing false switching.

* Answers to Question 2: The simplest way to debounce in software is to add a slight delay between the inital button state check and a secondary check to see if the state has changed. If the state is the different then the "press" is ignored.

* Answers to Question 3: We configure the pins to no pull-up, no pull-down as the button takes care of the pull down. The button is active low meaning when pushed it sends a low signal. If we were to configure it to pull-down that would work as well but it is repetitive.

* Answers to Question 4: The "low speed" slew rate is recommended for the LED because the slower rate will reduce the high-frequency parts of the signal as well as reduce EMI in the circuit.


Suggestions and Comments
-------

* Do you have comments or suggestions for the lab and course?


* Do you have comments or suggestions for the textbook and lab handouts? Any typos or errors?



Lab Credits
-------
Did you received any help from someone other than the instructor and lab teaching assistants?

Your answer: Drew Hillard, Zach Holm, and I worked together to get our while loop counter to work.


