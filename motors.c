#include <stdint.h>
#include <stdbool.h>

#include "inc/hw_ints.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"

#include "driverlib/gpio.h"
#include "driverlib/rom.h"
#include "driverlib/sysctl.h"
#include "driverlib/timer.h"

#include "motors.h"

#include "serial.h"

#define ACCEL_SPEED (1)

#define SPEED_STEP (1)

#define NUM_VELOCIDADES_MOTOR 200
#define	MAX_COMP 							20


bool debug_red = true;
bool debug_blue = true;
bool debug_green = true;

int counter_ppm = 0;

bool motoresInicializados = true;
int counterInici = 0;

int counter_atualiza_velocidade = 0;

extern bool decolando;
extern bool no_ar;

typedef struct motor {
	int id;
	int velocidade_atual;
	int velocidade_alvo;
	int velocidade_base;
	int compensacao;
	bool acelerando;
	bool desacelerando;
} motor;

motor motores[4];

void inicializa_motores(void) {
	
	for (int i = 0; i < 4; i++) {
		motores[i].id = i;
		motores[i].velocidade_atual = 0;
		motores[i].velocidade_alvo = 0;
		motores[i].velocidade_base = 0;
		motores[i].compensacao = 0;
		motores[i].acelerando = false;
		motores[i].desacelerando = false;
	}
	
	//
  // Enable the GPIO port that is used for the on-board LED.
  //
  ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);

  //
  // Enable the GPIO pins for the LED (PF1 & PF2).
  //
	ROM_GPIOPinTypeGPIOOutput(GPIO_PORTB_BASE, GPIO_PIN_0);
	ROM_GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1);
	ROM_GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_2);
  ROM_GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_3);
}

void changeSpeed(int id_motor, int nova_velocidade, int compensacao) {
	
	if (nova_velocidade > NUM_VELOCIDADES_MOTOR) nova_velocidade = NUM_VELOCIDADES_MOTOR;
	if (nova_velocidade < 0) nova_velocidade = 0;
	
	if (compensacao > MAX_COMP) compensacao = MAX_COMP;
	if (compensacao < -MAX_COMP) compensacao = -MAX_COMP;
	
	if ((nova_velocidade + compensacao) > NUM_VELOCIDADES_MOTOR) compensacao = 0;
	if ((nova_velocidade + compensacao) < 0) compensacao = 0;
	
	motores[id_motor].velocidade_base = nova_velocidade;
	motores[id_motor].compensacao = compensacao;
	motores[id_motor].velocidade_alvo = nova_velocidade + compensacao;
	
	if (motores[id_motor].velocidade_alvo > motores[id_motor].velocidade_atual) {
		motores[id_motor].acelerando = true;
		motores[id_motor].desacelerando = false;
	} else if (motores[id_motor].velocidade_alvo < motores[id_motor].velocidade_atual) {
		motores[id_motor].acelerando = false;
		motores[id_motor].desacelerando = true;
	} else {
		motores[id_motor].acelerando = false;
		motores[id_motor].desacelerando = false;
	}
	
	sendMotorVelocity(id_motor, motores[id_motor].velocidade_atual, motores[id_motor].compensacao);
}

void update_ppm(void) {
	if(counter_ppm == 0) {
		GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_0, GPIO_PIN_0);
		GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1, GPIO_PIN_1);
		GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, GPIO_PIN_2);
		GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_3, GPIO_PIN_3);
	}
	
	if(counter_ppm == NUM_VELOCIDADES_MOTOR+motores[0].velocidade_atual) {
		GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_0, 0);
	}
	if(counter_ppm == NUM_VELOCIDADES_MOTOR+motores[1].velocidade_atual) {
		GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1, 0);
	}
	if(counter_ppm == NUM_VELOCIDADES_MOTOR+motores[2].velocidade_atual) {
		GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, 0);
	}
	if(counter_ppm == NUM_VELOCIDADES_MOTOR+motores[3].velocidade_atual) {
		GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_3, 0);
	}
			
	counter_ppm = counter_ppm + 1;
			
	if(counter_ppm == 4000){
		counter_ppm = 0;
		
		counter_atualiza_velocidade++;
		
		if (counter_atualiza_velocidade == 5) {
			counter_atualiza_velocidade = 0;
			
			for (int i = 0; i < 4; i++) {
				if (motores[i].acelerando) motores[i].velocidade_atual++;
				if (motores[i].desacelerando) motores[i].velocidade_atual--;
				if (motores[i].velocidade_atual == motores[i].velocidade_alvo) {
					motores[i].acelerando = false;
					motores[i].desacelerando = false;
					
					if (decolando) {
						decolando = false;
						no_ar = true;
					}
				}
			}
		}
		
		if(counterInici <= 1){
			counterInici += 1;
		}
		if(counterInici == 1) {
			
			//changeSpeed(motores[0].id, 18, 0);
			//changeSpeed(motores[2].id, 4, 0);
			motoresInicializados = true;
		}
		
	}
  //
  // Clear the timer interrupt.
  //
  ROM_TimerIntClear(TIMER1_BASE, TIMER_TIMA_TIMEOUT);
}

void accelerate(int id_motor) {
	changeSpeed(id_motor, motores[id_motor].velocidade_base + SPEED_STEP, motores[id_motor].compensacao);
}

void decelerate(int id_motor) {
	changeSpeed(id_motor, motores[id_motor].velocidade_base - SPEED_STEP, motores[id_motor].compensacao);
}

void adjustCompensacao(int id_motor, int vel) {
	changeSpeed(id_motor, motores[id_motor].velocidade_base, vel);
}

void panic() {
	for (int i = 0; i < 4; i++) {
		motores[i].velocidade_atual = 0;
		motores[i].acelerando = false;
		motores[i].desacelerando = false;
	}
}
