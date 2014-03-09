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

void getMotion(char* package){
	
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
	

	
}

void move_direita(){
	

	
}

void move_frente(){

	
}

void move_tras(){
	
}

void move_gir_horario(){
	

}

void move_gir_anti(){
	

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


