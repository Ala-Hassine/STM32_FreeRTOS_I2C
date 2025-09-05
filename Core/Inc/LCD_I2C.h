#ifndef LCD_I2C_H_
#define LCD_I2C_H_

#include "stm32f4xx_hal.h"

extern I2C_HandleTypeDef hi2c2;

#define SLAVE_ADDRESS_LCD             0x4E
#define LCD_BUFFER_SIZE               4
#define UPPER_BITS_MASK               0xF0
#define TIMEOUT                       100
#define EN_BIT_MASK                   0x0C
#define RS_EN_OFF_MASK                0x08
#define RS_EN_ON_MASK                 0x0D
#define RS_BIT_MASK                   0x09
#define LCD_CURSOR_ROW_FIRST          0x80
#define LCD_CURSOR_ROW_SECOND         0xC0
#define LCD_CLEAR_ROW_LENGTH          16
#define DELAY_50MS                    40
#define DELAY_5MS                     5
#define DELAY_1MS                     1
#define DELAY_10MS                    10
#define LCD_INIT_CMD_8BIT             0x30              // Commande d'initialisation 8 bits
#define LCD_INIT_CMD_4BIT             0x20              // Commande pour activer le mode 4 bits
#define LCD_INIT_CMD_FUNCTION_SET     0x28              // Function set
#define LCD_INIT_CMD_DISPLAY_OFF      0x08              // Display on/off control (display off)
#define LCD_INIT_CMD_CLEAR_DISPLAY    0x01              // Clear display
#define LCD_INIT_CMD_ENTRY_MODE_SET   0x06              // Entry mode set
#define LCD_INIT_CMD_DISPLAY_ON       0x0C              // Display on/off control (display on)
#define LCD_MOVE_RIGHT                0x1C
#define LCD_MOVE_LEFT                 0x18


void LCD_SEND_CMD(char cmd);
void LCD_SEND_DATA(char data);
void LCD_CLEAR(void);
void LCD_SET_CURSOR(int row, int col);
void LCD_INIT(void);
void LCD_SEND_STRING(char *str);
void LCD_SCROLL_LEFT(void);
void LCD_SCROLL_RIGHT(void);


#endif /* LCD_I2C_H_ */
