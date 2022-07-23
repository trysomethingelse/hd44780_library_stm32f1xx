#include "hd44780.h"

static void lcd_write_nibble_and_clock (uint8_t command, uint8_t start_bit, uint8_t rs);
static void lcd_write_nibble (uint8_t command, uint8_t start_bit, uint8_t rs);
static void lcd_clock (void);
static uint8_t busy_flag_check (void);
static uint8_t get_bit (uint8_t command, uint8_t no);
static uint8_t getCgramAddr (uint8_t customCharacterNb);
static void lcd_wait (uint64_t LimitTime);

uint8_t lcd_write (uint8_t command)
{
  if  (busy_flag_check () == BUSY)
    return COMMAND_BUSY_ERROR;
  lcd_write_nibble_and_clock (command, MSB, 0);
  lcd_write_nibble_and_clock (command, LSB, 0);
  return COMMAND_SUCCESS;
}

uint8_t lcd_print (char word[])
{
  char letter = 0;
  for (int i = 0; word[i] != 0; i++)
  {
    letter = word[i];

    if (letter == '\n') {
      lcd_write (SET_SECOND_LINE);
    }
    else {
      if (lcd_print_letter (letter) == COMMAND_BUSY_ERROR) {
        return COMMAND_BUSY_ERROR;
      }
    }
  }
  return COMMAND_SUCCESS;
}

uint8_t lcd_print_int (int number)
{
  char word[sizeof (int)];
  sprintf (word,"%d",number);
  return lcd_print (word);
}

uint8_t lcd_print_double (double number, uint8_t precision)
{
  // FIXME: make lightweight printing
//	char word[100]; //should be changed to dynamic
//	sprintf (word, "%.*lf", precision, number);
//	return lcd_print (word);
  return COMMAND_UNSUPPORTED;
}

uint8_t lcd_print_letter (uint8_t letter)
{
  if (busy_flag_check () == BUSY) {
    return COMMAND_BUSY_ERROR;
  }
  lcd_write_nibble_and_clock (letter, MSB, 1);
  lcd_write_nibble_and_clock (letter, LSB, 1);
  return COMMAND_SUCCESS;
}

void lcd_init ()
{
  HAL_Delay(15);
  lcd_write_nibble_and_clock (FUNCTION_SET, MSB, 0);
  HAL_Delay(5);
  lcd_write_nibble_and_clock (FUNCTION_SET, MSB, 0);
  HAL_Delay(1);
  lcd_write_nibble_and_clock (FUNCTION_SET, MSB, 0);

  lcd_write (TWO_LINES_4BIT);
  lcd_write (DISPLAY_ON);
  lcd_write (ENTRY_MODE_INCREMENT | DISPLAY_SHIFT_OFF);
  lcd_write (DISPLAY_CLEAR);
}

void lcd_dummy_test (uint16_t delay_ms)
{
  //set default states on lcd pins  (low, high, low, high pattern):
  HAL_GPIO_WritePin (LCD_CONTROL_PORT, LCD_RS, GPIO_PIN_RESET);
  HAL_GPIO_WritePin (LCD_CONTROL_PORT, LCD_RW, GPIO_PIN_SET);
  HAL_GPIO_WritePin (LCD_CONTROL_PORT, LCD_CLOCK, GPIO_PIN_RESET);
  HAL_GPIO_WritePin (LCD_D4_PORT, LCD_4, GPIO_PIN_SET);
  HAL_GPIO_WritePin (LCD_D5_PORT, LCD_5, GPIO_PIN_RESET);
  HAL_GPIO_WritePin (LCD_D6_PORT, LCD_6, GPIO_PIN_SET);
  HAL_GPIO_WritePin (LCD_D7_PORT, LCD_7, GPIO_PIN_RESET);

  while (1) {
    HAL_GPIO_TogglePin (LCD_CONTROL_PORT, LCD_RS);
    HAL_GPIO_TogglePin (LCD_CONTROL_PORT, LCD_RW);
    HAL_GPIO_TogglePin (LCD_CONTROL_PORT, LCD_CLOCK);
    HAL_GPIO_TogglePin (LCD_D4_PORT, LCD_4);
    HAL_GPIO_TogglePin (LCD_D5_PORT, LCD_5);
    HAL_GPIO_TogglePin (LCD_D6_PORT, LCD_6);
    HAL_GPIO_TogglePin (LCD_D7_PORT, LCD_7);
    lcd_wait (1);
  }
}

uint8_t setCustomCharacter (const char customChar[8], uint8_t characterNb)
{
  uint8_t result = COMMAND_SUCCESS;

  lcd_write (getCgramAddr (characterNb)); //enter to CGRAM

  //and send custom character:
  for (uint8_t i = 0; i < 8; i++)
  {
    result = lcd_print_letter (customChar[i]);
    if (result != COMMAND_SUCCESS) {
      return result;
    }
  }
  return result;
}

/*
 *  @brief Returns CGRAM address according to given number
 *  @param customCharacterNb Number of custom character which address will be returned
 *  @retval Address
 */
static uint8_t getCgramAddr (uint8_t customCharacterNb)
{
  uint8_t nb = customCharacterNb;
  if (nb >= 8) {
    nb = 7;
  }

  return CGRAM_1ST_CHAR_ADDR + 0x8 * nb;
}

static void lcd_write_nibble_and_clock (uint8_t command, uint8_t start_bit, uint8_t rs)
{
  lcd_write_nibble (command, start_bit, rs);
  lcd_clock ();
}

static void lcd_write_nibble (uint8_t command, uint8_t start_bit, uint8_t rs)
{
  if (get_bit (command, start_bit)) {
    LCD_D7_PORT->BSRR = LCD_7; // set one
  } else {
    LCD_D7_PORT->BSRR = (uint32_t)LCD_7<<16U;  //reset ODR by 16 times shifted current place
  }

  if (get_bit (command, start_bit-1)) {
    LCD_D6_PORT->BSRR = LCD_6; // set one
  } else {
    LCD_D6_PORT->BSRR = (uint32_t)LCD_6<<16U;  //reset ODR by 16 times shifted current place
  }

  if (get_bit (command, start_bit-2)) {
    LCD_D5_PORT->BSRR = LCD_5; // set one
  } else {
    LCD_D5_PORT->BSRR = (uint32_t)LCD_5<<16U;  //reset ODR by 16 times shifted current place
  }

  if (get_bit (command, start_bit-3)) {
    LCD_D4_PORT->BSRR = LCD_4; // set one
  } else {
    LCD_D4_PORT->BSRR = (uint32_t)LCD_4<<16U;  //reset ODR by 16 times shifted current place
  }

  // control port
  LCD_CONTROL_PORT->BSRR = (uint32_t)LCD_RW<<16U; //set zero on RW
  if (rs) {
    LCD_CONTROL_PORT->BSRR = LCD_RS; //set one
  } else {
    LCD_CONTROL_PORT->BSRR = (uint32_t)LCD_RS<<16U;  //reset ODR by 16 times shifted current place
  }
}

static void lcd_clock (void)
{
  LCD_CONTROL_PORT->BSRR = LCD_CLOCK; // set one;
  lcd_wait (LCD_SIGNAL_COOLDOWN_TIMEOUT);
  LCD_CONTROL_PORT->BSRR = (uint32_t)LCD_CLOCK<<16U;  //reset ODR by 16 times shifted current place
  lcd_wait (LCD_SIGNAL_COOLDOWN_TIMEOUT);
}

static uint8_t busy_flag_check (void)
{
  uint8_t out = BUSY;

  //must to set all data lines as input
  GPIO_InitTypeDef GPIO_InitStruct_D4;
  GPIO_InitTypeDef GPIO_InitStruct_D5;
  GPIO_InitTypeDef GPIO_InitStruct_D6;
  GPIO_InitTypeDef GPIO_InitStruct_D7;

  GPIO_InitStruct_D4.Pin = LCD_4;
  GPIO_InitStruct_D5.Pin = LCD_5;
  GPIO_InitStruct_D6.Pin = LCD_6;
  GPIO_InitStruct_D7.Pin = LCD_7;

  GPIO_InitStruct_D4.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct_D5.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct_D6.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct_D7.Mode = GPIO_MODE_INPUT;

  HAL_GPIO_Init (LCD_D4_PORT, &GPIO_InitStruct_D4);
  HAL_GPIO_Init (LCD_D5_PORT, &GPIO_InitStruct_D5);
  HAL_GPIO_Init (LCD_D6_PORT, &GPIO_InitStruct_D6);
  HAL_GPIO_Init (LCD_D7_PORT, &GPIO_InitStruct_D7);

  LCD_CONTROL_PORT->BSRR = LCD_RW; // set one - read from lcd
  LCD_CONTROL_PORT->BSRR = (uint32_t)LCD_RS<<16U;  //reset ODR by 16 times shifted current place
  LCD_CONTROL_PORT->BSRR = LCD_CLOCK;

  for (int i = BUSY_TIMEOUT_MS; i >  0; i-- ) {
    if (HAL_GPIO_ReadPin (LCD_D7_PORT, LCD_7) == NOT_BUSY) {
      out = NOT_BUSY;
      break;
    }
    lcd_wait (1);
  }

  //set data lines as output
  GPIO_InitStruct_D4.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct_D5.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct_D6.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct_D7.Mode = GPIO_MODE_OUTPUT_PP;

  GPIO_InitStruct_D4.Speed = GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct_D5.Speed = GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct_D6.Speed = GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct_D7.Speed = GPIO_SPEED_FREQ_LOW;

  HAL_GPIO_Init (LCD_D4_PORT, &GPIO_InitStruct_D4);
  HAL_GPIO_Init (LCD_D5_PORT, &GPIO_InitStruct_D5);
  HAL_GPIO_Init (LCD_D6_PORT, &GPIO_InitStruct_D6);
  HAL_GPIO_Init (LCD_D7_PORT, &GPIO_InitStruct_D7);

  LCD_CONTROL_PORT->BSRR = (uint32_t)LCD_RW<<16U;  // reset ODR by 16 times shifted current place - write to lcd

  return out;
}

static uint8_t get_bit (uint8_t command, uint8_t no)
{
  return  ( (command >> no) & 1);
}

static void lcd_wait (uint64_t LimitCycles)
{
  for (uint64_t cycle = 0; cycle < LimitCycles; cycle++) {
    asm __volatile__("nop");
  }
}
