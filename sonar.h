#ifndef __SONAR_H__
#define __SONAR_H__

#define SONAR_FRENTE 0x01
#define SONAR_TRAS 0x02
#define SONAR_ESQUERDA 0x03
#define SONAR_DIREITA 0x04
#define SONAR_CIMA 0x05
#define SONAR_BAIXO 0x06

void iniciaLeituraSonar(int sonar);
void atualizaLeituraSonar(int sonar);

#endif //__SONAR_H__
