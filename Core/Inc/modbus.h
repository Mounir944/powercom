#include "hardware.h"

#define ModBus_TxCplt	0x00000001
#define ModBus_RxCplt	0x00000002

void ModBus_Init(void);
void ModBus_Process(uint32_t *Reg, uint8_t NbReg);
