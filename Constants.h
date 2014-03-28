// Constants.h

#ifndef _CONSTANTSCLASS_h
#define _CONSTANTSCLASS_h

#define END_STRING				'\0'
#define SEPARADOR				';'
#define SEPARADOR_GERAL			'$'

/* Ethernet Shield. */
#define SERVER_PORTA			55535
#define SERVER_WEBPORTA			80

/* Pacotes. */

#define BUFFER_SIZE				256

// Colaborador
#define CADASTRAR_COLABORADOR	0x01
#define CONSULTAR_COLABORADOR	0x02
#define ALTERAR_COLABORADOR		0x03
#define	EXCLUIR_COLABORADOR		0x04

#define COLABORADOR_CONSULTADO	0xF0

// Empregador
#define CADASTRAR_EMPREGADOR	0x05
#define CONSULTAR_EMPREGADOR	0x06
#define ALTERAR_EMPREGADOR		0x07

// Rel�gio
#define ALTERAR_RELOGIO			0x08

// Ponto
#define CON_PONTO_COLABORADOR	0x11

// Historico
#define HIST_RELOGIO			0x12
#define HIST_EMPREGADOR			0x13
#define HIST_COLABORADOR		0x14

#endif

