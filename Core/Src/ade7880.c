#include "ade7880.h"
#include "cmsis_os.h"
#include "math.h"
//#define Volt_LSB 11060  //10750
//#define Amp_LSB  743658 //724441
//#define Watt_LSB 981
//#define VAR_LSB  1
//#define WattHR_LSB 10893,913
//#define VLEVEL	0x439F

#define Volt_LSB 11033.08 //10750

//#define Volt_LSB 11033 //10750
#define Amp_LSB  743472 //724441
#define Watt_LSB 1010    //,55652
#define VAR_LSB  3968,12338
#define VA_LSB 997,330435
#define WattHR_LSB 10893.913
#define VLEVEL	0x439F



#define AIRMS		0x43C0
#define AVRMS		0x43C1
#define BIRMS 	0x43C2
#define BVRMS 	0x43C3
#define CIRMS 	0x43C4
#define CVRMS 	0x43C5
#define NIRMS 	0x43C6

#define RUN			0xE228
	#define START		0x0001

#define STATUS0 0xE502
	#define HREADY	0x00080000

#define STATUS1 0xE503
	#define RSTDONE 0x00008000

#define MASK0		0xE50A

#define AWATT 	0xE513
#define BWATT 	0xE514
#define CWATT 	0xE515

#define FWATT		0xE882
#define FVAR		0xE883
#define VTHD		0xE886
#define ITHD		0xE887
#define HXVRMS	0xE888
#define HYVRMS	0xE890
#define HZVRMS	0xE898

#define HCONFIG	0xE900
	#define HPHASE_A		0x0000
	#define HPHASE_B		0x0002
	#define HPHASE_C		0x0004
	#define HSTIME			0x0008
	#define ACTPHSEL_A	0x0000
	#define ACTPHSEL_B 	0x0100
	#define ACTPHSEL_C 	0x0200

#define CPERIOD	0xE907

#define WTHR 		0xEA02
#define VARTHR	0xEA03
#define VATHR 	0xEA04

#define HX 			0xEA08
#define HY 			0xEA09
#define HZ 			0xEA0A

#define FVA     0xE884    //puissance apparente

#define AVA 		0xE519
#define BVA 		0xE51A
#define CVA 		0xE51B



#define FPF       0xE885   //power factor 
#define APF			  0xE902   //power factor on phase A
#define BPF			  0xE903  //power factor on phase B
#define CPF			  0xE904   //power factor on phase C
#define AFWATTHR  0xE403 //active energy fundament phase A
#define BFWATTHR  0xE404 //active energy fundament phase B
#define CFWATTHR  0xE405 //active energy fundament phase C
 
 
 
 
#define AWATTHR   0xE400 //TOT ctive energy  phase A
#define BWATTHR   0xE401 //TOT active energy phase B
#define CWATTHR  0xE402//TOTAL active energ

#define AFVARHR   0xE409 //reactive energy fundament phase A
#define BFVARHR   0xE40A //reactive energy fundament phase B
#define CFVARHR   0xE40B //reactive energy fundament phase C

#define AVAHR 0xE40C
#define BVAHR 0xE40D
#define CVAHR 0xE40E

 uint64_t   energyc_a     ;
extern SPI_HandleTypeDef SPI1_Handle;

void ADE7880_Write08(uint16_t Reg, uint8_t Val);
void ADE7880_Write16(uint16_t Reg, uint16_t Val);
void ADE7880_Write32(uint16_t Reg, uint32_t Val);
void ADE7880_Read16(uint16_t Reg, uint16_t *Val);
void ADE7880_Read32(uint16_t Reg, uint32_t *Val);

void ADE7880_Init(void)
{
	MTR_IRQ0_Init();
	MTR_IRQ1_Init();
	MTR_RST_Init();
	SPI1_SS_Init();
	SPI1_Init();
}

void ADE7880_Reset(void)
{
	MTR_RST_Low();
	osDelay(1);
	MTR_RST_High();
	osThreadFlagsWait(ADE7880_IRQ1, osFlagsWaitAny, osWaitForever);
}

void ADE7880_SPI_Enable(void)
{
	SPI1_SS_Low();
	osDelay(1);
	SPI1_SS_High();
	osDelay(1);
	
	SPI1_SS_Low();
	osDelay(1);
	SPI1_SS_High();
	osDelay(1);
	
	SPI1_SS_Low();
	osDelay(1);
	SPI1_SS_High();
}

void ADE7880_Start(void)
{
	// reset is held low at startup
	ADE7880_Write08(0xE7FE, 0xAD);
	ADE7880_Write08(0xE7E2, 0x14);
	osDelay(1);
	ADE7880_Write08(0xE7FE, 0xAD);
	ADE7880_Write08(0xE7E2, 0x04);
	
	ADE7880_Write32(STATUS0, 0xFFFFFFFF);
	ADE7880_Write32(STATUS1, RSTDONE);
	ADE7880_Write32(VLEVEL, 0x005CFA67);
	ADE7880_Write08(WTHR, 0x28);
	ADE7880_Write08(VARTHR, 0x28);
	ADE7880_Write08(VATHR, 0x28);
	
	// make Harmonic ready flag set IRQ0
	ADE7880_Write32(MASK0, HREADY);
	
	// start DSP
	ADE7880_Write16(RUN, START);
}

void ADE7880_Read16(uint16_t Reg, uint16_t *Val)
{
	uint8_t TxData[3] = {0x01, Reg >> 8, Reg};
	uint8_t RxData[2]; 
	
	SPI1_SS_Low();
	HAL_SPI_Transmit_IT(&SPI1_Handle, (uint8_t *)TxData, 3);
	osThreadFlagsWait(ADE7880_TxCplt, osFlagsWaitAny, osWaitForever);
	HAL_SPI_Receive_IT(&SPI1_Handle, (uint8_t *)RxData, 2);
	osThreadFlagsWait(ADE7880_RxCplt, osFlagsWaitAny, osWaitForever);
	SPI1_SS_High();
	
	*Val = RxData[0] << 8 | RxData[1];
}

void ADE7880_Read32(uint16_t Reg, uint32_t *Val)
{
	uint8_t TxData[3] = {0x01, Reg >> 8, Reg};
	uint8_t RxData[4];
	
	SPI1_SS_Low();
	HAL_SPI_Transmit_IT(&SPI1_Handle, (uint8_t *)TxData, 3);
	osThreadFlagsWait(ADE7880_TxCplt, osFlagsWaitAny, osWaitForever);
	HAL_SPI_Receive_IT(&SPI1_Handle, (uint8_t *)RxData, 4);
	osThreadFlagsWait(ADE7880_RxCplt, osFlagsWaitAny, osWaitForever);
	SPI1_SS_High();
	
	*Val = RxData[0] << 24 | RxData[1] << 16 | RxData[2] << 8 | RxData[3] ;
}

void ADE7880_Write08(uint16_t Reg, uint8_t Val)
{
	uint8_t Data[4] = {0x00, Reg >> 8, Reg, Val};
	
	SPI1_SS_Low();
	HAL_SPI_Transmit_IT(&SPI1_Handle, (uint8_t *)Data, 4);
	osThreadFlagsWait(ADE7880_TxCplt, osFlagsWaitAny, osWaitForever);
	SPI1_SS_High();
}

void ADE7880_Write16(uint16_t Reg, uint16_t Val)
{
	
	uint8_t Data[5] = {0x00, Reg >> 8, Reg, Val >> 8, Val};
	
	SPI1_SS_Low();
	HAL_SPI_Transmit_IT(&SPI1_Handle, (uint8_t *)Data, 5);
	osThreadFlagsWait(ADE7880_TxCplt, osFlagsWaitAny, osWaitForever);
	SPI1_SS_High();
	
}

void ADE7880_Write32(uint16_t Reg, uint32_t Val)
{
	uint8_t Data[7] = {0x00, Reg >> 8, Reg, Val >> 24, Val >> 16, Val >> 8, Val};
	
	SPI1_SS_Low();
	HAL_SPI_Transmit_IT(&SPI1_Handle, (uint8_t *)Data, 7);
	osThreadFlagsWait(ADE7880_TxCplt, osFlagsWaitAny, osWaitForever);
	SPI1_SS_High();
}
//
void ADE7880_Get_Freq(float *Freq)
{
	uint16_t Data;
	ADE7880_Read16(CPERIOD, &Data);
	*Freq = 256000 / (float)Data;
}
void ADE7880_Get_VrmsA(float *VrmsA)
{
	uint32_t Data;
	
	ADE7880_Read32(AVRMS, &Data);
	*VrmsA = (float)Data / Volt_LSB;
}
void ADE7880_Get_VrmsB(float *VrmsB)
{
	uint32_t Data;
	ADE7880_Read32(BVRMS, &Data);
	*VrmsB = (float)Data / Volt_LSB;
}
void ADE7880_Get_VrmsC(float *VrmsC)
{
	uint32_t Data;
	ADE7880_Read32(CVRMS, &Data);
	*VrmsC = (float)Data / Volt_LSB;
}
void ADE7880_Get_IrmsA(float *IrmsA)
{
	uint32_t Data;
	ADE7880_Read32(AIRMS, &Data);
//	*IrmsA = ((float)Data / Amp_LSB)*20;
		*IrmsA = ((float)Data / Amp_LSB)*20;

}
void ADE7880_Get_IrmsB(float *IrmsB)
{
	uint32_t Data;
	ADE7880_Read32(BIRMS, &Data);
	*IrmsB = ((float)Data / Amp_LSB)*20;
}
void ADE7880_Get_IrmsC(float *IrmsC)
{
	uint32_t Data;
	ADE7880_Read32(CIRMS, &Data);
	*IrmsC = ((float)Data / Amp_LSB)*20;
}
void ADE7880_Get_IrmsN(float *IrmsN)
{
	uint32_t Data;
	ADE7880_Read32(NIRMS, &Data);
	*IrmsN = (float)Data / Amp_LSB;
}
void ADE7880_Get_APwrA(float *APwrA)
{
	uint32_t Data;
	uint32_t *s=0;
		uint32_t *a=0;

//		for(int i=1;i<=10;i++)
//	{
	ADE7880_Read32(AWATT, &Data);
		//*s += (float)(int32_t)Data;
	//	*a=*s/5;
		//	*APwrA = *a/Watt_LSB;
	*APwrA = fabs((float)(int32_t)Data/Watt_LSB);
//	*APwrA = fabs((float)(int32_t)Data )*(9.17*pow(10,-5));
	}
	//	/ Watt_LSB;

void ADE7880_Get_APwrB(float *APwrB)
{
	uint32_t Data;
	ADE7880_Read32(BWATT, &Data);
	*APwrB = (float)(int32_t)Data / Watt_LSB;
}
void ADE7880_Get_APwrC(float *APwrC)
{
	uint32_t Data;
	ADE7880_Read32(CWATT, &Data);
	*APwrC = (float)(int32_t)Data / Watt_LSB;
}
void ADE7880_Get_APPwr(float *APPW,float *AVA1,float *ABA1,float *ACA1)
{
	
	uint32_t Data;
	ADE7880_Read32(FVA, &Data);  //fundamental component of apparnt power
	*APPW = (float)(int32_t)Data / Watt_LSB;
		ADE7880_Read32(AVA, &Data); //instaneous value of apparent power on phase A
	*AVA1=(float)(int32_t)Data /Watt_LSB;
		ADE7880_Read32(BVA, &Data);
	*ABA1=(float)(int32_t)Data /Watt_LSB;

		ADE7880_Read32(CVA, &Data);
	*ACA1=(float)(int32_t)Data /Watt_LSB;

	
}
float ADE7880_Get_AWATTHR(float *AWATTHRR )
{
	uint32_t Data;
	uint64_t *AWA;
uint64_t	energy_cumulAA ;
	ADE7880_Read32(AWATTHR, &Data);

	
	//*AWATTHRR =(-1)*(float)(int32_t)Data/10000;
	*AWATTHRR =(float)(int32_t)Data/10893.913;
	//*AWA=(-1)*(float)(int32_t)Data/10000;
//	*AWA =(int64_t)*AWATTHRR ;
	//	 / WattHR_LSB;
return *AWATTHRR;

}

float ADE7880_Get_BWATTHR(float *BWATTHRR )
{
	uint32_t Data;
	ADE7880_Read32(BWATTHR, &Data);
	*BWATTHRR = (float)(int32_t)Data / WattHR_LSB;
	return *BWATTHRR;
}
float ADE7880_Get_CWATTHR(float *CWATTHRR )
{
	uint32_t Data;
	ADE7880_Read32(CWATTHR, &Data);
	*CWATTHRR = (float)(int32_t)Data / WattHR_LSB;
	return *CWATTHRR;
}
void ADE7880_Get_PWFACTOR(float *PF,float *PF_A,float *PF_B,float *PF_C)
{
	

	uint32_t Data;
	ADE7880_Read32(FPF, &Data);
	*PF = (float)(int32_t)Data;
  //uint16_t PF_A;
	uint16_t Data1,data2,data3;
	ADE7880_Read16(APF, &Data1);
	ADE7880_Read16(BPF, &data2);
	ADE7880_Read16(CPF, &data3);
	*PF_A=(float)(int16_t)Data1/32768;
	*PF_B=(float)(int16_t)data2/32768;
	*PF_C=(float)(int16_t)data3/32768;
	//*PF_A=(float)(int16_t)Data1;
}

void SAG_detection()
{




}











uint64_t ADE7880_Get_AFWATTHR(float *AFWATTHRR )
{
	uint32_t Data;
	ADE7880_Read32(AFWATTHR, &Data);
	*AFWATTHRR = (float)(int32_t)Data / WattHR_LSB;
	return *AFWATTHRR;
}

uint64_t ADE7880_Get_BFWATTHR(float *BFWATTHRR )
{
	uint32_t Data;
	ADE7880_Read32(BFWATTHR, &Data);
	*BFWATTHRR = (float)(int32_t)Data / WattHR_LSB;
	return *BFWATTHRR;
}
uint64_t ADE7880_Get_CFWATTHR(float *CFWATTHRR )
{
	uint32_t Data;
	ADE7880_Read32(CFWATTHR, &Data);
	*CFWATTHRR = (float)(int32_t)Data / WattHR_LSB;
	return *CFWATTHRR;


}


uint64_t ADE7880_Get_AFVARHR(float *AFVARHR1)
{   uint32_t Data;
	ADE7880_Read32(AFVARHR, &Data);
	*AFVARHR1 = (float)(int32_t)Data / WattHR_LSB;
	return *AFVARHR1;
}


uint64_t ADE7880_Get_BFVARHR(float *BFVARHR1)
{   uint32_t Data;
	ADE7880_Read32(BFVARHR, &Data);
	*BFVARHR1 = (float)(int32_t)Data / WattHR_LSB;
	return *BFVARHR1;
}


uint64_t ADE7880_Get_CFVARHR(float *CFVARHR1)
{   uint32_t Data;
	ADE7880_Read32(CFVARHR, &Data);
	*CFVARHR1 = (float)(int32_t)Data / WattHR_LSB;
	return *CFVARHR1;
}

void ADE7880_Get_AVAHR(float *AVAHR1)
{

uint32_t Data;
	ADE7880_Read32(AVAHR, &Data);
	*AVAHR1 = (float)(int32_t)Data / WattHR_LSB;
	
}

void ADE7880_Get_BVAHR(float *BVAHR1)
{

uint32_t Data;
	ADE7880_Read32(BVAHR, &Data);
	*BVAHR1 = (float)(int32_t)Data / WattHR_LSB;
	
}

void ADE7880_Get_CVAHR(float *CVAHR1)
{
	
uint32_t Data;
	ADE7880_Read32(CVAHR, &Data);
	*CVAHR1 = (float)(int32_t)Data / WattHR_LSB;
	
}

void ADE7880_Get_FWATT(float *FWATT1)
{
	
uint32_t Data;
	ADE7880_Read32(FWATT, &Data);
	*FWATT1 = (float)(int32_t)Data / WattHR_LSB;
	
}
void ADE7880_Get_FVAR(float *FVAR1)
{
	
uint32_t Data;
	ADE7880_Read32(FVAR, &Data);
	*FVAR1 = (float)(int32_t)Data / WattHR_LSB;
	
}



void ADE7880_Get_Har_Pwr_THD(uint8_t Ph, uint8_t x, uint8_t y, uint8_t z, float *H_x, float *H_y, float *H_z, float *APwr, float *RPwr, float *THDV, float *THDI)
{
	uint32_t Data;
	
	switch (Ph)
	{
		case PhA: ADE7880_Write16(HCONFIG, HSTIME | ACTPHSEL_A | HPHASE_A); break;
		case PhB: ADE7880_Write16(HCONFIG, HSTIME | ACTPHSEL_B | HPHASE_B); break;
		case PhC: ADE7880_Write16(HCONFIG, HSTIME | ACTPHSEL_C | HPHASE_C); break;
		default : ADE7880_Write16(HCONFIG, HSTIME | ACTPHSEL_A | HPHASE_A); break;
	}

	ADE7880_Write08(HX, x);
	ADE7880_Write08(HY, y);
	ADE7880_Write08(HZ, z);
	
	ADE7880_Write32(STATUS0, HREADY);
	osThreadFlagsWait(ADE7880_IRQ0, osFlagsWaitAny, osWaitForever);
	
	ADE7880_Read32(HXVRMS, &Data);
	*H_x = (float)Data / Volt_LSB;
	
	ADE7880_Read32(HYVRMS, &Data);
	*H_y = (float)Data / Volt_LSB;
	
	ADE7880_Read32(HZVRMS, &Data);
	*H_z = (float)Data / Volt_LSB;
	
	ADE7880_Read32(FWATT, &Data);
	*APwr = (float)(int32_t)Data / Watt_LSB;
	
	ADE7880_Read32(FVAR, &Data);
	*RPwr = (float)(int32_t)Data / VAR_LSB;
	
	ADE7880_Read32(VTHD, &Data);
	*THDV = (float)(int32_t)Data / (1 << 21);
	
	ADE7880_Read32(ITHD, &Data);
	*THDI = (float)(int32_t)Data / (1 << 21);
}

void ADE7880_Get_Har(uint8_t Ph, uint8_t x, uint8_t y, uint8_t z, float *H_x, float *H_y, float *H_z)
{
	uint32_t Data;
	
	switch (Ph)
	{
		case PhA: ADE7880_Write16(HCONFIG, HSTIME | ACTPHSEL_A | HPHASE_A); break;
		case PhB: ADE7880_Write16(HCONFIG, HSTIME | ACTPHSEL_B | HPHASE_B); break;
		case PhC: ADE7880_Write16(HCONFIG, HSTIME | ACTPHSEL_C | HPHASE_C); break;
		default : ADE7880_Write16(HCONFIG, HSTIME | ACTPHSEL_A | HPHASE_A); break;
	}
	
	ADE7880_Write08(HX, x);
	ADE7880_Write08(HY, y);
	ADE7880_Write08(HZ, z);
	
	ADE7880_Write32(STATUS0, HREADY);
	osThreadFlagsWait(ADE7880_IRQ0, osFlagsWaitAny, osWaitForever);
	
	ADE7880_Read32(HXVRMS, &Data);
	*H_x = (float)Data / Volt_LSB;
	
	ADE7880_Read32(HYVRMS, &Data);
	*H_y = (float)Data / Volt_LSB;
	
	ADE7880_Read32(HZVRMS, &Data);
	*H_z = (float)Data / Volt_LSB;
}
//




