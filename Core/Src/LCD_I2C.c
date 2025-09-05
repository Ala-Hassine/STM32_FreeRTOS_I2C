#include "LCD_I2C.h"

/**
  * @brief  Sends a command to the LCD screen.
  * @param  cmd: The command to be sent.
  * @retval None
  * @note   This function prepares and sends a command to the LCD screen via the I2C bus.
  *         The function divides the command into upper and lower 4-bit parts, puts them 
  *         into an LCD buffer, and sends them sequentially to trigger the control (enable) 
  *         and selection (RS) signals of the LCD screen.
  *         The control signals indicate to the LCD screen that it is a command, 
  *         and the enable bit triggers the reading of the command by the LCD screen.
  *
  * @note   For the LCD_SEND_CMD function:
  *         - The upper_data variable holds the upper 4 bits of the command.
  *         - The lower_data variable holds the lower 4 bits of the command.
  *         - The lcd_Buffer is a buffer array used for constructing the data to be sent.
  *         - The LCD_BUFFER_SIZE specifies the size of the lcd_Buffer array.
  *         - The UPPER_BITS_MASK is a bitmask to extract the upper 4 bits of a byte.
  *         - The ENABLE_BIT_MASK, RS_EN_OFF_MASK, and RS_BIT_MASK are masks for control bits.
  *         - The TIMEOUT is the maximum time to wait for the I2C transmission to complete.
  */

void LCD_SEND_CMD(char cmd)
{
  uint8_t lcd_Buffer[LCD_BUFFER_SIZE];
  char upper_data, lower_data;
  upper_data = (cmd & UPPER_BITS_MASK);
  lower_data = ((cmd << LCD_BUFFER_SIZE) & UPPER_BITS_MASK);

  // Constructing the LCD buffer with control (enable) and selection (RS) signals
  lcd_Buffer[0]  = upper_data|EN_BIT_MASK;      //en=1, rs=0
  lcd_Buffer[1]  = upper_data|RS_EN_OFF_MASK;   //en=0, rs=0
  lcd_Buffer[2]  = lower_data|EN_BIT_MASK;      //en=1, rs=0
  lcd_Buffer[3]  = lower_data|RS_EN_OFF_MASK;   //en=0, rs=0

  // Transmitting the buffer via the I2C bus to the LCD screen with the specified address
  HAL_I2C_Master_Transmit (&hi2c2, SLAVE_ADDRESS_LCD, (uint8_t *) lcd_Buffer, LCD_BUFFER_SIZE, TIMEOUT);
}


/**
  * @brief  Sends data to the LCD screen.
  * @param  data: The data to be sent.
  * @retval None
  * @note   This function prepares and sends data to the LCD screen via the I2C bus.
  *         The function divides the data into upper and lower 4-bit parts, puts them 
  *         into an LCD buffer, and sends them sequentially to trigger the control (enable) 
  *         and selection (RS) signals of the LCD screen.
  *         The control signals indicate to the LCD screen that it is data, 
  *         and the enable bit triggers the reading of the data by the LCD screen.
  *
  * @note   For the LCD_SEND_DATA function:
  *         - The upper_data variable holds the upper 4 bits of the data.
  *         - The lower_data variable holds the lower 4 bits of the data.
  *         - The lcd_Buffer is a buffer array used for constructing the data to be sent.
  *         - The LCD_BUFFER_SIZE specifies the size of the lcd_Buffer array.
  *         - The UPPER_BITS_MASK is a bitmask to extract the upper 4 bits of a byte.
  *         - The RS_EN_ON_MASK and RS_BIT_MASK are masks for control bits.
  *         - The TIMEOUT is the maximum time to wait for the I2C transmission to complete.
  */

void LCD_SEND_DATA(char data)
{
  uint8_t lcd_Buffer[LCD_BUFFER_SIZE];
  char upper_data, lower_data;
  upper_data = (data & UPPER_BITS_MASK);
  lower_data = ((data << LCD_BUFFER_SIZE) & UPPER_BITS_MASK);
  
  // Constructing the LCD buffer with control (enable) and selection (RS) signals
  lcd_Buffer[0]  = upper_data|RS_EN_ON_MASK;  //en=1, rs=1
  lcd_Buffer[1]  = upper_data|RS_BIT_MASK;    //en=0, rs=1
  lcd_Buffer[2]  = lower_data|RS_EN_ON_MASK;  //en=1, rs=1
  lcd_Buffer[3]  = lower_data|RS_BIT_MASK;    //en=0, rs=1
  HAL_I2C_Master_Transmit (&hi2c2, SLAVE_ADDRESS_LCD, (uint8_t *) lcd_Buffer, LCD_BUFFER_SIZE, TIMEOUT);
}


/**
  * @brief  Clears the content of the LCD screen.
  * @param  None
  * @retval None
  * @note   This function sends a command to set the cursor to the beginning of the first row
  *         and then sends space (' ') characters to clear the display.
  *
  * @note   For the LCD_CLEAR function:
  *         - The LCD_CURSOR_ROW_FIRST is a command to set the cursor to the beginning of the first row.
  *         - The LCD_CLEAR_ROW_LENGTH specifies the number of spaces to be sent to clear the display.
  */

void LCD_CLEAR(void)
{
  // Set the cursor to the beginning of the first row
  LCD_SEND_CMD(LCD_CURSOR_ROW_FIRST);

  // Send space (' ') characters to clear the display
  for (char i = 0; i < LCD_CLEAR_ROW_LENGTH; i++)
    LCD_SEND_DATA(' ');
}


/**
  * @brief  Sets the cursor position on the LCD screen.
  * @param  row: The row where the cursor will be set (0 or 1).
  * @param  col: The column where the cursor will be set (0 to 15).
  * @retval None
  * @note   This function sets the cursor position on the LCD screen based on the specified row and column.
  *
  * @note   For the LCD_SET_CURSOR function:
  *         - The LCD_CURSOR_ROW_FIRST and LCD_CURSOR_ROW_SECOND are commands to set the cursor to the
  *           beginning of the first and second rows, respectively.
  */

void LCD_SET_CURSOR(int row, int col)
{
  switch(row)
  {
    case 0: col |= LCD_CURSOR_ROW_FIRST; break;
    case 1: col |= LCD_CURSOR_ROW_SECOND; break;
  }
  LCD_SEND_CMD(col);
}


/**
  * @brief  Initializes the LCD screen.
  * @param  None
  * @retval None
  * @note   This function initializes the LCD screen. It follows a step-by-step process for proper
  *         initialization, including setting the LCD to 4-bit mode, configuring display settings,
  *         and enabling the display.
  *
  * @note   For the LCD_INIT function:
  *         - The LCD_INIT_CMD_8BIT is a command for initializing the LCD in 8-bit mode.
  *         - The LCD_INIT_CMD_4BIT is a command for switching the LCD to 4-bit mode.
  *         - The DELAY_50MS, DELAY_5MS, DELAY_1MS, and DELAY_10MS are delay values for waiting
  *           specific durations during the initialization process.
  *         - The LCD_INIT_CMD_FUNCTION_SET, LCD_INIT_CMD_DISPLAY_OFF, LCD_INIT_CMD_CLEAR_DISPLAY,
  *           LCD_INIT_CMD_ENTRY_MODE_SET, and LCD_INIT_CMD_DISPLAY_ON are commands for configuring
  *           various settings such as function set, display on/off control, clearing the display,
  *           entry mode set, and enabling the display.
  */

void LCD_INIT(void)
{
  // Initialisation en mode 4 bits
  HAL_Delay(DELAY_50MS);
  LCD_SEND_CMD(LCD_INIT_CMD_8BIT);
  HAL_Delay(DELAY_5MS);
  LCD_SEND_CMD(LCD_INIT_CMD_8BIT);
  HAL_Delay(DELAY_1MS);
  LCD_SEND_CMD(LCD_INIT_CMD_8BIT);
  HAL_Delay(DELAY_10MS);
  LCD_SEND_CMD(LCD_INIT_CMD_4BIT);
  HAL_Delay(DELAY_10MS);

  // dislay initialisation
  LCD_SEND_CMD (LCD_INIT_CMD_FUNCTION_SET); // Function set --> DL=0 (4 bit mode), N = 1 (2 line display) F = 0 (5x8 characters)
  HAL_Delay(DELAY_1MS);
  LCD_SEND_CMD (LCD_INIT_CMD_DISPLAY_OFF); //Display on/off control --> D=0,C=0, B=0  ---> display off
  HAL_Delay(DELAY_1MS);
  LCD_SEND_CMD (LCD_INIT_CMD_CLEAR_DISPLAY);  // clear display
  HAL_Delay(DELAY_1MS);
  HAL_Delay(DELAY_1MS);
  LCD_SEND_CMD (LCD_INIT_CMD_ENTRY_MODE_SET); //Entry mode set --> I/D = 1 (increment cursor) & S = 0 (no shift)
  HAL_Delay(DELAY_1MS);
  LCD_SEND_CMD (LCD_INIT_CMD_DISPLAY_ON); //Display on/off control --> D = 1, C and B = 0. (Cursor and blink, last two bits)
}


/**
  * @brief  Sends a string of characters to be displayed on the LCD screen.
  * @param  str: Pointer to the string to be displayed.
  * @retval None
  * @note   This function iterates through each character in the provided string
  *         and sends it to the LCD screen using the LCD_SEND_DATA function.
  *
  * @note   For the LCD_SEND_STRING function:
  *         - The *str is a pointer to the input string.
  */

void LCD_SEND_STRING(char *str)
{
  while (*str) LCD_SEND_DATA(*str++);
}

/**
  * @brief  Scrolls the content of the LCD screen to the left.
  * @param  None
  * @retval None
  * @note   For the LCD_SCROLL_LEFT function:
  *         - The LCD_MOVE_LEFT the command for shifting the display to the left.
  */

void LCD_SCROLL_LEFT(void)
{
  LCD_SEND_CMD(LCD_MOVE_LEFT);
}


/**
  * @brief  Scrolls the content of the LCD screen to the right.
  * @param  None
  * @retval None
  * @note   For the LCD_SCROLL_RIGHT function:
  *         - The LCD_MOVE_RIGHT the command for shifting the display to the right.
  */

void LCD_SCROLL_RIGHT(void)
{
  LCD_SEND_CMD(LCD_MOVE_RIGHT);
}
