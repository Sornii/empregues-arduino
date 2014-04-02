#include "Pacote.h"

void Pacote::receber(EthernetClient* cliente)
{
	tamanho = cliente->read();
	tipo = cliente->read();

	cliente->read(buffer, tamanho);

	buffer[tamanho] = 0;
}

void Pacote::enviar(EthernetClient* cliente)
{
	for (uint8_t i = tamanho; i > 0; i--)
		buffer[i + 1] = buffer[i - 1];

	buffer[0] = tamanho;
	buffer[1] = tipo;

	cliente->write(buffer, tamanho + 2);
}