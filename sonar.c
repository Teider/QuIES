#include <stdint.h>

#include "sonar.h"

#include "driverlib/interrupt.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/sysctl.h"
#include "driverlib/timer.h"

#include "inc/hw_ints.h"
#include "inc/hw_memmap.h"

#include <math.h>

uint32_t counter_sonar_cima, start_sonar_cima;
uint32_t counter_sonar_baixo, start_sonar_baixo;
uint32_t counter_sonar_frente, start_sonar_frente;
uint32_t counter_sonar_tras, start_sonar_tras;
uint32_t counter_sonar_esquerda, start_sonar_esquerda;
uint32_t counter_sonar_direita, start_sonar_direita;

bool current_PD1 = false;
bool current_PD2 = false;
bool current_PD3 = false;
bool current_PE1 = false;
bool current_PE2 = false;
bool current_PE3 = false;

uint32_t sonar_cima_mm = 0;
uint32_t sonar_baixo_mm = 0;
uint32_t sonar_frente_mm = 0;
uint32_t sonar_tras_mm = 0;
uint32_t sonar_esquerda_mm = 0;
uint32_t sonar_direita_mm = 0;

void verificaMedidas() {
	
	int32_t statusGPIOD = GPIOPinRead(GPIO_PORTD_BASE, GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3);
	int32_t statusGPIOE = GPIOPinRead(GPIO_PORTE_BASE, GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3);
	
	if ((statusGPIOD & GPIO_PIN_1) && !current_PD1) { //Status do PD1 era 1, foi para 0
		current_PD1 = true;
		start_sonar_cima = TimerValueGet(TIMER0_BASE, TIMER_A);
	} 
	if (!(statusGPIOD & GPIO_PIN_1) && (current_PD1)) { // Status do PD1 era 1, foi para 0
		current_PD1 = false;
		counter_sonar_cima = TimerValueGet(TIMER0_BASE, TIMER_A) - start_sonar_cima;
	}
	
	if ((statusGPIOD & GPIO_PIN_2) && !current_PD2) { //Status do PD2 era 1, foi para 0
		current_PD2 = true;
		start_sonar_baixo = TimerValueGet(TIMER0_BASE, TIMER_A);
	} 
	if (!(statusGPIOD & GPIO_PIN_2) && (current_PD2)) { // Status do PD2 era 1, foi para 0
		current_PD2 = false;
		counter_sonar_baixo = TimerValueGet(TIMER0_BASE, TIMER_A) - start_sonar_baixo;
	}
	
	if ((statusGPIOD & GPIO_PIN_3) && !current_PD3) { //Status do PD3 era 1, foi para 0
		current_PD3 = true;
		start_sonar_frente = TimerValueGet(TIMER0_BASE, TIMER_A);
	} 
	if (!(statusGPIOD & GPIO_PIN_3) && (current_PD3)) { // Status do PD3 era 1, foi para 0
		current_PD3 = false;
		counter_sonar_frente = TimerValueGet(TIMER0_BASE, TIMER_A) - start_sonar_frente;
	}
	
	if ((statusGPIOE & GPIO_PIN_1) && !current_PE1) { //Status do PE1 era 1, foi para 0
		current_PE1 = true;
		start_sonar_tras = TimerValueGet(TIMER0_BASE, TIMER_A);
	} 
	if (!(statusGPIOE & GPIO_PIN_1) && (current_PE1)) { // Status do PE1 era 1, foi para 0
		current_PE1 = false;
		counter_sonar_tras = TimerValueGet(TIMER0_BASE, TIMER_A) - start_sonar_tras;
	}
	
	if ((statusGPIOE & GPIO_PIN_2) && !current_PE2) { //Status do PE2 era 1, foi para 0
		current_PE2 = true;
		start_sonar_esquerda = TimerValueGet(TIMER0_BASE, TIMER_A);
	} 
	if (!(statusGPIOE & GPIO_PIN_2) && (current_PE2)) { // Status do PE2 era 1, foi para 0
		current_PE2 = false;
		counter_sonar_esquerda = TimerValueGet(TIMER0_BASE, TIMER_A) - start_sonar_esquerda;
	}
	
	if ((statusGPIOE & GPIO_PIN_3) && !current_PE3) { //Status do PE3 era 1, foi para 0
		current_PE3 = true;
		start_sonar_direita = TimerValueGet(TIMER0_BASE, TIMER_A);
	} 
	if (!(statusGPIOE & GPIO_PIN_3) && (current_PE3)) { // Status do PE3 era 1, foi para 0
		current_PE3 = false;
		counter_sonar_direita = TimerValueGet(TIMER0_BASE, TIMER_A) - start_sonar_direita;
	}
	
}

void Timer0IntHandler(void) {
	
	current_PD1 = false;
	current_PD2 = false;
	current_PD3 = false;
	current_PE1 = false;
	current_PE2 = false;
	current_PE3 = false;
	
	sonar_cima_mm = (int32_t)(round(((double)counter_sonar_cima) / 5.8));
	sonar_baixo_mm = (int32_t)(round(((double)counter_sonar_cima) / 5.8));
	sonar_frente_mm = (int32_t)(round(((double)counter_sonar_cima) / 5.8));
	sonar_tras_mm = (int32_t)(round(((double)counter_sonar_cima) / 5.8));
	sonar_esquerda_mm = (int32_t)(round(((double)counter_sonar_cima) / 5.8));
	sonar_direita_mm = (int32_t)(round(((double)counter_sonar_cima) / 5.8));
}

void configuraSonar() {
	
	TimerConfigure(TIMER0_BASE, TIMER_CFG_ONE_SHOT_UP);
	TimerLoadSet(TIMER0_BASE, TIMER_A, SysCtlClockGet() / 10);
	
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
	
	GPIOPinTypeGPIOOutput(GPIO_PORTD_BASE, GPIO_PIN_0);
	GPIOPinTypeGPIOInput(GPIO_PORTD_BASE, GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3);
	GPIOPinTypeGPIOInput(GPIO_PORTE_BASE, GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3);
	
	GPIOIntRegister(GPIO_PORTD_BASE, verificaMedidas);
	GPIOIntRegister(GPIO_PORTE_BASE, verificaMedidas);
	
	GPIOIntEnable(GPIO_PORTD_BASE, GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3);
	GPIOIntEnable(GPIO_PORTE_BASE, GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3);
	
	IntEnable(INT_TIMER1A);
   
  TimerIntEnable(TIMER1_BASE, TIMER_TIMA_TIMEOUT);
}

void iniciaLeituraSonar() {
	
	start_sonar_cima = 0;
	start_sonar_baixo = 0;
	start_sonar_frente = 0;
	start_sonar_tras = 0;
	start_sonar_esquerda = 0;
	start_sonar_direita = 0;
	
	int32_t temp = SysCtlClockGet() / 10;
	
	counter_sonar_cima = temp;
	counter_sonar_baixo = temp;
	counter_sonar_frente = temp;
	counter_sonar_tras = temp;
	counter_sonar_esquerda = temp;
	counter_sonar_direita = temp;
	
	TimerEnable(TIMER0_BASE, TIMER_A);
}
