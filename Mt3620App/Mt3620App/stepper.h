#pragma once

typedef enum { STOP, CW, CCW } motordir;

typedef struct {
	GPIO_Id pin1;
	int gpio1Fd;
	GPIO_Id pin2;
	int gpio2Fd;
	GPIO_Id pin3;
	int gpio3Fd;
	GPIO_Id pin4;
	int gpio4Fd;
	motordir direction;
	int stepsToGo;
	int stepcycle;
} stepper_t;

int STEPPER_Init(volatile stepper_t *s);
int STEPPER_Run(volatile stepper_t *s);
int STEPPER_SetSteps(volatile stepper_t *s, int steps);
int ConfigurePin(GPIO_Id pin, int pinNo);