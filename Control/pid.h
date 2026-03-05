#ifndef __PID_H
#define __PID_H
#include "stdint.h"

enum
{
  POSITION_PID = 0, 
  DELTA_PID,         
};

typedef struct
{
	float target;	
	float now;
	float error[3];		
	float p,i,d;
	float pout, dout, iout;
	float out;   
	
	uint32_t mode;

}pid_t;

void PID_Update(pid_t *pid, uint32_t mode, float p, float i, float d);
void PID_Reset(pid_t *pid);
void PID_Calculate(pid_t *pid);
void PID_OutputLimit(pid_t *pid, float duty);

#endif
