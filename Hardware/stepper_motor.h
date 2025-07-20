#ifndef _stepper_motor_h
#define _stepper_motor_h

void Stepper_X_Start(uint8_t dir, uint16_t steps);
void Stepper_Y_Start(uint8_t dir, uint16_t steps);
void stepX(void);
void stepY(void);
int16_t angle_to_steps_X(float angle_deg);
int16_t angle_to_steps_Y(float angle_deg);
void Stepper_X_Goto_Angle(float target_angle_deg);
void Stepper_Y_Goto_Angle(float target_angle_deg);

#endif