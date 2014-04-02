#ifndef _PACOTE_h
#define _PACOTE_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include <Ethernet.h>

class Pacote
{
private:


public:
	void receber(EthernetClient*);
	void enviar(EthernetClient*);

	byte tipo;
	byte tamanho;
	byte buffer[256];
};

#endif