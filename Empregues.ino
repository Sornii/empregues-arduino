#include "Pacote.h"
#include "Arquivos.h"
#include "Constants.h"
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <Adafruit_NFCShield_I2C.h>
#include <RTClib.h>
#include <util.h>
#include <EthernetUdp.h>
#include <EthernetServer.h>
#include <EthernetClient.h>
#include <Ethernet.h>
#include <Dns.h>
#include <Dhcp.h>

EthernetServer server(SERVER_PORTA);
EthernetUDP Udp;

byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };

void setup()
{
	pinMode(53, OUTPUT);
	
	Ethernet.begin(mac, IPAddress(10,1,1,14));

	SD.begin(4);

	Arquivos.init();

	Serial.println(Ethernet.localIP());
}

void loop()
{
	/*
	int packetSize = Udp.parsePacket();
	if(packetSize)
	{
		IPAddress remote = Udp.remoteIP();
		char packetBuffer[UDP_TX_PACKET_MAX_SIZE];
		Udp.read(packetBuffer, UDP_TX_PACKET_MAX_SIZE);

		Udp.beginPacket(Udp.remoteIP(), Udp.remotePort());

		for (int i = 0; i < 4; i++)
			Udp.write(Ethernet.localIP()[i]);

		Udp.endPacket();
	}
	*/

	EthernetClient cliente = server.available();
	if (cliente)
	{
		if (cliente.available())
		{
			/* leitura do pacote */

			Pacote pacote;

			pacote.receber(&cliente);

			switch (pacote.tipo)
			{
			case CADASTRAR_COLABORADOR: {

				/*
				
				Cadastrado do colaborador 
				
				A informação deve vir obrigatoriamente da seguinte forma

				PIS     |  NOME
				38231283;Fulano da Silva
				
				*/

				uint32_t idCadastrado = Arquivos.incluirColaborador((char*) pacote.buffer);

				/*
				
				Envia a resposta com o id cadastrado
				
				O valor será enviado da seguinte forma

				ID
				901823792

				*/

				ltoa(idCadastrado, (char*) pacote.buffer, 10);

				pacote.tamanho = strlen((char*) pacote.buffer);
				pacote.tipo = COLABORADOR_CADASTRADO;
				
				pacote.enviar(&cliente);

				break; 
			}
			case CONSULTAR_COLABORADOR: {


				
				break;
			}
			case LISTAR_COLABORADORES: {

				for (uint32_t i = 1; i <= Arquivos.Id(); i++)
				{
					bool achou = Arquivos.consultarColaborador((char*) pacote.buffer, i);

					if (achou)
					{
						pacote.tamanho = strlen((char*) pacote.buffer);

						char buffer[12];

						ltoa(i, buffer, 10);
						strcat(buffer, ";");

						pacote.insertBefore(buffer);

						pacote.tipo = COLABORADOR_LISTADO;

						pacote.enviar(&cliente);
					}
				}

				pacote.tipo = PACOTE_NULO;
				pacote.tamanho = 1;

				pacote.enviar(&cliente);
			}
			case ALTERAR_COLABORADOR: {

				

				break;
			}
			case EXCLUIR_COLABORADOR: {



				break;
			}
			default:
				break;
			}

			delay(10);
		}
	}
}