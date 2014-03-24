#include <stdint.h>

#include "driverlib/fpu.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/sysctl.h"
#include "driverlib/interrupt.h"

#include "inc/hw_memmap.h"
#include "inc/hw_ints.h"

#include "giroacel.h"
#include "control.h"

#include "serial.h"

float roll=180.0, pitch=180.0, yaw=180.0;

extern char sensorData[6];

void iniciaLeituraMPU6050() {
	requestMPUData();
}

void atualizaLeiturasMPU6050() {
	
	roll = (((int)sensorData[0]) << 8) + (((int)sensorData[1]) & 0xFF);
	pitch = (((int)sensorData[2]) << 8) + (((int)sensorData[3]) & 0xFF);
	yaw = (((int)sensorData[4]) << 8) + (((int)sensorData[5]) & 0xFF);
	
	roll /= ((float)60.0);
	pitch /= ((float)60.0);
	yaw /= ((float)60.0);

	enviarDadosMPU6050();
	
	atualizaControle();
}
