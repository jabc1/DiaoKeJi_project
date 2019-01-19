
#ifndef __NRFTASK_H__
#define __NRFTASK_H__
#include "stm32f10x.h"
#include "dk_conf.h"

void NRF_S_CRTL(void);
void NRF_S_RX(void);






void NRFTask_RX(u8 *RxBuf,u8 *TxBuf);
void NRFTask(void);

#endif



