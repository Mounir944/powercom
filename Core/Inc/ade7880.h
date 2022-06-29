#include "hardware.h"

#define PhA	0x01
#
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
#define PhB	0x02
#define PhC	0x04

#define ADE7880_IRQ0		0x00000001
#define ADE7880_IRQ1 		0x00000002
#define ADE7880_TxCplt	0x00000004
#define ADE7880_RxCplt 	0x00000008

void ADE7880_Init(void);
void ADE7880_Reset(void);
void ADE7880_SPI_Enable(void);
void ADE7880_Start(void);

void ADE7880_Get_Freq(float *Freq);
void ADE7880_Get_VrmsA(float *VrmsA);
void ADE7880_Get_VrmsB(float *VrmsB);
void ADE7880_Get_VrmsC(float *VrmsC);
void ADE7880_Get_IrmsA(float *IrmsA);
void ADE7880_Get_IrmsB(float *IrmsB);
void ADE7880_Get_IrmsC(float *IrmsC);
void ADE7880_Get_IrmsN(float *IrmsN);
void ADE7880_Get_APwrA(float *APwrA);
void ADE7880_Get_APwrB(float *APwrB);
void ADE7880_Get_APwrC(float *APwrC);
void ADE7880_Get_APPwr(float *APPW,float *AVA1,float *ABA1,float *ACA1);
uint64_t ADE7880_Get_AFWATTHR(float *AFWATTHRR );
uint64_t ADE7880_Get_BFWATTHR(float *BFWATTHRR );
uint64_t ADE7880_Get_CFWATTHR(float *CFWATTHRR );
void ADE7880_Get_PWFACTOR(float *PF,float *PF_A,float *PF_B,float *PF_C);

float ADE7880_Get_AWATTHR(float *AWATTHRR );
float ADE7880_Get_BWATTHR(float *BWATTHRR );
float ADE7880_Get_CWATTHR(float *CWATTHRR );




uint64_t ADE7880_Get_AFVARHR(float *AFVARHR1);
uint64_t ADE7880_Get_BFVARHR(float *BFVARHR1);
uint64_t ADE7880_Get_CFVARHR(float *CFVARHR1);

void ADE7880_Get_AVAHR(float *AVAHR1);
void ADE7880_Get_AVAHR(float *BVAHR1);
void ADE7880_Get_AVAHR(float *CVAHR1);


void ADE7880_Get_AVAHR(float *AVAHR1);
	void ADE7880_Get_BVAHR(float *BVAHR1);
	void ADE7880_Get_CVAHR(float *CVAHR1);
	


void ADE7880_Get_FWATT(float *FWATT1);
	void ADE7880_Get_FVAR(float *FVAR1);



void ADE7880_Get_Har_Pwr_THD(uint8_t Ph, uint8_t x, uint8_t y, uint8_t z, float *H_x, float *H_y, float *H_z, float *APwr, float *RPwr, float *THDV, float *THDI);
void ADE7880_Get_Har(uint8_t Ph, uint8_t x, uint8_t y, uint8_t z, float *H_x, float *H_y, float *H_z);
