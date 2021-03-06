//*****************************************************************************
//
// timers.c - Timers example.
//
// Copyright (c) 2012-2013 Texas Instruments Incorporated.  All rights reserved.
// Software License Agreement
// 
// Texas Instruments (TI) is supplying this software for use solely and
// exclusively on TI's microcontroller products. The software is owned by
// TI and/or its suppliers, and is protected under applicable copyright
// laws. You may not combine this software with "viral" open-source
// software in order to form a larger program.
// 
// THIS SOFTWARE IS PROVIDED "AS IS" AND WITH ALL FAULTS.
// NO WARRANTIES, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT
// NOT LIMITED TO, IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE. TI SHALL NOT, UNDER ANY
// CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL, OR CONSEQUENTIAL
// DAMAGES, FOR ANY REASON WHATSOEVER.
// 
// This is part of revision 2.0.1.11577 of the EK-TM4C123GXL Firmware Package.
//
//*****************************************************************************

#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_ints.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
//#include "driverlib/debug.h"
#include "driverlib/fpu.h"
#include "driverlib/gpio.h"
#include "driverlib/interrupt.h"
#include "driverlib/pin_map.h"
//#include "driverlib/rom.h"
#include "driverlib/sysctl.h"
#include "driverlib/timer.h"
#include "driverlib/uart.h"
//#include "utils/uartstdio.h"

#include "motors.h"
#include "giroacel.h"
#include "serial.h"
#include "sonar.h"
#include "control.h"

#include "drivers/buttons.h"

//extern bool debug_red;
//extern bool debug_blue;
//extern bool debug_green;

extern bool g_bMPU6050Done_read;

extern float fAccel[3], fGyro[3];
extern uint_fast16_t rawAccel[3], rawGyro[3];

extern bool generate_pulse_flag;
bool pulse_flag;

uint_fast16_t counter_sonar;

extern bool motoresInicializados;

bool init_control = false;

bool no_ar = false, no_chao = false;


//*****************************************************************************
//
//! \addtogroup example_list
//! <h1>Timer (timers)</h1>
//!
//! This example application demonstrates the use of the timers to generate
//! periodic interrupts.  One timer is set up to interrupt once per second and
//! the other to interrupt twice per second; each interrupt handler will toggle
//! its own indicator on the display.
//!
//! UART0, connected to the Virtual Serial Port and running at 115,200, 8-N-1,
//! is used to display messages from this application.
//
//*****************************************************************************

//*****************************************************************************
//
// Flags that contain the current value of the interrupt indicator as displayed
// on the UART.
//
//*****************************************************************************
//uint32_t g_ui32Flags;

//*****************************************************************************
//
// The error routine that is called if the driver library encounter_ppms an error.
//
//*****************************************************************************
#ifdef DEBUG
void
__error__(char *pcFilename, uint32_t ui32Line)
{
}
#endif

//*****************************************************************************
//
// The interrupt handler for the second timer interrupt.
//
//*****************************************************************************

//int counter_leitura_mpu6050 = 0;

void Timer0IntHandler(void) {
}

int counter_measure_mpu6050 = 0;
int counter_measure_sonar = 0;

void Timer1IntHandler(void)
{
	if (motoresInicializados && init_control) {
		if (counter_measure_mpu6050 == 10000) {
			counter_measure_mpu6050 = 0;
			iniciaLeituraMPU6050();
			//atualizaControle();
		}
		counter_measure_mpu6050++;
	}
	
	if (motoresInicializados && init_control) {
		if (counter_measure_sonar == 100000) {
			counter_measure_sonar = 0;
			iniciaLeituraSonar(SONAR_BAIXO);
		}
		counter_measure_sonar++;
	}	
	
	update_ppm();
}

int sonar_select = SONAR_BAIXO;

void checkButtons(void) {
	uint8_t button_state = 0;
	uint8_t button_changed = 0;
	button_state = ButtonsPoll(&button_changed,0);
	switch (button_changed & ALL_BUTTONS) {
		case LEFT_BUTTON:
			if (button_state & LEFT_BUTTON) {
				iniciaLeituraSonar(sonar_select);
			}
		break;
		case RIGHT_BUTTON:
			if (button_state & RIGHT_BUTTON) {
				iniciaLeituraMPU6050();
			}
		break;
	}
}

//*****************************************************************************
//
// This example application demonstrates the use of the timers to generate
// periodic interrupts.
//
//*****************************************************************************
int
main(void)
{
    //
    // Enable lazy stacking for interrupt handlers.  This allows floating-point
    // instructions to be used within interrupt handlers, but at the expense of
    // extra stack usage.
    //
		FPUEnable();
		FPULazyStackingEnable();

    //
    // Set the clocking to run directly from the crystal.
    //
    SysCtlClockSet(SYSCTL_SYSDIV_4 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN |
                       SYSCTL_XTAL_16MHZ);
	
		//
    // Enable processor interrupts.
    //
    IntMasterEnable();

    //
    // Initialize the UART and write status.
    //
    ConfigureUART();
		ConfigureXBeeUART();
		ConfigureUARTSensores();
		ButtonsInit();
		inicializa_motores();

    //
    // Enable the peripherals used by this example.
    //
    SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER1);

    //
    // Configure the two 32-bit periodic timers.
    //
    TimerConfigure(TIMER1_BASE, TIMER_CFG_PERIODIC);
    TimerLoadSet(TIMER1_BASE, TIMER_A, SysCtlClockGet()/200000);
		
		
    //
    // Setup the interrupts for the timer timeouts.
    //
    IntEnable(INT_TIMER1A);
   
    TimerIntEnable(TIMER1_BASE, TIMER_TIMA_TIMEOUT);

    //
    // Enable the timers.
    //
		
    TimerEnable(TIMER1_BASE, TIMER_A);


		// Configure a wide timer for timing purposes
		
		SysCtlPeripheralEnable(SYSCTL_PERIPH_WTIMER0);
		TimerConfigure(WTIMER0_BASE, TIMER_CFG_PERIODIC_UP);
		TimerLoadSet64(WTIMER0_BASE, (((long long)1) << 60));
		TimerEnable(WTIMER0_BASE, TIMER_A);
	
	int counter_verify_no_ar = 0;
	
	while(1) {
		SysCtlDelay(SysCtlClockGet() / 1000);
		checkButtons();
		
		readPackage();
		
		counter_verify_no_ar++;
		
		if (counter_verify_no_ar == 1000) {
			counter_verify_no_ar = 0;
			if (no_ar) {
				enviaNoAr();
				no_ar = false;
			}
			if (no_chao) {
				enviaNoChao();
				no_chao = false;
			}
		}
	}
}
