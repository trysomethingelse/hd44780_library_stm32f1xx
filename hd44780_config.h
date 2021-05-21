/*
 * This file can be modified to achive proper working with different hardware and connections
 *
 */

#ifndef INC_HD44780_CONFIG_H_
#define INC_HD44780_CONFIG_H_


//all lcd control ports must be in te same group eg. GPIOB
#define LCD_CONTROL_PORT LCD_RS_GPIO_Port

#define LCD_RS LCD_RS_Pin
#define LCD_RW LCD_RW_Pin
#define LCD_CLOCK LCD_CLK_Pin

//all lcd data ports must be in te same group eg. GPIOA
#define LCD_DATA_PORT LCD_4_GPIO_Port

#define LCD_4 LCD_4_Pin
#define LCD_5 LCD_5_Pin
#define LCD_6 LCD_6_Pin
#define LCD_7 LCD_7_Pin

#endif /* INC_HD44780_CONFIG_H_ */
