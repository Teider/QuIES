#ifndef __MOTORS_H__
#define __MOTORS_H__

void inicializa_motores(void);

void accelerate(int id_motor);
void decelerate(int id_motor);

void changeSpeed(int id_motor, int nova_velocidade, int compensacao);

void adjustCompensacao(int id_motor, int vel);

void update_ppm(void);

void panic(void);

#endif //__MOTORS_H__
