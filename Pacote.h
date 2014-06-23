#ifndef _PACOTE_h
#define _PACOTE_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

// Necessário para EthernetClient
#include <Ethernet.h>

// Necessário para BUFFER_SIZE
#include "Constants.h"

class Pacote
{
private:


public:
	// Recebe um pacote conforme as regras do
	// protocolo
	//
	// \cliente Cliente para receber o pacote
	void receber(EthernetClient*);

	// Envia o pacote conforme a atribuição
	// dos valores do pacote em questão
	//
	// \cliente Cliente para enviar o pacote
	void enviar(EthernetClient*);

	// Envia o pacote como um aviso
	//
	// \cliente Cliente para enviar o pacote
	// \byte Byte de aviso para ser enviado
	void enviar(EthernetClient*, byte);

	// Envia um pacote nulo como sinalização
	// de envio de vários pacotes
	//
	// \cliente Cliente para enviar o pacote nulo
	void enviarNulo(EthernetClient*);

	// Insere a cadeia de caracteres no começo
	// do buffer
	//
	// \toInsert Cadeia de caracteres para ser
	// inserida no buffer
	void insertBefore(char*);

	// Guarda o tipo do pacote
	byte tipo;

	// Define o tamanho do buffer a ser
	// enviado/recebido
	byte tamanho;

	// Guarda o buffer para ser enviado/recebido
	byte buffer[BUFFER_SIZE];
};

#endif