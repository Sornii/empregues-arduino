#ifndef _PACOTE_h
#define _PACOTE_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include <Ethernet.h>
#include "Constants.h"

class Pacote
{
private:


public:
	void receber(EthernetClient*);
	void enviar(EthernetClient*);
	void enviarNulo(EthernetClient*);

	void insertBefore(char*);

	byte tipo;
	byte tamanho;
	byte buffer[256];
};

#endif