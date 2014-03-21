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

#define ACCEL_SPEED (1)

#define SPEED_STEP (1)

bool debug_red = true;
bool debug_blue = true;
bool debug_green = true;

int counter_ppm = 0;

bool motoresInicializados = true;
int counterInici = 0;

int counter_atualiza_velocidade = 0;

typedef struct motor {
	int id;
	int velocidade_atual;
	int velocidade_alvo;
	bool acelerando;
	bool desacelerando;
	int compensacao;	
} motor;

motor motores[4];

void inicializa_motores(void) {
	
	for (int i = 0; i < 4; i++) {
		motores[i].id = i;
		motores[i].velocidade_atual = 0;
		motores[i].velocidade_alvo = 0;
		motores[i].acelerando = false;
		motores[i].desacelerando = false;
		motores[i].compensacao = 0;
	}
	//motores[0].compensacao = 10;
	
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

void changeSpeed(int id_motor, int nova_velocidade) {
	//nova_velocidade += motores[id_motor].compensacao;
	if (nova_velocidade > 100) nova_velocidade = 100;
	if (nova_velocidade < 0) nova_velocidade = 0;
	
	motores[id_motor].velocidade_atual = nova_velocidade;
	
	/*
	motores[id_motor].velocidade_alvo = nova_velocidade;
	if (nova_velocidade > motores[id_motor].velocidade_atual) {
		motores[id_motor].acelerando = true;
	} else if (nova_velocidade < motores[id_motor].velocidade_atual) {
		motores[id_motor].desacelerando = true;
	}
	*/
}

void update_ppm(void) {
	if(counter_ppm == 0) {
		GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_0, GPIO_PIN_0);
		GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1, GPIO_PIN_1);
		GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, GPIO_PIN_2);
		GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_3, GPIO_PIN_3);
	}
	
	if(counter_ppm == 100+motores[0].velocidade_atual){
		GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_0, 0);
	}
	if(counter_ppm == 100+motores[1].velocidade_atual){
		GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1, 0);
	}
	if(counter_ppm == 100+motores[2].velocidade_atual){
		GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, 0);
	}
	if(counter_ppm == 100+motores[3].velocidade_atual){
		GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_3, 0);
	}
			
	counter_ppm = counter_ppm + 1;
			
	if(counter_ppm == 2000){
		counter_ppm = 0;
		
		if(counterInici <= 1250){
			counterInici += 1;
		}
		if(counterInici == 1250){
			
			//motores[0].compensacao = 10;
			changeSpeed(motores[0].id, 9);
			changeSpeed(motores[2].id, 2);
			motoresInicializados = true;
		}
		/*
		if (motoresInicializados) {
		counter_atualiza_velocidade++;
		
		if (counter_atualiza_velocidade == 50) {
		
			counter_atualiza_velocidade = 0;
			
			for (int i = 0; i < 4; i++) {
				if (motores[i].acelerando) {
					motores[i].velocidade_atual += ACCEL_SPEED;
					if (motores[i].velocidade_atual >= motores[i].velocidade_alvo) {
						motores[i].velocidade_atual = motores[i].velocidade_alvo;
						motores[i].acelerando = false;
					}
				} else if (motores[i].desacelerando) {
					motores[i].velocidade_atual -= ACCEL_SPEED;
					if (motores[i].velocidade_atual <= motores[i].velocidade_alvo) {
						motores[i].velocidade_atual = 	motores[i].velocidade_alvo;
						motores[i].desacelerando = false;
					}
				}
			}
		}
		
		}
		*/
		
	}
  //
  // Clear the timer interrupt.
  //
  ROM_TimerIntClear(TIMER1_BASE, TIMER_TIMA_TIMEOUT);
}

void accelerate(int id_motor) {
	changeSpeed(id_motor, motores[id_motor].velocidade_atual += SPEED_STEP);
}

void decelerate(int id_motor) {
	changeSpeed(id_motor, motores[id_motor].velocidade_atual -= SPEED_STEP);
}

void adjustSpeed(int id_motor, int vel) {
	changeSpeed(id_motor, motores[id_motor].velocidade_atual += vel);
}
