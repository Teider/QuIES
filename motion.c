#include "motion.h"
#include "message_types.h"
#include "giroacel.h"
#include "motors.h"
#include "serial.h"

char* packageReceived;

bool moving = false;
bool decolando = false;

extern bool debug_red;
extern bool debug_blue;
extern bool debug_green;

bool mover_frente = false;
bool mover_tras = false;
bool mover_esquerda = false;
bool mover_direita = false;

extern bool init_control;

extern bool no_chao;

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

	decolando = true;
	//changeSpeed(0,100,0);
	//changeSpeed(1,100,0);
	//changeSpeed(2,100,0);
	//changeSpeed(3,100,0);
	
}

void move_pousar(){
	
	mover_frente = false;
	mover_tras = false;
	mover_esquerda = false;
	mover_direita = false;
	
	init_control = false;
	
	decolando = false;
	
	void panic();
	
	no_chao = true;
	
}

void move_esquerda() {
	
	mover_frente = false;
	mover_tras = false;
	mover_esquerda = true;
	mover_direita = true;
	
	
}

void move_direita(){
	
	mover_frente = false;
	mover_tras = false;
	mover_esquerda = true;
	mover_direita = true;
	
}

void move_frente(){
	mover_frente = true;
	mover_tras = true;
	mover_esquerda = false;
	mover_direita = false;
}

void move_tras(){
	mover_frente = true;
	mover_tras = true;
	mover_esquerda = false;
	mover_direita = false;
}

void move_gir_horario(){
	
	
	init_control = true;

}

void move_gir_anti(){
	

	init_control = false;
	
}

void move_subir(){
	
	accelerate(0);
	accelerate(1);
	accelerate(2);
	accelerate(3);

}

void move_descer(){
	
	decelerate(0);
	decelerate(1);
	decelerate(2);
	decelerate(3);
}


