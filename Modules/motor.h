#ifndef __MOTOR_H
#define __MOTOR_H

#include "stdint.h"
#include "ti_msp_dl_config.h"

/* 电机对象结构体：封装状态与硬件资源 */
typedef struct {
    // 运行状态
    int16_t tar;       
    int16_t count;     
    uint8_t state;  
    float current;
    
    // 硬件映射参数
    GPTIMER_Regs* timer;       // 定时器实例
    GPIO_Regs* port;        // GPIO 端口
    uint32_t      dirPin;      // 方向引脚
    uint32_t      stepPin;     // 脉冲引脚
    float         spr;         // Steps Per Revolution (步数/圈)
} MOTOR_t;

// 声明电机实例
extern MOTOR_t StepX;
extern MOTOR_t StepY;

// 通用接口函数
void StepMotor_SetAngle(MOTOR_t* motor, float target);
void StepMotor_Process(MOTOR_t* motor);

#endif