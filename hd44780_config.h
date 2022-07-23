/*
 * This file can be modified to achive proper working with different hardware and connections
 *
 */

#ifndef INC_HD44780_CONFIG_H_
#define INC_HD44780_CONFIG_H_


//all lcd control ports must be in te same group eg. GPIOB
#define LCD_CONTROL_PORT LCD_RS_GPIO_Port

#define LCD_RS      LCD_RS_Pin
#define LCD_RW      LCD_RW_Pin
#define LCD_CLOCK   LCD_CLK_Pin

#define LCD_D4_PORT LCD_D4_GPIO_Port
#define LCD_D5_PORT LCD_D5_GPIO_Port
#define LCD_D6_PORT LCD_D6_GPIO_Port
#define LCD_D7_PORT LCD_D7_GPIO_Port

#define LCD_4       LCD_D4_Pin
#define LCD_5       LCD_D5_Pin
#define LCD_6       LCD_D6_Pin
#define LCD_7       LCD_D7_Pin

#define LCD_SIGNAL_COOLDOWN_TIMEOUT 10


#endif /* INC_HD44780_CONFIG_H_ */
