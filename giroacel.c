#include <stdint.h>

#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/sysctl.h"
#include "driverlib/i2c.h"
#include "driverlib/interrupt.h"

#include "inc/hw_memmap.h"
#include "inc/hw_ints.h"

#include "sensorlib/mpu6050.h"
#include "sensorlib/hw_mpu6050.h"

#include "giroacel.h"

#include "serial.h"

float fAccel[3], fGyro[3];
uint_fast16_t rawAccel[3], rawGyro[3];
tI2CMInstance sI2CInst;
tMPU6050 sMPU6050;

bool g_bMPU6050Done_init;
bool g_bMPU6050Done_read;

//extern bool debug_red;
//extern bool debug_blue;
//extern bool debug_green;

void InitCallback(void *pvCallbackData, uint_fast8_t ui8Status) {
//
// See if an error occurred.
//
	if(ui8Status != I2CM_STATUS_SUCCESS) {
		//sendShort(0xEEEE, UART0_BASE);
	}
//
// Indicate that the MPU6050 transaction has completed.
//
	
	g_bMPU6050Done_init = true;
}

void ReadCallback(void *pvCallbackData, uint_fast8_t ui8Status) {
	atualizaLeituras();
	g_bMPU6050Done_read = true;
}

void I2CInterruptHandler(void) {
	I2CMIntHandler(&sI2CInst);
}

void inicializaGiro(void) {
	g_bMPU6050Done_init = false;
	
	SysCtlPeripheralEnable(SYSCTL_PERIPH_I2C3);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
	
	GPIOPinConfigure(GPIO_PD0_I2C3SCL);
	GPIOPinConfigure(GPIO_PD1_I2C3SDA);
	
	GPIOPinTypeI2CSCL(GPIO_PORTD_BASE, GPIO_PIN_0);
	GPIOPinTypeI2C(GPIO_PORTD_BASE, GPIO_PIN_1);
	
	I2CIntRegister(I2C3_BASE, I2CInterruptHandler);
	IntEnable(INT_I2C3);
	I2CMasterIntEnable(I2C3_BASE);
	
	I2CMasterInitExpClk(I2C3_BASE, SysCtlClockGet(), true);
	
	I2CMInit(&sI2CInst, I2C3_BASE, INT_I2C3, 0xff, 0xff, SysCtlClockGet());

	//sendShort(0x0000, UART0_BASE);
	
	MPU6050Init(&sMPU6050, &sI2CInst, 0x68, InitCallback, &sMPU6050);
	
	while (!g_bMPU6050Done_init) {
		SysCtlDelay((SysCtlClockGet() / 1000) / 3);
	}
	
	g_bMPU6050Done_init = false;
	
	//sendShort(0x1234, UART0_BASE);
	
	//MPU6050Write(&sMPU6050, MPU6050_O_PWR_MGMT_1, sMPU6050.pui8Data, 1, InitCallback, &sMPU6050);
	
	MPU6050Read(&sMPU6050, MPU6050_O_ACCEL_CONFIG, sMPU6050.pui8Data, 1, InitCallback, &sMPU6050);
	while (!g_bMPU6050Done_init) {
		SysCtlDelay((SysCtlClockGet() / 1000) / 3);
	}
	g_bMPU6050Done_init = false;
	
	//sendShort(0x1234, UART0_BASE);
	
	sMPU6050.pui8Data[0] = 0x00;
	
	MPU6050Write(&sMPU6050, MPU6050_O_PWR_MGMT_2 , sMPU6050.pui8Data, 1, InitCallback, &sMPU6050);
	//MPU6050Read(&sMPU6050, MPU6050_O_ACCEL_CONFIG, sMPU6050.pui8Data, 1, InitCallback, &sMPU6050);
	//MPU6050ReadModifyWrite(&sMPU6050, MPU6050_O_PWR_MGMT_1, ~MPU6050_PWR_MGMT_1_SLEEP, 0x00, InitCallback, &sMPU6050);
	
	while (!g_bMPU6050Done_init) {
		SysCtlDelay((SysCtlClockGet() / 1000) / 3);
	}
	
	
	g_bMPU6050Done_init = false;
	
	//sendShort(0xABCD, UART0_BASE);
	
	/*
	sMPU6050.pui8Data[0] = 0x40;
	while (sMPU6050.pui8Data[0] == 0x40) {
		MPU6050Read(&sMPU6050, MPU6050_O_PWR_MGMT_1, sMPU6050.pui8Data, 1, InitCallback, &sMPU6050);
	
		sendShort(0x0000, UART0_BASE);
		while (!g_bMPU6050Done_init) {
			SysCtlDelay((SysCtlClockGet() / 1000) / 3);
		}
		g_bMPU6050Done_init = false;
		sendShort(0xFFFF, UART0_BASE);

	}
	*/
	
	//sendShort(0xFFFF, UART0_BASE);
	
	
	sMPU6050.pui8Data[0] = 0;
//	uint_fast16_t teste;
	
	for (int i = MPU6050_O_SELF_TEST_X; i <= MPU6050_O_WHO_AM_I;) {
		MPU6050Read(&sMPU6050, i, sMPU6050.pui8Data, 1, InitCallback, 0);
		
		while (!g_bMPU6050Done_init) {
			SysCtlDelay((SysCtlClockGet() / 1000) / 3);
		}
		
		g_bMPU6050Done_init = false;
		
	//	teste = ((i) << 8) + sMPU6050.pui8Data[0];
		
		//sendShort(teste, UART0_BASE);
		i++;
	}
	
		
	/*
	MPU6050Read(&sMPU6050, MPU6050_O_PWR_MGMT_1, sMPU6050.pui8Data, 1, InitCallback, &sMPU6050);
	
	while (!g_bMPU6050Done_init) {
		SysCtlDelay((SysCtlClockGet() / 1000) / 3);
	}
	
	uint_fast16_t teste = 0xFF00;
	teste |= sMPU6050.pui8Data[0];
	
	//sendShort(teste, UART3_BASE);
							 
	sMPU6050.pui8Data[0] |= MPU6050_PWR_MGMT_1_CLKSEL_XG;
	
	MPU6050Write(&sMPU6050, MPU6050_O_PWR_MGMT_1, sMPU6050.pui8Data, 1, InitCallback, &sMPU6050);
	
	while (!g_bMPU6050Done_init) {
		SysCtlDelay((SysCtlClockGet() / 1000) / 3);
	}
	*/
	//sendShort(teste, UART3_BASE);
	
}

void atualizaLeituras(void) {
	//MPU6050DataAccelGetFloat(&sMPU6050, &fAccel[0], &fAccel[1], &fAccel[2]);
	//MPU6050DataGyroGetFloat(&sMPU6050, &fGyro[0], &fGyro[1], &fGyro[2]);
	MPU6050DataAccelGetRaw(&sMPU6050, &rawAccel[0], &rawAccel[1], &rawAccel[2]);
	MPU6050DataGyroGetRaw(&sMPU6050, &rawGyro[0], &rawGyro[1], &rawGyro[2]);
	
	//debug_red = !debug_red;
	
	//Colocar código para filtrar os valores
}

void iniciaLeitura(void) {
	MPU6050DataRead(&sMPU6050, ReadCallback, &sMPU6050);
	//MPU6050Read(&sMPU6050, MPU6050_O_ACCEL_XOUT_H, sMPU6050.pui8Data, 14, InitCallback, &sMPU6050);
}
