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
	
	Ethernet.begin(mac);

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

			char buffer[BUFFER_SIZE];

			char tamanhoPacote = cliente.read();
			char tipoPacote = cliente.read();

			buffer[tamanhoPacote] = END_STRING;

			for (int i = 0; i < tamanhoPacote; i++){
				buffer[i] = cliente.read();
			}

			/* decisao do pacote */

			switch (tipoPacote)
			{
			case CADASTRAR_COLABORADOR: {

				Arquivos.incluirColaborador(buffer);

				break; 
			}
			case CONSULTAR_COLABORADOR: {

				char* pessoa;
				int id = buffer[0];

				Arquivos.consultarColaborador(pessoa, id); //TODO: bitwise manolo

				cliente.print(COLABORADOR_CONSULTADO);
				cliente.print(pessoa);

				break;
			}
			case ALTERAR_COLABORADOR: {

				Arquivos.alterarColaborador(buffer);

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