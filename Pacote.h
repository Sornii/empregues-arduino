// Pacote.h

#ifndef _PACOTE_h
#define _PACOTE_h

/*
#if defined(ARDUINO) && ARDUINO >= 100
	#include "Arduino.h"
#else
	#include "WProgram.h"
#endif
*/

#include <EthernetClient.h>
#include "Constants.h"

class Pacote
{
 private:
	 EthernetClient* _cliente;
 public:
	 Pacote();
	 Pacote(uint8_t tipoPacote, char* mensagem);
	 
	 void receber(EthernetClient* cliente);
	 void enviar(EthernetClient* cliente);

	 uint8_t tamanhoPacote;
	 uint8_t tipoPacote;

	 char buffer[BUFFER_SIZE];
};

#endif

