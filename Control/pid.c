#include "pid.h"

void PID_Update(pid_t *pid, uint32_t mode, float p, float i, float d)
{
	pid->mode = mode;
	pid->p = p;
	pid->i = i;
	pid->d = d;
}

void PID_Reset(pid_t *pid)
{
    pid->error[0] = 0;
    pid->error[1] = 0;
    pid->error[2] = 0;
    pid->pout = 0;
    pid->iout = 0;
    pid->dout = 0;
    pid->out = 0;
    pid->now = 0;
    pid->target = 0;
}

void PID_Calculate(pid_t *pid)
{
	pid->error[0] = pid->target - pid->now;

	if(pid->mode == DELTA_PID)  
	{
		pid->pout = pid->p * (pid->error[0] - pid->error[1]);
		pid->iout = pid->i * pid->error[0];
		pid->dout = pid->d * (pid->error[0] - 2 * pid->error[1] + pid->error[2]);
		pid->out += pid->pout + pid->iout + pid->dout;
	}
	else if(pid->mode == POSITION_PID)  
	{
		pid->pout = pid->p * pid->error[0];
		pid->iout = pid->i * pid->error[0];
		pid->dout = pid->d * (pid->error[0] - pid->error[1]);
		pid->out = pid->pout + pid->iout + pid->dout;
	}

	pid->error[2] = pid->error[1];
	pid->error[1] = pid->error[0];
}

void PID_OutputLimit(pid_t *pid, float duty)
{
	if(pid->out >= duty)	pid->out = duty;
	if(pid->out <= -duty) pid->out = -duty;
}

uint8_t pid_flag = 0;
int speed_tar = 0;
int angle_tar = 0;
uint8_t Yaw_update = 0;



