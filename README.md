# HD44780 library for STM32
Control HD44780 lcd by STM32F1xx, should work also for other uC.
![obraz](https://user-images.githubusercontent.com/12532083/180615831-843eef88-5132-4c97-873a-9db40f745951.png)

## Description
The files have been created and tested in Atolic TrueStudio. Tested on blue pill with 1602A LCD display. For this moment works only in 4bit mode.
### How to use:
Create project with STM32CubeMX. Data ports (DB7,DB6,DB5,DB4) can be configured for any GPIO port. Control ports need to be in one group
(RW, RS, E(CLOCK)) for eg. group GPIOB. Set all as output. Modify hd44780_config.h respectively to your configuration.

Open project in IDE. In Atolic Studio just drag header file to Inc folder, and source file to Src folder. Next you should edit hd44780_config.h if yours configuration is diffrent than default.

LCD_SIGNAL_COOLDOWN_TIMEOUT should be changed for different value if driver doesn't work properly for you. Delay function is made based on CPU freqency. Tests were made with 72 MHz clock. 

Add header file to main.c with:
```
#include "hd44780.h"
```

Initialize lcd by :
```
lcd_init();
```
### Functions:
Print letter:
```
lcd_print_letter('a');
```
Print word:
```
lcd_print("  hello,world!");
```
Print integer:
```
lcd_print_int(42);
```
Print double with precision of n:
```
lcd_print_double(1.23,n);
```
Clear display:
```
lcd_write(DISPLAY_CLEAR);
```
Home display:
```
lcd_write(DISPLAY_HOME);
```
Go to second line:
```
lcd_write(SET_SECOND_LINE);
```

