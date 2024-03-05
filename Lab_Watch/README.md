##  Lab Report ##

## Bonus Lab #1: Analog Watch ##

The main functions of this code are `LCD_RTC_Clock_Enable`, `RTC_Init`, `drawClock`, `drawHands`, and `main`.

`LCD_RTC_Clock_Enable` sets up the clock using the RTC (real-time clock) of the STM32L476 board. It first enables the power interface clock and selects the LSE (low-speed external) oscillator as the RTC clock source. It then resets the LSEON and LSEBYP bits before configuring LSE and selects LSE as the RTC clock source. Finally, it disables the power interface clock and enables the RTC clock.

`RTC_Init` initializes the RTC registers. It enables the RTC clock, disables the write protection of RTC registers, enters initialization mode, sets the hour format, generates a 1 Hz clock for the RTC time counter, sets the time and date, and exits the initialization mode.

`drawClock` draws a clock on the SSD1306 OLED display. It fills the display with white, draws the clock shape (a circle), and writes the numbers 3, 6, 9, and 12 on the clock face.

`drawHands()` retrieves the current time from a Real Time Clock (RTC) and uses the hour, minute, and second values to draw clock hands on an SSD1306 OLED display. The hour hand is drawn using a 16 pixel radius, the minute hand is drawn using a 24 pixel radius, and the second hand is drawn using a 32 pixel radius.

`main`  initializes the RTC and the I2C communication and configures the SSD1306 OLED display. It then enters an infinite loop, where it gets the current time from the RTC, calls the `drawHands` function to draw the hands on the display, and updates the display with the new information using the `ssd1306_UpdateScreen` function.