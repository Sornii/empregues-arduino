// 
// 
// 

#include "Pacote.h"

void Pacote::receber(EthernetClient* cliente)
{
	tamanhoPacote = cliente->read();
	tipoPacote = cliente->read();

	buffer[tamanhoPacote] = FIM_STRING;

	for (int i = 0; i < tamanhoPacote; i++)
		buffer[i] = cliente->read();
}

void Pacote::enviar(EthernetClient* cliente)
{
	cliente->write(tamanhoPacote);
	cliente->write(tipoPacote);

	int tam = strlen(buffer);

	for (int i = 0; i < tam; i++)
		cliente->write(buffer[i]);
}

Pacote::Pacote(uint8_t tipoPacote, char* mensagem)
{
}