#include "pid.h"

double erroX = 0;
double erroY = 0;

int kP = 1;

double calculaAjusteEstabilidadeRoll(double setpoint, double input) {
	
	erroX = input - setpoint;
	
	return erroX;
}


double calculaAjusteEstabilidadePitch(double setpoint, double input) {
	erroX = input - setpoint;
	
	return erroX;
}
