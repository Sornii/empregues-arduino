// Constants.h

#ifndef _CONSTANTSCLASS_h
#define _CONSTANTSCLASS_h

#define DEBUG Serial.println

#define SEM_APITO_POR_HOJE
#define BUZZER_PIN				30
#define DELAY_PAUSA				800
#define DELAY_APITO				50

/* SPI BUS */

#define SCK 52
#define MISO 50
#define MOSI 51

/* I2C BUS */

#define RFID_ADDRESS			0x24
#define RTC_ADDRESS				0x68
#define LCD_ADDRESS				0x27

/* LCD */

#define BACKLIGHT_PIN			3

/* NFC MODULE */

#define NFC_IRQ					(2)
#define NFC_RESET				(3)

/* Comunicacao */

#define FIM_STRING				'\0'
#define SEPARADOR				';'

/* Ethernet Shield. */
#define SERVER_PORTA			55535
#define SERVER_WEBPORTA			80
#define FIX_OUTPUT				53

/* Pacotes. */

#define BUFFER_SIZE				256
#define PACOTE_NULO				0x00

// Colaborador
#define CADASTRAR_COLABORADOR	0x01
#define CONSULTAR_COLABORADOR	0x02
#define ALTERAR_COLABORADOR		0x03
#define	EXCLUIR_COLABORADOR		0x04

#define LISTAR_COLABORADORES	0x05
#define DEFINIR_CARTAO_COLABO	0x06
#define LISTAR_CARTOES			0x07
#define CARTAO_CONSULTADO		0x08
#define CARTAO_DEFINIDO			0x09
#define LISTAR_PONTOS			0x10
#define PONTO_LISTADO			0x11

#define COLABORADOR_CONSULTADO	0xF0
#define COLABORADOR_CADASTRADO	0xEF
#define COLABORADOR_LISTADO		0xEE

// Empregador
#define CADASTRAR_EMPREGADOR	0x15
#define CONSULTAR_EMPREGADOR	0x16
#define ALTERAR_EMPREGADOR		0x17

// Relógio
#define ALTERAR_RELOGIO			0xA8

// Ponto
#define CON_PONTO_COLABORADOR	0xA1

// Historico
#define HIST_RELOGIO			0x22
#define HIST_EMPREGADOR			0x23
#define HIST_COLABORADOR		0x24

#endif

