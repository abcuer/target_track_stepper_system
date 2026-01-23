#include "timer.h"

static void tim0_init(void)
{
	NVIC_ClearPendingIRQ(TIMER_0_INST_INT_IRQN);
	NVIC_EnableIRQ(TIMER_0_INST_INT_IRQN);	
}

static void tim1_init(void)
{
	NVIC_ClearPendingIRQ(TIMER_1_INST_INT_IRQN);
	NVIC_EnableIRQ(TIMER_1_INST_INT_IRQN);
}

static void tim2_init(void)
{
	NVIC_ClearPendingIRQ(TIMER_2_INST_INT_IRQN);
	NVIC_EnableIRQ(TIMER_2_INST_INT_IRQN);
}

void TimerDeviceInit(void)
{
	tim0_init();
	tim1_init();
	tim2_init();
}