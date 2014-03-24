#include "motors.h"
#include "giroacel.h"
#include "pid.h"

#include "control.h"

float ajuste_motores[4];
float output = 0.0;

float fator0 = 0.3;
float fator1 = 0.3;
float fator2 = 0.3;
float fator3 = 0.3;

float setpointRoll = 180.0, setpointPitch = 180.0, setpointYaw = 180.0;
//float setpointAltura = 100.0;

extern float roll, pitch, yaw;

extern bool mover_frente;
extern bool mover_tras;
extern bool mover_esquerda;
extern bool mover_direita;

void atualizaControle() {
	
	output = 0.0;
	
	output = calculaAjusteEstabilidadeRoll(setpointRoll, roll);
	
	for (int i = 0; i < 4; i++) {
		ajuste_motores[i] = 0.0;
	}
	
	//if (mover_esquerda || mover_direita) {
		ajuste_motores[0] -= (output * fator0);
		ajuste_motores[1] += (output * fator1);
		ajuste_motores[2] += (output * fator2);
		ajuste_motores[3] -= (output * fator3);
	//}
	
	output = calculaAjusteEstabilidadePitch(setpointPitch, pitch);

	//if (mover_frente || mover_tras) {
		ajuste_motores[0] -= (output * fator0);
		ajuste_motores[1] -= (output * fator1);
		ajuste_motores[2] += (output * fator2);
		ajuste_motores[3] += (output * fator3);
	//}
	
	//if (mover_frente || mover_tras || mover_direita || mover_esquerda) {
		
		adjustCompensacao(0, (int)ajuste_motores[0]);
		adjustCompensacao(1, (int)ajuste_motores[1]);
		adjustCompensacao(2, (int)ajuste_motores[2]);
		adjustCompensacao(3, (int)ajuste_motores[3]);
	//}
	
}
