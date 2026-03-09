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
#include "hc05.h"
#include "oled.h"
/***************** Control *******************/
#include "pid.h"
#include "ZDTControl.h"
/***************** App ***********************/
#include "app_utils.h"
#include "app_task.h"
#include "test.h"

#define SPEED_PID 	1
#define ANGLE1_PID 	2
#define ANGLE2_PID 	3
#define TRACK1_PID 	4
#define TRACK2_PID 	5

extern float basespeed;
extern uint8_t Task;
extern uint8_t first_flag;
extern uint8_t start_flag;
extern uint8_t workstep;

extern uint8_t rev_flag;
extern uint8_t rx_buf[MAX_FRAME_LEN];
extern uint8_t data_len;

#endif