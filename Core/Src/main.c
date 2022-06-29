#include "main.h"
#include "cmsis_os.h"

#include "registers.h"
#include "ade7880.h"
#include "modbus.h"
#include "flash.h"
#include "string.h"
extern uint8_t NbRxData;

struct registers reg;
float energy_cumulA;

float energy_cumulAA;
float energy_cumulBB;
float energy_cumulCC;

float energy_cumulB,energy_cumulC;
osThreadId_t meteringTaskHandle;
const osThreadAttr_t meteringTask_attributes = {
  .name = "meteringTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};

osThreadId_t modbusTaskHandle;
const osThreadAttr_t modbusTask_attributes = {
  .name = "modbusTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};

void SystemClock_Config(void);
void MeteringTask(void *argument);
void ModbusTask(void *argument);
char *data = "hello FLASH from ControllerTech\
			  This is a test to see how many words can we work with";

uint32_t data2[] = {0x1,0x2,0x3,0x4,0x5,0x6,0x7,0x8,0x9};

uint32_t Rx_Data[30];

char string[100];

int number = 123;

float val = 123.456;

float RxVal_A;
float RxVal_B;
float RxVal_C;
int main(void)
{
HAL_Init();
SystemClock_Config();
 
osKernelInitialize();
meteringTaskHandle = osThreadNew(MeteringTask, NULL, &meteringTask_attributes);
modbusTaskHandle = osThreadNew(ModbusTask, NULL, &modbusTask_attributes);
osKernelStart();

  while (1) {  
		
//	meteringTaskHandle = osThreadNew(MeteringTask, NULL, &meteringTask_attributes);
//osKernelStart();
	//			ADE7880_Get_VrmsC(&reg.VrmsC);
//  osKernelStart();

}
}
void MeteringTask(void *argument)
{
	ADE7880_Init();
	ADE7880_Reset();
	ADE7880_SPI_Enable();
	ADE7880_Start();
	
	// 40ms  settling time for Period measurment
	// 580ms settling time for Irms/Vrms measurment
	osDelay(1000);
	
  for(;;)
  {
		
	ADE7880_Get_Freq(&reg.Freq);
   ADE7880_Get_VrmsA(&reg.VrmsA);
		ADE7880_Get_VrmsB(&reg.VrmsB);
		ADE7880_Get_VrmsC(&reg.VrmsC);

		
ADE7880_Get_IrmsA(&reg.IrmsA);
 	    ADE7880_Get_IrmsB(&reg.IrmsB);
    ADE7880_Get_IrmsC(&reg.IrmsC);
	  ADE7880_Get_IrmsN(&reg.IrmsN);
 	ADE7880_Get_APwrA(&reg.APwrA);
    ADE7880_Get_APwrB(&reg.APwrB);
   ADE7880_Get_APwrC(&reg.APwrC);
  energy_cumulA=ADE7880_Get_AWATTHR(&reg.AWATTHRR);
	energy_cumulB=  ADE7880_Get_BWATTHR(&reg.BWATTHRR);
	energy_cumulC=  ADE7880_Get_CWATTHR(&reg.CWATTHRR);
	
		ADE7880_Get_APPwr(&reg.APPW,&reg.AVA1,&reg.ABA1,&reg.ACA1);
//		
	ADE7880_Get_AFWATTHR(&reg.AFWATTHRR);
	ADE7880_Get_BFWATTHR(&reg.BFWATTHRR);
	ADE7880_Get_CFWATTHR(&reg.CFWATTHRR);

//		
	(energy_cumulAA) += (energy_cumulA);

	//energy_cumulA +=reg.AWATTHRR;
	energy_cumulBB+= energy_cumulB;
  energy_cumulCC += energy_cumulC;
	
	(reg.energy_cumulAAA)=energy_cumulAA;
	reg.energy_cumulBBB=energy_cumulBB;
	reg.energy_cumulCCC=energy_cumulCC;
//		
//		
	 ADE7880_Get_PWFACTOR(&reg.PF,&reg.PF_A,&reg.PF_B,&reg.PF_C);
//		
   ADE7880_Get_AFVARHR(&reg.AFVARHR1);
   ADE7880_Get_BFVARHR(&reg.BFVARHR1);
   ADE7880_Get_CFVARHR(&reg.CFVARHR1);

   ADE7880_Get_AVAHR(&reg.AVAHR1);
   ADE7880_Get_AVAHR(&reg.BVAHR1);
   ADE7880_Get_AVAHR(&reg.CVAHR1);


   ADE7880_Get_AVAHR(&reg.AVAHR1);
	 ADE7880_Get_BVAHR(&reg.BVAHR1);
	 ADE7880_Get_CVAHR(&reg.CVAHR1);
	


   ADE7880_Get_FWATT(&reg.FWATT1);
	 ADE7880_Get_FVAR(&reg.FVAR1);
		
		
	ADE7880_Get_Har_Pwr_THD(PhA, 3, 5, 7, &reg.H03A, &reg.H05A, &reg.H07A, &reg.APwrAA, &reg.RPwrA, &reg.THDV_A, &reg.THDI_A);
		ADE7880_Get_Har_Pwr_THD(PhB, 3, 5, 7, &reg.H03B, &reg.H05B, &reg.H07B, &reg.APwrB, &reg.RPwrB, &reg.THDV_B, &reg.THDI_B);
		ADE7880_Get_Har_Pwr_THD(PhC, 3, 5, 7, &reg.H03C, &reg.H05C, &reg.H07C, &reg.APwrC, &reg.RPwrC, &reg.THDV_C, &reg.THDI_C);

	ADE7880_Get_Har(PhA, 9, 11, 13, &reg.H09A, &reg.H11A, &reg.H13A);
	ADE7880_Get_Har(PhB, 9, 11, 13, &reg.H09B, &reg.H11B, &reg.H13B);
		ADE7880_Get_Har(PhC, 9, 11, 13, &reg.H09C, &reg.H11C, &reg.H13C);
	
	ADE7880_Get_Har(PhA, 15, 17, 19, &reg.H15A, &reg.H17A, &reg.H19A);
		ADE7880_Get_Har(PhB, 15, 17, 19, &reg.H15B, &reg.H17B, &reg.H19B);
		ADE7880_Get_Har(PhC, 15, 17, 19, &reg.H15C, &reg.H17C, &reg.H19C);
		
		ADE7880_Get_Har(PhA, 21, 23, 25, &reg.H21A, &reg.H23A, &reg.H25A);
		ADE7880_Get_Har(PhB, 21, 23, 25, &reg.H21B, &reg.H23B, &reg.H25B);
		ADE7880_Get_Har(PhC, 21, 23, 25, &reg.H21C, &reg.H23C, &reg.H25C);

		ADE7880_Get_Har(PhA, 27, 29, 31, &reg.H27A, &reg.H29A, &reg.H31A);
		ADE7880_Get_Har(PhB, 27, 29, 31, &reg.H27B, &reg.H29B, &reg.H31B);
		ADE7880_Get_Har(PhC, 27, 29, 31, &reg.H27C, &reg.H29C, &reg.H31C);

		ADE7880_Get_Har(PhA, 33, 35, 37, &reg.H03A, &reg.H05A, &reg.H07A);
		ADE7880_Get_Har(PhB, 33, 35, 37, &reg.H03B, &reg.H05B, &reg.H07B);
		ADE7880_Get_Har(PhC, 33, 35, 37, &reg.H03C, &reg.H05C, &reg.H07C);

		ADE7880_Get_Har(PhA, 39, 41, 43, &reg.H39A, &reg.H41A, &reg.H43A);
		ADE7880_Get_Har(PhB, 39, 41, 43, &reg.H39B, &reg.H41B, &reg.H43B);
		ADE7880_Get_Har(PhC, 39, 41, 43, &reg.H39C, &reg.H41C, &reg.H43C);
		
		ADE7880_Get_Har(PhA, 45, 47, 49, &reg.H45A, &reg.H47A, &reg.H49A);
		ADE7880_Get_Har(PhB, 45, 47, 49, &reg.H45B, &reg.H47B, &reg.H49B);
		ADE7880_Get_Har(PhC, 45, 47, 49, &reg.H45C, &reg.H47C, &reg.H49C);
		
		ADE7880_Get_Har(PhA, 51, 53, 55, &reg.H51A, &reg.H53A, &reg.H55A);
		ADE7880_Get_Har(PhB, 51, 53, 55, &reg.H51B, &reg.H53B, &reg.H55B);
		ADE7880_Get_Har(PhC, 51, 53, 55, &reg.H51C, &reg.H53C, &reg.H55C);
		
		ADE7880_Get_Har(PhA, 57, 59, 61, &reg.H57A, &reg.H59A, &reg.H61A);
		ADE7880_Get_Har(PhB, 57, 59, 61, &reg.H57B, &reg.H59B, &reg.H61B);
		ADE7880_Get_Har(PhC, 57, 59, 61, &reg.H57C, &reg.H59C, &reg.H61C);

		osDelay(1);
//		Flash_Write_Data(0x08004100 , (uint32_t *)&(reg.VrmsA),1);
//     Flash_Read_Data(0x0800C400 , Rx_Data, 10);


//  int numofwords = (strlen(data)/4)+((strlen(data)%4)!=0);
//  Flash_Write_Data(0x08008100 , (uint32_t *)&(reg.VrmsA), numofwords);
//  Flash_Read_Data(0x08008100 , Rx_Data, numofwords);
//  Convert_To_Str(Rx_Data, string);
//	 Flash_Write_NUM(0x0800C100, number);
//  RxVal = Flash_Read_NUM(0x0800C100);

//  Flash_Write_NUM(0x0800D100,reg.energy_cumulAAA);
//  RxVal_A = Flash_Read_NUM(0x0800D100);
//	
//	  Flash_Write_NUM(0x0800C100,reg.energy_cumulBBB);
//  RxVal_B = Flash_Read_NUM(0x0800C100);
//	
//	  Flash_Write_NUM(0x08008100,reg.energy_cumulCCC);
//  RxVal_C = Flash_Read_NUM(0x08008100);
  }
}
//void ADE7880_PHASEUNBALANCING()
//{
//if(reg.VrmsA != reg.VrmsB !=reg.VrmsC )
//	
//{


//}
//}
void ModbusTask(void *argument)
{
	ModBus_Init();
	
  for(;;)
  {
		ModBus_Process((uint32_t *)&reg, sizeof(reg)/sizeof(float));
		osDelay(1);
  }
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	

	
	
	
	if (GPIO_Pin == GPIO_PIN_0)
	{
		// ADE7880 IRQ0
		osThreadFlagsSet(meteringTaskHandle, ADE7880_IRQ0);
	}
	else if (GPIO_Pin == GPIO_PIN_1)
	{
		// ADE7880 IRQ1
		osThreadFlagsSet(meteringTaskHandle, ADE7880_IRQ1);
	}
}

void HAL_SPI_TxCpltCallback(SPI_HandleTypeDef *hspi)
{
	osThreadFlagsSet(meteringTaskHandle, ADE7880_TxCplt);
}

void HAL_SPI_RxCpltCallback(SPI_HandleTypeDef *hspi)
{
	osThreadFlagsSet(meteringTaskHandle, ADE7880_RxCplt);
}

void HAL_UART_TxCpltCallback(UART_HandleTypeDef *UartHandle)
{
	osThreadFlagsSet(modbusTaskHandle, ModBus_TxCplt);
}

void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size)
{
	NbRxData = Size;
	osThreadFlagsSet(modbusTaskHandle, ModBus_RxCplt);
}

void HAL_UART_ErrorCallback(UART_HandleTypeDef *UartHandle)
{
	// 2DO
	__NOP();
}
//

void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE3);
  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 25;
  RCC_OscInitStruct.PLL.PLLN = 180;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 2;
  RCC_OscInitStruct.PLL.PLLR = 2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  /* USER CODE BEGIN Callback 0 */

  /* USER CODE END Callback 0 */
  if (htim->Instance == TIM7) {
    HAL_IncTick();
  }
  /* USER CODE BEGIN Callback 1 */

  /* USER CODE END Callback 1 */
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
