#ifndef __SERIAL_H__
#define __SERIAL_H__

#include <stdint.h>

void ConfigureUART(void);
void ConfigureXBeeUART(void);
void ConfigureUARTSensores(void);
void checkUART(uint32_t UART_checked);
void echoUART(uint32_t UART_sender, uint32_t UART_receiver);

//void sendShort(uint_fast16_t int16, uint32_t UART_sender);

void enviarDiagnostico(void);

void enviarDadosSonares(void);
void enviarDadosMPU6050(void);

void readPackage(void);
void getCommand(void);
void readType(void);

void check(void);

void requestMPUData(void);
void requestSonarData(int sonar);

#endif //__SERIAL_H__
