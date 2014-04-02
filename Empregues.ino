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
}

void loop()
{
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

	EthernetClient cliente = server.available();
	if (cliente)
	{
		if (cliente.available())
		{
			/* leitura do pacote */

			Pacote pacote;

			pacote.receber(&cliente);

			switch (pacote.tipoPacote)
			{
			case CADASTRAR_COLABORADOR: {

				uint8_t serl[4];
				uint8_t* p = serl;

				long id = Arquivos.incluirColaborador(pacote.buffer);

				for (int i = 0; i < 4; i++)
					*(p + i) = (uint8_t) (id >> 8 * i);

				pacote.tipoPacote = COLABORADOR_CADASTRADO;
				pacote.tamanhoPacote = 4;

				for (int i = 0; i < 4; i++)
					pacote.buffer[i] = serl[i];

				pacote.enviar(&cliente);

				break; 
			}
			case CONSULTAR_COLABORADOR: {
				
				break;
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