#include <applibs/gpio.h>
#include <applibs/log.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>  //File operations e.g. close
#include "stepper.h"

//Stepper motor controller
const int stepcyclesize = 8;
const char stepValues[8] = { 0b1000, 0b1100, 0b0100, 0b0110, 0b0010, 0b0011, 0b0001, 0b1001 };

int STEPPER_Init(volatile stepper_t * s)
{
	if ((s->gpio1Fd = ConfigurePin(s->pin1, 1)) < 0) return -1;
	if ((s->gpio2Fd = ConfigurePin(s->pin2, 2)) < 0) return -1;
	if ((s->gpio3Fd = ConfigurePin(s->pin3, 3)) < 0) return -1;
	if ((s->gpio4Fd = ConfigurePin(s->pin4, 4)) < 0) return -1;
	return 0;
}

int ConfigurePin(GPIO_Id pin,int pinNo)
{
	int fd = -1;
	Log_Debug("Configuring stepper pin%d (%d) as output\n", pinNo, pin);
	fd = GPIO_OpenAsOutput(pin, GPIO_OutputMode_PushPull, GPIO_Value_Low);
	if (fd < 0) {
		Log_Debug("ERROR: Could not open stepper GPIO: %s (%d).\n", strerror(errno), errno);
	}
	return fd;
}

int SetPin(int pinFD, int pinNo, GPIO_Value_Type value) {
	Log_Debug("Setting stepper pin%d\n", pinNo);
	int result = GPIO_SetValue(pinFD, value);
	if (result != 0) {
		Log_Debug("ERROR: Could not set LED output value: %s (%d).\n", strerror(errno), errno);
		return -1;
	}
	return 0;
}

int STEPPER_Run(volatile stepper_t * s)
{
	if (s->direction == STOP) { return 0; }
	if (s->direction == CW) {
		if (s->stepcycle == stepcyclesize) {
			s->stepcycle = 0;
		}
		else {
			s->stepcycle += 1;
		}
	}
	if (s->direction == CCW) {
		if (s->stepcycle == 0) {
			s->stepcycle = stepcyclesize;
		} else {
			s->stepcycle -= 1;
		}
	}
	SetPin(s->gpio1Fd, 1, GPIO_Value_Low);
	SetPin(s->gpio2Fd, 2, GPIO_Value_Low);
	SetPin(s->gpio3Fd, 3, GPIO_Value_Low);
	SetPin(s->gpio4Fd, 4, GPIO_Value_Low);

	SetPin(s->gpio1Fd, 1, ((stepValues[s->stepcycle] & 0b1000) > 0) ? GPIO_Value_High : GPIO_Value_Low);
	SetPin(s->gpio2Fd, 2, ((stepValues[s->stepcycle] & 0b0100) > 0) ? GPIO_Value_High : GPIO_Value_Low);
	SetPin(s->gpio3Fd, 3, ((stepValues[s->stepcycle] & 0b0010) > 0) ? GPIO_Value_High : GPIO_Value_Low);
	SetPin(s->gpio4Fd, 4, ((stepValues[s->stepcycle] & 0b0001) > 0) ? GPIO_Value_High : GPIO_Value_Low);

	s->stepsToGo--;
	if (s->stepsToGo <= 0) { s->direction = STOP; }

	return 0;
}

void CloseFdAndPrintError(int fd, const char *fdName)
{
	if (fd >= 0) {
		int result = close(fd);
		if (result != 0) {
			Log_Debug("ERROR: Could not close fd %s: %s (%d).\n", fdName, strerror(errno), errno);
		}
	}
}

int STEPPER_Terminate(volatile stepper_t *s)
{
	CloseFdAndPrintError(s->gpio1Fd, "Pin1");
	CloseFdAndPrintError(s->gpio2Fd, "Pin2");
	CloseFdAndPrintError(s->gpio3Fd, "Pin3");
	CloseFdAndPrintError(s->gpio4Fd, "Pin4");

	return 0;
}

int STEPPER_SetSteps(volatile stepper_t *s, int steps)
{
	s->stepsToGo = abs(steps);
	if (steps > 0) s->direction = CW;
	if (steps < 0) s->direction = CCW;
	if (steps == 0) s->direction = STOP;
	return 0;
}
