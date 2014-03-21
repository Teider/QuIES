#ifndef __MESSAGE_TYPES_H__
#define __MESSAGE_TYPES_H__

#define MESSAGE_TYPE_ESQUERDA 					0x00
#define MESSAGE_TYPE_DIREITA 						0x01
#define MESSAGE_TYPE_FRENTE 						0x02
#define MESSAGE_TYPE_TRAS 							0x03
#define MESSAGE_TYPE_GIR_HORARIO 				0x04
#define MESSAGE_TYPE_GIR_ANTI 					0x05
#define MESSAGE_TYPE_SUBIR 							0x06
#define MESSAGE_TYPE_DESCER 						0x07
#define MESSAGE_TYPE_PARAR 							0x08
#define MESSAGE_TYPE_DECOLAR		 				0x09
#define MESSAGE_TYPE_POUSAR 						0x0A
#define MESSAGE_TYPE_ERRO_DESCONHECIDO 	0x0B
#define MESSAGE_TYPE_ID_MSG_ERRADA 			0x0C
#define MESSAGE_TYPE_MSG_CORROMP 				0x0D
#define MESSAGE_TYPE_MSG_INESPERADA 		0x0E
#define MESSAGE_TYPE_VALOR_INVALIDO 		0x0F
#define MESSAGE_TYPE_HANDSHAKE 					0x10
#define MESSAGE_TYPE_DESCONECTAR 				0x11
#define MESSAGE_TYPE_KEEP_ALIVE 				0x12
#define MESSAGE_TYPE_DIAGNOSTICO 				0x13
#define MESSAGE_TYPE_DADOS_SENSORES 		0x14
#define MESSAGE_TYPE_COLISAO_DETECT 		0x15
#define MESSAGE_TYPE_NO_AR 							0x16
#define MESSAGE_TYPE_NO_CHAO 						0x17
#define MESSAGE_TYPE_PEDE_MPU6050				0x18
#define MESSAGE_TYPE_DADOS_MPU6050			0x19
#define MESSAGE_TYPE_DADOS_SONAR				0x1A
#define MESSAGE_TYPE_PEDE_SONAR					0x1B


#define MESSAGE_TYPE_DEBUG_VELOCIDADE_MOTOR	0xD0


#endif //__MESSAGE_TYPES_H__
