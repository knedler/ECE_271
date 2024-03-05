##  Lab Report ##

## Bonus Lab #1: Buzzer ##
See .png for note A scope trace of 440 Hz.

Song: Calm by C418

The `melody` array is used to define the notes and delay between each note for the song.

The `configure_BUZZER_pin()` function configures the GPIO pin connected to the buzzer to operate as a push-pull output with fast speed, no pull-up or pull-down, and an alternate function.

The `SysTick_Initialize(int ticks)` function initializes the SysTick timer, sets the reload register to the number of ticks specified as the parameter minus 1, sets the interrupt priority, sets the clock source to the system clock, enables the SysTick exception and enables the SysTick timer.

The `delay(uint32_t nTime)` function sets the `TimeDelay` variable to the value of the parameter `nTime`, which is then decremented in the SysTick interrupt handler until it reaches zero, causing the delay to end.

The `SysTick_Handler()` function decrements the `TimeDelay` variable if it is greater than zero, causing the delay to end when it reaches zero.

The `TIM2_Init()` function enables the TIMER2 clock and selects the up-counting mode.

The `playNote()` function plays a single note on a buzzer connected to pin PA.1 using pulse-width modulation (PWM). The frequency of the note is passed as an argument to the function. The function initializes TIMER2 with a frequency of 100kHz and sets the PWM mode to 1. The duty cycle is calculated based on the desired frequency and is set as the compare value. The function then waits for a short duration using the `delay` function before turning off the PWM signal and returning.

The `playSong()` function plays a melody using the `playNote` function. The notes and their durations are stored in an array `melody`. The function iterates over the array and calls `playNote` for each note with the appropriate frequency and duration. The duration is converted to a delay using the `delay` function. After playing the melody, the function turns off the buzzer.

The `main()` function configures the buzzer pin as an output using the `configure_BUZZER_pin` function. It initializes the SysTick timer with a tick interval of 1ms using the `SysTick_Initialize` function. It then calls the `playSong` function to play the melody stored in the `melody` array.