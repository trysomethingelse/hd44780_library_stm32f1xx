# hd44780_library_stm32f1xx
Control HD44780 lcd by STM32F103C8 aka blue pill. 
## Description
The files have been created and tested in Atolic TrueStudio. Tested on blue pill with 1602A LCD display. For this moment works only in 4bit mode.
### How to use:
Create project with STM32CubeMX. Data ports (DB7,DB6,DB5,DB4) must be on same group for eg. GPIOA. The same thing applies to control ports
(RW, RS, E(CLOCK)) for eg. group GPIOB. Set all as output.

Open project in IDE. In Atolic Studio just drag header file to Inc folder, and source file to Src folder.Next you should edit hd44780.h if yours configuration is diffrent than default.

In default configuration (tested) - hd44780.h:
```
#define LCD_CONTROL_PORT GPIOB

#define LCD_RS GPIO_PIN_12
#define LCD_RW GPIO_PIN_13
#define LCD_CLOCK GPIO_PIN_14

#define LCD_DATA_PORT GPIOA

#define LCD_4 GPIO_PIN_8
#define LCD_5 GPIO_PIN_9
#define LCD_6 GPIO_PIN_10
#define LCD_7 GPIO_PIN_11
```


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
lcd_print_word("Hello world!");
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

