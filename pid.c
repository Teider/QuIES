#include "driverlib/sysctl.h"
#include "driverlib/timer.h"

#include "inc/hw_memmap.h"

#include "serial.h"

#include "pid.h"

float erroX = 0;
float erroX_sum = 0;
int64_t last_timeX = -1;

float erroY = 0;
float erroY_sum = 0;
int64_t last_timeY = -1;

int kP = 1, kI = 0;

float calculaAjusteEstabilidadeRoll(float setpoint, float input) {
	
	erroX = input - setpoint;
	
	int64_t current_time = TimerLoadGet64(WTIMER0_BASE);
	if (last_timeX == -1) last_timeX = current_time;
	float delta_t = ((float)(current_time - last_timeX)) / ((float) SysCtlClockGet());
	last_timeX = current_time;
	
	erroX_sum += (erroX * delta_t);
	
	//if (erroX > 2) return 1;
	//else if (erroX < -2) return -1;
	//else return 0;
	
	return (erroX * kP + erroX_sum * kI);
}


float calculaAjusteEstabilidadePitch(float setpoint, float input) {
	
	erroY = input - setpoint;
	
	int64_t current_time = TimerLoadGet64(WTIMER0_BASE);
	if (last_timeX == -1) last_timeX = current_time;
	float delta_t = ((float)(current_time - last_timeY)) / ((float) SysCtlClockGet());
	last_timeY = current_time;
	
	
	erroY_sum += (erroY * delta_t);
	
	//if (erroY > 2) return 1;
	//else if (erroY < -2) return -1;
	//else return 0;
	
	return (erroY * kP + erroY_sum * kI);
}
