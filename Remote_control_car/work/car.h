#ifndef _CAR_H
#define _CAR_H

void GPIOB_Init(void);
void forward(void);
void back(void);
void turn_left(void);
void turn_left1(void);
void turn_right(void);
void turn_right1(void);
void stop(void);
void setpwm(int i);
void setpwm1(int i,int j);

#endif

