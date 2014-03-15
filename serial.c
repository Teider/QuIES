#include "inc/hw_memmap.h"

#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/sysctl.h"
#include "driverlib/uart.h"
#include "giroacel.h"
#include "serial.h"
#include "stdlib.h"
#include "String.h"
#include "message_types.h"
#include "motion.h"
#include "diagnostic_msg.h"



//extern bool debug_red;
//extern bool debug_blue;
//extern bool debug_green;

extern bool motoresInicializados;

char package[30];
int pacoteToInt[30];

char id;
char tipo;
char velocidade;

bool led = false;

int packageSize = 4;
int packageCounter = 0;

bool pacotePronto = false;

char recebido = 'r';

char sensorData[12];
int sensorDataCounter = 0;
bool sensorDataDone = false;

extern uint32_t sonar_cima_mm;
extern uint32_t sonar_baixo_mm;
extern uint32_t sonar_frente_mm;
extern uint32_t sonar_tras_mm;
extern uint32_t sonar_esquerda_mm;
extern uint32_t sonar_direita_mm;


//*****************************************************************************
//
// Configure the UART and its pins.  This must be called before UARTprintf().
//
//*****************************************************************************
void ConfigureUART(void) {
    //
    // Enable the GPIO Peripheral used by the UART.
    //
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);

    //
    // Enable UART0
    //
    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);

    //
    // Configure GPIO Pins for UART mode.
    //
    GPIOPinConfigure(GPIO_PA0_U0RX);
    GPIOPinConfigure(GPIO_PA1_U0TX);
    GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);

    //
    // Use the internal 16MHz oscillator as the UART clock source.
    //
    UARTClockSourceSet(UART0_BASE, UART_CLOCK_PIOSC);
		UARTConfigSetExpClk(UART0_BASE, 16000000, 9600,
                        (UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE |
                         UART_CONFIG_PAR_NONE));

		UARTEnable(UART0_BASE);
}

void ConfigureXBeeUART(void)
{
    //
    // Enable the GPIO Peripheral used by the UART.
    //
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);

    //
    // Enable UART2
    //
    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART3);

    //
    // Configure GPIO Pins for UART mode.
    //
    GPIOPinConfigure(GPIO_PC6_U3RX);
    GPIOPinConfigure(GPIO_PC7_U3TX);
    GPIOPinTypeUART(GPIO_PORTC_BASE, GPIO_PIN_6 | GPIO_PIN_7);

    //
    // Use the internal 16MHz oscillator as the UART clock source.
    //
    UARTClockSourceSet(UART3_BASE, UART_CLOCK_PIOSC);
		UARTConfigSetExpClk(UART3_BASE, 16000000, 9600,
                        (UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE |
                         UART_CONFIG_PAR_NONE));
		
		UARTEnable(UART3_BASE);
}

void ConfigureUARTSensores() {
	
		//
    // Enable the GPIO Peripheral used by the UART.
    //
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);

    //
    // Enable UART2
    //
    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART4);

    //
    // Configure GPIO Pins for UART mode.
    //
    GPIOPinConfigure(GPIO_PC4_U4RX);
    GPIOPinConfigure(GPIO_PC5_U4TX);
    GPIOPinTypeUART(GPIO_PORTC_BASE, GPIO_PIN_4 | GPIO_PIN_5);

    //
    // Use the internal 16MHz oscillator as the UART clock source.
    //
    UARTClockSourceSet(UART4_BASE, UART_CLOCK_PIOSC);
		UARTConfigSetExpClk(UART4_BASE, 16000000, 9600,
                        (UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE |
                         UART_CONFIG_PAR_NONE));
		
		UARTEnable(UART4_BASE);
}


void getCommand(){
	
	packageCounter++;
	
	if(packageCounter == 4){
		readType();
	}
	
	if(packageCounter == packageSize){
		
		packageCounter = 0;
		packageSize = 4;
		
		getMotion(package);
	}
}

void check(){
	
	UARTCharPutNonBlocking(UART3_BASE, recebido);
}

void readPackage(){

	while(UARTCharsAvail(UART3_BASE)){
		
		package[packageCounter] = (char)UARTCharGet(UART3_BASE);
		
		getCommand();
	}
	
	while(UARTCharsAvail(UART4_BASE)) {
		sensorData[sensorDataCounter++] = (char)UARTCharGet(UART4_BASE);
		
		if (sensorDataCounter == 12) {
			sensorDataDone = true;
			sensorDataCounter = 0;
			
			/*
			while (sensorDataDone) {
				SysCtlDelay(SysCtlClockGet() / 1000000);
			}
			*/
		}
	}
}

void enviaID() {
	
	UARTCharPutNonBlocking(UART3_BASE, 0x00);
	UARTCharPutNonBlocking(UART3_BASE, 0x00);
	UARTCharPutNonBlocking(UART3_BASE, 0x00);
	
}

void enviarDiagnostico(){
	
	enviaID();
	
	UARTCharPutNonBlocking(UART3_BASE, MESSAGE_TYPE_DIAGNOSTICO);
	
	char c = 0x00;
	
	if(motoresInicializados == true){
		
		c |= DIAGNOSTIC_READY;
	}
	
	UARTCharPutNonBlocking(UART3_BASE, c);
}

void enviarDadosSonares() {
	
	enviaID();
	UARTCharPutNonBlocking(UART3_BASE, MESSAGE_TYPE_DADOS_SONAR);
	
	UARTCharPutNonBlocking(UART3_BASE, ((sonar_cima_mm & 0xFF00) >> 8));
	UARTCharPutNonBlocking(UART3_BASE, (sonar_cima_mm & 0xFF));
	UARTCharPutNonBlocking(UART3_BASE, ((sonar_baixo_mm & 0xFF00) >> 8));
	UARTCharPutNonBlocking(UART3_BASE, (sonar_baixo_mm & 0xFF));
	UARTCharPutNonBlocking(UART3_BASE, ((sonar_frente_mm & 0xFF00) >> 8));
	UARTCharPutNonBlocking(UART3_BASE, (sonar_frente_mm & 0xFF));
	UARTCharPutNonBlocking(UART3_BASE, ((sonar_tras_mm & 0xFF00) >> 8));
	UARTCharPutNonBlocking(UART3_BASE, (sonar_tras_mm & 0xFF));
	UARTCharPutNonBlocking(UART3_BASE, ((sonar_esquerda_mm & 0xFF00) >> 8));
	UARTCharPutNonBlocking(UART3_BASE, (sonar_esquerda_mm & 0xFF));
	UARTCharPutNonBlocking(UART3_BASE, ((sonar_direita_mm & 0xFF00) >> 8));
	UARTCharPutNonBlocking(UART3_BASE, (sonar_direita_mm & 0xFF));
}

void readType() {
	
	switch (package[3]) {
		
		case MESSAGE_TYPE_PARAR:
	  case MESSAGE_TYPE_DECOLAR:		
    case MESSAGE_TYPE_POUSAR:
			packageSize = 4;
			break;
		case MESSAGE_TYPE_ESQUERDA:
	  case MESSAGE_TYPE_DIREITA:		
    case MESSAGE_TYPE_FRENTE:	
    case MESSAGE_TYPE_TRAS:			
    case MESSAGE_TYPE_GIR_HORARIO:
		case MESSAGE_TYPE_GIR_ANTI:
	  case MESSAGE_TYPE_SUBIR:	
		case MESSAGE_TYPE_DESCER:
			packageSize = 5;
			break;
		case MESSAGE_TYPE_HANDSHAKE:
			packageSize = 4;
			enviarDiagnostico();
			break;
		
	}
	
}

void requestMPUData() {
	UARTCharPutNonBlocking(UART4_BASE, MESSAGE_TYPE_PEDE_DADOS);
	sensorDataDone = false;
}
