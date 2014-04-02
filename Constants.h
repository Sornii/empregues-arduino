// Constants.h

#ifndef _CONSTANTSCLASS_h
#define _CONSTANTSCLASS_h

#define FIM_STRING				'\0'
#define SEPARADOR				';'
#define SEPARADOR_GERAL			'$'

/* Ethernet Shield. */
#define SERVER_PORTA			55535
#define SERVER_WEBPORTA			80

/* Pacotes. */

#define BUFFER_SIZE				256
#define PACOTE_NULO				0x00

// Colaborador
#define CADASTRAR_COLABORADOR	0x01
#define CONSULTAR_COLABORADOR	0x02
#define ALTERAR_COLABORADOR		0x03
#define	EXCLUIR_COLABORADOR		0x04

#define LISTAR_COLABORADORES	0x05

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

