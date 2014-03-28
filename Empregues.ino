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
#include <aJSON.h>

EthernetServer server(SERVER_PORTA);
EthernetUDP Udp;

byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };

void setup()
{
	pinMode(53, OUTPUT);

	Serial.begin(9600);

	printResult();

	Ethernet.begin(mac);
	//SD.begin(4);

	//Arquivos.init();

	printResult();
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

	EthernetClient client = server.available();
	if (client)
	{
		if (client.available())
		{
			int tamanhoPacote = client.available();

			Serial.print("Disponivel: ");
			Serial.println(tamanhoPacote);

			char* buffer = new char[tamanhoPacote];
			buffer[tamanhoPacote] = '\0';

			client.readBytes(buffer, tamanhoPacote);

			aJsonObject* pacote = aJson.parse(buffer);

			aJsonObject* tipoPacote = aJson.getObjectItem(pacote, "Tipo");
			aJsonObject* mensagem = aJson.getObjectItem(pacote, "Mensagem");

			switch (tipoPacote->valueint)
			{
			case CADASTRAR_COLABORADOR:

				Arquivos.incluirColaborador(mensagem);

				break;
			case CADASTRAR_EMPREGADOR:



				break;
			case CONSULTAR_COLABORADOR:

				if (strncmp(mensagem->valuestring, "0", 1))
				{
					
				}
				else if (strncmp(mensagem->valuestring, "todos", 5))
				{

				}

				break;
			}

			Serial.println(aJson.print(pacote));
			aJson.deleteItem(pacote);

			delay(10);
		}
	}
}