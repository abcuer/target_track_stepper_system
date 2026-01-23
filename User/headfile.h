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
#include "motor.h"
#include "usart.h"
/***************** Control *******************/
#include "pid.h"
/***************** App ***********************/
#include "app_utils.h"
#include "app_task.h"
#include "test.h"

#endif