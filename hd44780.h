#ifndef HD44780_H

#include "stm32f1xx_hal.h"
#include "stdio.h"
#include "string.h"

//all lcd control ports must be in te same group eg. GPIOB
#define LCD_CONTROL_PORT GPIOB

#define LCD_RS GPIO_PIN_12
#define LCD_RW GPIO_PIN_13
#define LCD_CLOCK GPIO_PIN_14

//all lcd data ports must be in te same group eg. GPIOA
#define LCD_DATA_PORT GPIOA

#define LCD_4 GPIO_PIN_8
#define LCD_5 GPIO_PIN_9
#define LCD_6 GPIO_PIN_10
#define LCD_7 GPIO_PIN_11


#define LCD_4567 LCD_4|LCD_5|LCD_6|LCD_7
#define BUSY_FLAG LCD_7

#define ON 1
#define OFF 0
#define BUSY 1
#define NOT_BUSY 0
#define NO_BUSY_CHECK 10

#define COMMAND_BUSY_ERROR 1
#define COMMAND_SUCCESS 0

#define DISPLAY_CLEAR 0x01
#define DISPLAY_HOME 0x02
#define SET_4BIT_OPERATION 0x20
#define TWO_LINES_4BIT 0x28
#define DISPLAY_ON_OFF 0x0E

#define DISPLAY_SHIFT_OFF 0x04
#define DISPLAY_SHIFT_ON 0x05
#define ENTRY_MODE_INCREMENT 0x06
#define ENTRY_MODE_DECREMENT 0x04

#define DISPLAY_SHIFT 0x18
#define CURSOR_SHIFT 0x10
#define SHIFT_LEFT 0x10
#define SHIFT_RIGHT 0x14

#define SET_SECOND_LINE 0xC0

#define MSB 7
#define LSB 3

/**
  * @brief Check busy flag
  * @note


  * @retval eror
  */
uint8_t busy_flag_check(void);

void lcd_clock(void);
void lcd_init(void);
void lcd_write_nibble(uint8_t, uint8_t, uint8_t);
uint8_t lcd_write(uint8_t);
uint8_t lcd_print_letter(uint8_t);
uint8_t get_bit(uint8_t, uint8_t);
uint8_t lcd_print(char []);
uint8_t lcd_print_int(int);
uint8_t lcd_print_double(double number,uint8_t precision);

#endif  // HD44780_H


