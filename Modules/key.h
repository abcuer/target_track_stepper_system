#ifndef __KEY_H
#define __KEY_H
#include "ti_msp_dl_config.h"

void Key_Scan(void);
uint8_t Key_GetState(void);
uint8_t Key_GetNum(void);

#endif
