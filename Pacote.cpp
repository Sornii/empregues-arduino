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

void Pacote::insertBefore(char* toInsert)
{
	int strInsertLength = strlen(toInsert);
	int strReceiveLength = strlen((char*) buffer);

	for (int i = strReceiveLength; i >= 0; i--)
	{
		buffer[i + strInsertLength] = buffer[i];
	}

	for (int i = 0; i < strInsertLength; i++)
	{
		buffer[i] = toInsert[i];
	}
}