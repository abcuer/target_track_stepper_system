#include "motor.h"
#include <math.h>

/* 1. 初始化电机对象，消除重复的全局变量 */
// X 轴配置：400 步/圈
MOTOR_t StepX = {
    .timer = TIMER_0_INST,
    .port = StepX_PORT, 
    .dirPin = StepX_DirX_PIN,
    .stepPin = StepX_MotorX_PIN,
    .spr = 400.0f
};

// Y 轴配置：2170 步/圈
MOTOR_t StepY = {
    .timer = TIMER_1_INST,
    .port = StepY_PORT,
    .dirPin = StepY_DirY_PIN,
    .stepPin = StepY_MotorY_PIN,
    .spr = 2170.0f
};

/* 2. 内部工具函数：角度转步数 */
static int16_t angle_to_steps(float angle, float spr) {
    return (int16_t)(angle * spr / 360.0f);
}

/* 3. 统一的角度控制逻辑 */
void StepMotor_SetAngle(MOTOR_t* motor, float target) {
    float err = target - motor->current;
    uint8_t dir = (err >= 0) ? 1 : 0;
    
    motor->tar = angle_to_steps(fabsf(err), motor->spr);
    motor->count = 0;
    motor->state = 0;
    motor->current = target;

    // 硬件控制：方向
    if (dir) DL_GPIO_setPins(motor->port, motor->dirPin);
    else     DL_GPIO_clearPins(motor->port, motor->dirPin);

    // 硬件控制：启动定时器
    DL_Timer_startCounter(motor->timer);
}

/* 4. 统一的中断处理逻辑 */
void StepMotor_Process(MOTOR_t* motor) 
{
    if (motor->state == 0) {
        DL_GPIO_setPins(motor->port, motor->stepPin);
        motor->state = 1;
    } else {
        DL_GPIO_clearPins(motor->port, motor->stepPin);
        motor->state = 0;
        
        if (++motor->count >= motor->tar) {
            DL_Timer_stopCounter(motor->timer);
            motor->count = 0;
        }
    }
}

/* 5. 中断服务函数：仅负责分发 */
void TIMER_0_INST_IRQHandler(void) 
{
    if (DL_TimerA_getPendingInterrupt(TIMER_0_INST) == DL_TIMER_IIDX_ZERO) 
	{
        StepMotor_Process(&StepX);
    }
}

void TIMER_1_INST_IRQHandler(void) 
{
    // 建议统一使用 IIDX_ZERO 或根据 SysConfig 确认标志位
    if (DL_TimerG_getPendingInterrupt(TIMER_1_INST) == DL_TIMER_IIDX_LOAD)
	{
        StepMotor_Process(&StepY);
    }
}

