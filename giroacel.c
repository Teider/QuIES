#include <stdint.h>

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

double fAccel[2], fGyro[3];
int_fast16_t rawAccel[3], rawGyro[3];
int_fast16_t offsetAccel[3], offsetGyro[3];

extern char sensorData[12];
extern bool sensorDataDone;

void inicializaGiro() {
	
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
	
	for (int i = 0; i < 3; i++) {
		offsetAccelTemp[i] /= 3000;
		offsetGyroTemp[i] /= 3000;
		
		offsetAccel[i] = (int_fast16_t)offsetAccelTemp[i];
		offsetGyro[i] = (int_fast16_t)offsetGyroTemp[i];
	}
	
}

void iniciaLeitura() {
	requestMPUData();
}

void atualizaLeituras() {
	rawAccel[0] += (((int_fast16_t)sensorData[0]) << 8) + (((int_fast16_t)sensorData[1]) & 0xFF);
	rawAccel[1] += (((int_fast16_t)sensorData[2]) << 8) + (((int_fast16_t)sensorData[3]) & 0xFF);
	rawAccel[2] += (((int_fast16_t)sensorData[4]) << 8) + (((int_fast16_t)sensorData[5]) & 0xFF);
	rawGyro[0]  += (((int_fast16_t)sensorData[6]) << 8) + (((int_fast16_t)sensorData[7]) & 0xFF);
	rawGyro[1]  += (((int_fast16_t)sensorData[8]) << 8) + (((int_fast16_t)sensorData[9]) & 0xFF);
	rawGyro[2]  += (((int_fast16_t)sensorData[10]) << 8) + (((int_fast16_t)sensorData[11]) & 0xFF);
	
	for (int i = 0; i < 3; i++) {
		rawAccel[i] -= offsetAccel[i];
		rawGyro[i] -= offsetGyro[i];
		
		fGyro[i] = (((double)rawGyro[i]) / 131.0);
	}
	
	double ax, ay, az;
	ax = ((rawAccel[0] / 16384.0) * 9.81);
	ay = ((rawAccel[1] / 16384.0) * 9.81);
	az = ((rawAccel[2] / 16384.0) * 9.81);
	
	fAccel[0] = atan2(ax , sqrt((pow(ay,2.0))+(pow(az,2.0))));
	fAccel[1] = atan2(ay , sqrt((pow(ax,2.0))+(pow(az,2.0))));
	
	for (int i = 0; i < 2; i++) {
		fAccel[i] = (fAccel[i] * (PI / 180.0));
	}
}
