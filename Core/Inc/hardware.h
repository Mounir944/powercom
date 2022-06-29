#include "stm32f4xx_hal.h"

#define LED_On()						HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_SET)
#define LED_Off()						HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_RESET)
#define MTR_RST_High()			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_4, GPIO_PIN_SET)
#define MTR_RST_Low()				HAL_GPIO_WritePin(GPIOC, GPIO_PIN_4, GPIO_PIN_RESET)
#define SPI1_SS_High()			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET)
#define SPI1_SS_Low()				HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET)
#define RS485_RW_SetRead()	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8, GPIO_PIN_RESET)
#define RS485_RW_SetWrite() HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8, GPIO_PIN_SET)

void MTR_IRQ0_Init(void);
void MTR_IRQ1_Init(void);
void MTR_RST_Init(void);
void SPI1_SS_Init(void);
void LED_Init(void);
void RS485_RW_Init(void);
void SPI1_Init(void);
void UART1_Init(void);
