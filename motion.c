#include "motion.h"
#include "message_types.h"
#include "giroacel.h"
#include "motors.h"
#include "serial.h"

char* packageReceived;

bool moving = false;

extern bool debug_red;
extern bool debug_blue;
extern bool debug_green;

bool mover_frente = false;
bool mover_tras = false;
bool mover_esquerda = false;
bool mover_direita = false;

void getMotion(char* package) {
	
	packageReceived = package;
	
		switch (packageReceived[3]) {
		
			case MESSAGE_TYPE_PARAR:
				move_parar();
				break;
			case MESSAGE_TYPE_DECOLAR:
				move_decolar();
				break;
			case MESSAGE_TYPE_POUSAR:
				move_pousar();
				break;
			case MESSAGE_TYPE_ESQUERDA:
				move_esquerda();
				break;
			case MESSAGE_TYPE_DIREITA:
				move_direita();
				break;
			case MESSAGE_TYPE_FRENTE:
				move_frente();
				break;
			case MESSAGE_TYPE_TRAS:
				move_tras();
				break;
			case MESSAGE_TYPE_GIR_HORARIO:
				move_gir_horario();
				break;
			case MESSAGE_TYPE_GIR_ANTI:
				move_gir_anti();
				break;
			case MESSAGE_TYPE_SUBIR:
				move_subir();
				break;
			case MESSAGE_TYPE_DESCER:
				move_descer();
				break;
			
		}
}

void move_parar(){
	
//	while(getVelocidade != 0){
//		
//		decelerate(0);
//		decelerate(1);
//		decelerate(2);
//		decelerate(3);
//	}
}

void move_decolar(){

	
}

void move_pousar(){
	

	
}

void move_esquerda(){
	
	mover_frente = false;
	mover_tras = false;
	mover_esquerda = true;
	mover_direita = false;
	
}

void move_direita(){
	
	mover_frente = false;
	mover_tras = false;
	mover_esquerda = false;
	mover_direita = true;
	
}

void move_frente(){
	mover_frente = true;
	mover_tras = false;
	mover_esquerda = false;
	mover_direita = false;
}

void move_tras(){
	mover_frente = false;
	mover_tras = true;
	mover_esquerda = false;
	mover_direita = false;
}

void move_gir_horario(){
	

}

void move_gir_anti(){
	

}

void move_subir(){
	
	if (mover_frente || mover_esquerda) accelerate(0);
	if (mover_frente || mover_direita) accelerate(1);
	if (mover_tras || mover_direita) accelerate(2);
	if (mover_tras || mover_esquerda) accelerate(3);

}

void move_descer(){
	
	decelerate(0);
	decelerate(1);
	decelerate(2);
	decelerate(3);
}


