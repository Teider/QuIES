#include "motors.h"
#include "giroacel.h"
#include "pid.h"

#include "control.h"

int ajuste_motores[4];
double output = 0.0;

double fator = 0.714285;

double setpointRoll = 50.0, setpointPitch = 50.0, setpointYaw = 50.0;
double setpointAltura = 50.0;

extern double roll, pitch, yaw;

extern bool mover_frente;
extern bool mover_tras;
extern bool mover_esquerda;
extern bool mover_direita;

void atualizaControle() {
	
	output = 0.0;
	
	output = calculaAjusteEstabilidadeRoll(setpointRoll, (roll / 3.6));
	
	
	if (mover_esquerda || mover_direita) {
		ajuste_motores[0] -= (int)(output * fator);
		ajuste_motores[1] += (int)(output * fator);
		ajuste_motores[2] += (int)(output * fator);
		ajuste_motores[3] -= (int)(output * fator);
	}
	
	output = calculaAjusteEstabilidadePitch(setpointPitch, (pitch / 3.6));

	if (mover_frente || mover_tras) {
		ajuste_motores[0] -= (int)(output * fator);
		ajuste_motores[1] -= (int)(output * fator);
		ajuste_motores[2] += (int)(output * fator);
		ajuste_motores[3] += (int)(output * fator);
	}
	
	if (mover_frente || mover_tras || mover_direita || mover_esquerda) {
		for (int i = 0; i < 4; i++)	{
			adjustSpeed(i, ajuste_motores[i]);
		}
	}
	
}
