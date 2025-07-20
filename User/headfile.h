#ifndef _headfile_h
#define _headfile_h

#pragma once

#include "ti_msp_dl_config.h"

#include "math.h"
#include "stdio.h"
#include "string.h"
#include "stdint.h"
#include "stdlib.h"
/******************* BSP **********************/
#include "board.h"
#include "timer.h"
/***************** Hardware *******************/
#include "key.h"
#include "led.h"
#include "buzzer.h"
#include "oled.h"
#include "stepper_motor.h"
#include "hc05.h"
/***************** Control *******************/
#include "pid.h"
/***************** App ***********************/
#include "app_utils.h"
#include "app_task.h"
#include "test.h"


extern uint8_t motor_left_dir;
extern uint8_t motor_right_dir;
extern float speedA;
extern float speedB;

extern float basespeed;
extern uint8_t Task;
extern uint8_t first_flag;
extern uint8_t start_flag;
extern float angle_initial;
extern uint8_t workstep;

#endif