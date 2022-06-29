#include "modbus.h"
#include "cmsis_os.h"

#define SlaveAddr	1
#define RxBufferSize 6	 // SlaveAddr + Function + StartAddr + NbReadReg + CRC1 + CRC2
#define TxBufferSize 133 // SlaveAddr + Function + ByteCnt + 32*4 + CRC1 + CRC2

#define ReadRegistersFn 	0x03

#define IllegalFunction 	0x01
#define IllegalDataAddr 	0x02
#define IllegalDataValue	0x03

extern UART_HandleTypeDef UART1_Handle;

uint8_t NbRxData;
uint8_t NbTxData;
uint8_t RxData[RxBufferSize];
uint8_t TxData[TxBufferSize];

uint16_t CRC_gen(uint8_t* Data, uint8_t NbData);

void ModBus_Init(void)
{
	RS485_RW_Init();
	UART1_Init();
	RS485_RW_SetRead();
}

void ModBus_Process(uint32_t *Reg, uint8_t NbReg)
{
	uint16_t crc;
	
	HAL_UARTEx_ReceiveToIdle_IT(&UART1_Handle, (uint8_t*)RxData, RxBufferSize);
	osThreadFlagsWait(ModBus_RxCplt, osFlagsWaitAny, osWaitForever);
	
	// supported trame: SlaveAddr + Function + StartAddr + NbReadReg + CRC1 + CRC2
	if(NbRxData != 6)
	{
		return;
	}

	if(RxData[0] != SlaveAddr)
	{
		return;
	}
	
	crc = CRC_gen(RxData, NbRxData-2);
	if((RxData[NbRxData-2] == (uint8_t)crc) && (RxData[NbRxData-1] == (uint8_t)(crc >> 8)))
	{
		uint8_t Function = RxData[1];
		
		NbTxData = 0;
		TxData[NbTxData++] = SlaveAddr;
		TxData[NbTxData++] = Function;
		
		switch(Function)
		{
			case ReadRegistersFn:
			{
				uint8_t StartAddr = RxData[2];
				uint8_t NbReadReg = RxData[3];
				
				if(StartAddr > NbReg-1)
				{
					TxData[1] |= 0x80;
					TxData[NbTxData++] = IllegalDataAddr;
					break;
				}
				
				if((NbReadReg > 32) | (StartAddr+NbReadReg > NbReg))
				{
					TxData[1] |= 0x80;
					TxData[NbTxData++] = IllegalDataValue;
					break;
				}
				
				TxData[NbTxData++] = NbReadReg * 4;
				
				for(int i=0; i<NbReadReg; i++)
				{
					TxData[NbTxData++] = (*(Reg + StartAddr + i)) >> 24;
					TxData[NbTxData++] = (*(Reg + StartAddr + i)) >> 16;
					TxData[NbTxData++] = (*(Reg + StartAddr + i)) >> 8;
					TxData[NbTxData++] = (*(Reg + StartAddr + i));
				}
				
				break;
			}
			default :
			{
				//Illegal function
				TxData[1] |= 0x80;
				TxData[NbTxData++] = IllegalFunction;
				break;
			}
		}
		
		crc = CRC_gen(TxData, NbTxData);
		TxData[NbTxData++] = crc;
		TxData[NbTxData++] = crc >> 8;
		
		RS485_RW_SetWrite();
		HAL_UART_Transmit_IT(&UART1_Handle, (uint8_t*)TxData, NbTxData);
		osThreadFlagsWait(ModBus_TxCplt, osFlagsWaitAny, osWaitForever);
		RS485_RW_SetRead();
	}
	// If crc error -> No response returned
}

uint16_t CRC_gen(uint8_t* Data, uint8_t NbData)
{
	uint16_t crc = 0xffff;
	
	for(int i=0; i<NbData; i++)
	{
		crc ^= Data[i];
		for(int j=8; j !=0; j--)
		{
			if((crc & 0x0001) !=0)
			{
				crc >>= 1;
				crc ^= 0xa001;
			}
			else
			{
				crc >>= 1; 
			}
		}
	}
	return crc;
}
