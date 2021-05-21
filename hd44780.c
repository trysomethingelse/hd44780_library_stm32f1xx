#include "hd44780.h"

static void lcd_write_nibble_and_clock(uint8_t command, uint8_t start_bit, uint8_t rs);
static void lcd_write_nibble(uint8_t command, uint8_t start_bit, uint8_t rs);
static void lcd_clock(void);
static uint8_t busy_flag_check(void);
static uint8_t get_bit(uint8_t command, uint8_t no);

uint8_t lcd_write(uint8_t command)
{
	if (busy_flag_check() == BUSY)
		return COMMAND_BUSY_ERROR;
	lcd_write_nibble_and_clock(command, MSB, 0);
	lcd_write_nibble_and_clock(command, LSB, 0);
	return COMMAND_SUCCESS;
}

uint8_t lcd_print(char word[])
{
	for(int i = 0; word[i] != 0; i++)
	{
		if(lcd_print_letter(word[i]) == COMMAND_BUSY_ERROR) {
			return COMMAND_BUSY_ERROR;
		}
	}
	return COMMAND_SUCCESS;
}

uint8_t lcd_print_int(int number)
{
	char word[sizeof(int)];
	sprintf(word,"%d",number);
	return lcd_print(word);
}

uint8_t lcd_print_double(double number, uint8_t precision)
{
	char word[sizeof(int)];
	sprintf(word, "%.*lf", precision, number);
	return lcd_print(word);
}

uint8_t lcd_print_letter(uint8_t letter)
{

	if (busy_flag_check() == BUSY) {
		return COMMAND_BUSY_ERROR;
	}
	lcd_write_nibble_and_clock(letter, MSB, 1);
	lcd_write_nibble_and_clock(letter, LSB, 1);
	return COMMAND_SUCCESS;
}

void lcd_init()
{
	uint16_t timeout = 0;

	while(busy_flag_check() == BUSY &&
		  timeout 			<  INIT_TIMEOUT_MS)
	{
		HAL_Delay(1);
		timeout++;
	}
	lcd_write_nibble_and_clock(SET_4BIT_OPERATION,MSB,0);
	lcd_write(TWO_LINES_4BIT);
	lcd_write(DISPLAY_ON_OFF);
	lcd_write(ENTRY_MODE_INCREMENT | DISPLAY_SHIFT_OFF);
	lcd_write(DISPLAY_CLEAR);
}

void lcd_dummy_test(uint16_t delay_ms)
{
	//set default states on lcd pins (low, high, low, high pattern):
	HAL_GPIO_WritePin(LCD_CONTROL_PORT, LCD_RS, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(LCD_CONTROL_PORT, LCD_RW, GPIO_PIN_SET);
	HAL_GPIO_WritePin(LCD_CONTROL_PORT, LCD_CLOCK, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(LCD_DATA_PORT, LCD_4, GPIO_PIN_SET);
	HAL_GPIO_WritePin(LCD_DATA_PORT, LCD_5, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(LCD_DATA_PORT, LCD_6, GPIO_PIN_SET);
	HAL_GPIO_WritePin(LCD_DATA_PORT, LCD_7, GPIO_PIN_RESET);

	while(1) {
		HAL_GPIO_TogglePin(LCD_CONTROL_PORT, LCD_RS);
		HAL_GPIO_TogglePin(LCD_CONTROL_PORT, LCD_RW);
		HAL_GPIO_TogglePin(LCD_CONTROL_PORT, LCD_CLOCK);
		HAL_GPIO_TogglePin(LCD_DATA_PORT, LCD_4);
		HAL_GPIO_TogglePin(LCD_DATA_PORT, LCD_5);
		HAL_GPIO_TogglePin(LCD_DATA_PORT, LCD_6);
		HAL_GPIO_TogglePin(LCD_DATA_PORT, LCD_7);
		HAL_Delay(delay_ms);
	}
}

static void lcd_write_nibble_and_clock(uint8_t command, uint8_t start_bit, uint8_t rs)
{
	lcd_write_nibble(command, start_bit, rs);
	lcd_clock();
}

static void lcd_write_nibble(uint8_t command, uint8_t start_bit, uint8_t rs)
{
	uint32_t bsrr = 0;

	if (get_bit(command, start_bit)) {
		bsrr |= LCD_7; //set one
	}
	else {
		bsrr |= (uint32_t)LCD_7<<16U;  //reset ODR by 16 times shifted current place
	}

	if (get_bit(command, start_bit-1)) {
		bsrr |= LCD_6; //set one
	}
	else {
		bsrr |= (uint32_t)LCD_6<<16U;  //reset ODR by 16 times shifted current place
	}

	if (get_bit(command, start_bit-2)) {
		bsrr |= LCD_5; //set one
	}
	else {
		bsrr |= (uint32_t)LCD_5<<16U;  //reset ODR by 16 times shifted current place
	}

	if (get_bit(command, start_bit-3)) {
		bsrr |= LCD_4; //set one
	}
	else {
		bsrr |= (uint32_t)LCD_4<<16U;  //reset ODR by 16 times shifted current place
	}

	LCD_DATA_PORT->BSRR = bsrr;

	bsrr = 0;
	bsrr |= (uint32_t)LCD_RW<<16U; //set zero on RW
	if (rs) {
		bsrr |= LCD_RS; //set one
	}
	else {
		bsrr |= LCD_RS<<16U;  //reset ODR by 16 times shifted current place
	}
	LCD_CONTROL_PORT->BSRR = bsrr;
}

static void lcd_clock(void)
{
	HAL_GPIO_WritePin(LCD_CONTROL_PORT, LCD_CLOCK,GPIO_PIN_SET);
	HAL_Delay(1);
	HAL_GPIO_WritePin(LCD_CONTROL_PORT, LCD_CLOCK,GPIO_PIN_RESET);
	HAL_Delay(1);
}

static uint8_t busy_flag_check(void)
{
	uint8_t out = BUSY;

	//must to set all data lines as input
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.Pin = LCD_4567;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	HAL_GPIO_Init(LCD_DATA_PORT, &GPIO_InitStruct);
	HAL_GPIO_WritePin(LCD_CONTROL_PORT,LCD_RW,GPIO_PIN_SET);//read from lcd
	HAL_GPIO_WritePin(LCD_DATA_PORT,LCD_RS,GPIO_PIN_RESET);
	HAL_GPIO_WritePin(LCD_CONTROL_PORT,LCD_CLOCK,GPIO_PIN_SET);

	for(int i = NO_BUSY_CHECK; i >  0; i-- )
	{
		if(HAL_GPIO_ReadPin(LCD_DATA_PORT,LCD_7) == NOT_BUSY)
		{
			out = NOT_BUSY;
			break;
		}
	}

	//set data lines as output
	GPIO_InitStruct.Pin = LCD_4567;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(LCD_DATA_PORT, &GPIO_InitStruct);
	HAL_GPIO_WritePin(LCD_CONTROL_PORT,LCD_RW,GPIO_PIN_RESET);//write to lcd

	return out;
}

static uint8_t get_bit(uint8_t command, uint8_t no)
{
	return ((command >> no) & 1);
}
