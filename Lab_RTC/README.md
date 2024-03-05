##  Lab Report ##

## Bonus Lab #5: RTC ##

The code implements a function called `LCD_RTC_Clock_Enable()`, which is responsible for enabling the Real-Time Clock (RTC) and configuring the low-speed external oscillator (LSE) as its clock source. This function first enables the power interface clock and then sets the LSE as the RTC clock source.

Another function implemented is `RTC_Init()`, which initializes the RTC registers to set the current date and time. It enters initialization mode, sets the clock format to 24-hour mode, sets the prescaler values for generating a 1 Hz clock, sets the current time and date, and exits the initialization mode.

The `main` function calls the `System_Clock_Init()` to set the system clock to 80 MHz, initializes the I2C interface and OLED display by calling `I2C_GPIO_init()`, `I2C_Initialization(I2C1)`, and `ssd1306_Init()`, respectively. Then, it calls the `RTC_Init()` to initialize the RTC. In an infinite loop, it reads the time and date from the RTC and formats them into two strings using the `sprintf` function. These strings can then be printed to the OLED display.
