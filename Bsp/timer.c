#include "timer.h"
#include "motor.h"
#include "ti_msp_dl_config.h"

void TimerDeviceInit(void)
{
	// TIM2
	NVIC_ClearPendingIRQ(TIMER_2_INST_INT_IRQN);
	NVIC_EnableIRQ(TIMER_2_INST_INT_IRQN);
}


