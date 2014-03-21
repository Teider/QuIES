#include <stdint.h>

#include "driverlib/fpu.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/sysctl.h"
#include "driverlib/interrupt.h"

#include "inc/hw_memmap.h"
#include "inc/hw_ints.h"

#include "giroacel.h"

#include "serial.h"

#include "math.h"

#define PI 3.14159265


double roll=0.0, pitch=0.0, yaw=0.0;

extern char sensorData[6];

void inicializaGiro() {
	/*
	int32_t offsetAccelTemp[3], offsetGyroTemp[3];
	
	for (int i = 0; i < 3000; i++) {
		requestMPUData();
		
		while (!sensorDataDone) {
			SysCtlDelay(SysCtlClockGet() / 1000000);
		}
		
		offsetAccelTemp[0] += (((int32_t)sensorData[0]) << 8) + (((int32_t)sensorData[1]) & 0xFF);
		offsetAccelTemp[1] += (((int32_t)sensorData[2]) << 8) + (((int32_t)sensorData[3]) & 0xFF);
		offsetAccelTemp[2] += (((int32_t)sensorData[4]) << 8) + (((int32_t)sensorData[5]) & 0xFF);
		offsetGyroTemp[0]  += (((int32_t)sensorData[6]) << 8) + (((int32_t)sensorData[7]) & 0xFF);
		offsetGyroTemp[1]  += (((int32_t)sensorData[8]) << 8) + (((int32_t)sensorData[9]) & 0xFF);
		offsetGyroTemp[2]  += (((int32_t)sensorData[10]) << 8) + (((int32_t)sensorData[11]) & 0xFF);
		
		SysCtlDelay(SysCtlClockGet() / 100);
	}
	
*/
	
	
}

void iniciaLeituraMPU6050() {
	requestMPUData();
}

void atualizaLeiturasMPU6050() {
	
	roll = (((int)sensorData[0]) << 8) + (((int)sensorData[1]) & 0xFF);
	pitch = (((int)sensorData[2]) << 8) + (((int)sensorData[3]) & 0xFF);
	yaw = (((int)sensorData[4]) << 8) + (((int)sensorData[5]) & 0xFF);
	

	enviarDadosMPU6050();
}
