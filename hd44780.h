#ifndef HD44780_H

#include "hd44780_config.h"
#include "main.h"
#include "stm32f1xx_hal.h"
#include "stdio.h"
#include "string.h"

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
#define INIT_TIMEOUT_MS	1000

#define MSB 7
#define LSB 3

/*
 *  @brief Inits lcd to proper state, sets 4 bit mode, clears display
 *  @param command
 */
void lcd_init(void);

/*
 *  @brief Sends command to lcd
 *  @param command
 *	@retval COMMAND_SUCCESS
 *		    COMMAND_BUSY_ERROR
 */
uint8_t lcd_write(uint8_t command);

/*
 *  @brief Prints one letter on lcd
 *  @param letter
 *	@retval COMMAND_SUCCESS
 *		    COMMAND_BUSY_ERROR
 */
uint8_t lcd_print_letter(uint8_t letter);

/*
 *  @brief Prints array of characters on lcd
 *  @param word pointer to beggining of array characters
 *	@retval COMMAND_SUCCESS
 *		    COMMAND_BUSY_ERROR
 */
uint8_t lcd_print(char word[]);

/*
 *  @brief Prints integer number on lcd
 *  @param number Integer number to print
 *	@retval COMMAND_SUCCESS
 *  		COMMAND_BUSY_ERROR
 */
uint8_t lcd_print_int(int number);

/*
 *  @brief Prints floating point number on lcd
 *  @param number Floating point number to print
 *	@param precision Number of digits after point
 *	@retval COMMAND_SUCCESS
 *			COMMAND_BUSY_ERROR
 */
uint8_t lcd_print_double(double number, uint8_t precision);

/*
 *  @brief Executes dummy test on lcd GPIOs.
 *  @note Test consists of setting GPIOs in default state in
 *  (low, high) pattern and next toggling pins. Test can be run wihtout lcd_init().
 *  Warning! Test runs in infinite loop.
 *  @param delay_ms Delay between state changes of GPIOs
 */
void lcd_dummy_test(uint16_t delay_ms);

#endif  // HD44780_H


