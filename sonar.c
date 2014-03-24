#include <stdint.h>

#include "sonar.h"

#include "driverlib/interrupt.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/sysctl.h"
#include "driverlib/timer.h"

#include "inc/hw_ints.h"
#include "inc/hw_memmap.h"

#include "serial.h"

extern char sonarData[3];
//extern bool sonarReadingDone;

uint32_t ultimaLeitura[6];

void iniciaLeituraSonar(int sonar) {
	
	requestSonarData(sonar);
}

void atualizaLeituraSonar(int sonar) {
	
	ultimaLeitura[sonar] = (((uint32_t)sonarData[1]) << 8) + ((uint32_t) sonarData[2]);
	
	enviarDadosSonares();
}
