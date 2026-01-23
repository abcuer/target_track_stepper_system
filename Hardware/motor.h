#ifndef _motor_h
#define _motor_h
#include "stdint.h"

typedef struct
{
    int16_t tar;       
    int16_t count;     
    int16_t state;  
	float 	current;
} MOTOR_t;

void StepXSetAngle(float target);
void StepYSetAngle(float target);

#endif