#include "hardware.h"

SPI_HandleTypeDef 	SPI1_Handle;
UART_HandleTypeDef	UART1_Handle;

void MTR_IRQ0_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};

  __HAL_RCC_GPIOB_CLK_ENABLE();

  GPIO_InitStruct.Pin = GPIO_PIN_0;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  HAL_NVIC_SetPriority(EXTI0_IRQn, 5, 0);
  HAL_NVIC_EnableIRQ(EXTI0_IRQn);
}

void MTR_IRQ1_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};

  __HAL_RCC_GPIOB_CLK_ENABLE();

  GPIO_InitStruct.Pin = GPIO_PIN_1;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  HAL_NVIC_SetPriority(EXTI1_IRQn, 5, 0);
  HAL_NVIC_EnableIRQ(EXTI1_IRQn);
}

void MTR_RST_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  __HAL_RCC_GPIOC_CLK_ENABLE();

  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_4, GPIO_PIN_RESET);

  GPIO_InitStruct.Pin = GPIO_PIN_4;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
}

void SPI1_SS_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  __HAL_RCC_GPIOA_CLK_ENABLE();

  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET);

  /*Configure GPIO pin : PA4 */
  GPIO_InitStruct.Pin = GPIO_PIN_4;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
}

void LED_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  __HAL_RCC_GPIOB_CLK_ENABLE();

  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_RESET);

  GPIO_InitStruct.Pin = GPIO_PIN_12;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
}



void RS485_RW_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  __HAL_RCC_GPIOB_CLK_ENABLE();

  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8, GPIO_PIN_RESET);

  GPIO_InitStruct.Pin = GPIO_PIN_8;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
}

// Metering IC
void SPI1_Init(void)
{
	SPI1_Handle.Instance = SPI1;
  SPI1_Handle.Init.Mode = SPI_MODE_MASTER;
  SPI1_Handle.Init.Direction = SPI_DIRECTION_2LINES;
  SPI1_Handle.Init.DataSize = SPI_DATASIZE_8BIT;
  SPI1_Handle.Init.CLKPolarity = SPI_POLARITY_HIGH;
  SPI1_Handle.Init.CLKPhase = SPI_PHASE_2EDGE;
  SPI1_Handle.Init.NSS = SPI_NSS_SOFT;
  SPI1_Handle.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_128;
  SPI1_Handle.Init.FirstBit = SPI_FIRSTBIT_MSB;
  SPI1_Handle.Init.TIMode = SPI_TIMODE_DISABLE;
  SPI1_Handle.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  SPI1_Handle.Init.CRCPolynomial = 10;
  HAL_SPI_Init(&SPI1_Handle);
}

// RS485 - modbus UART
void UART1_Init(void)
{
  UART1_Handle.Instance = USART1;
  UART1_Handle.Init.BaudRate = 115200;
  UART1_Handle.Init.WordLength = UART_WORDLENGTH_8B;
  UART1_Handle.Init.StopBits = UART_STOPBITS_1;
  UART1_Handle.Init.Parity = UART_PARITY_NONE;
  UART1_Handle.Init.Mode = UART_MODE_TX_RX;
  UART1_Handle.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  UART1_Handle.Init.OverSampling = UART_OVERSAMPLING_16;
	HAL_UART_Init(&UART1_Handle);
}
