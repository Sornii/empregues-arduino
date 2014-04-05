#ifndef TWI_FREQ
#define TWI_FREQ 400000L
#endif

#include <Wire.h>
#include <Adafruit_NFCShield_I2C.h>
#include <LiquidCrystal_I2C.h>
#include <LCD.h>
#include <I2CIO.h>
#include <FastIO.h>
#include "Pacote.h"
#include "Arquivos.h"
#include "Constants.h"
#include <SPI.h>
#include <SD.h>

#include <RTClib.h>
#include <Ethernet.h>

uint32_t lastmill;

LiquidCrystal_I2C lcd(LCD_ADDRESS, BACKLIGHT_PIN, POSITIVE);

RTC_DS1307 rtc;

Adafruit_NFCShield_I2C nfc(IRQ, RESET);

EthernetServer server(SERVER_PORTA);
EthernetUDP Udp;

byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };

void setup()
{
	//pinMode(BUZZER_PIN, OUTPUT);

	/* EthernetShield */

	// Quick fix
	pinMode(FIX_OUTPUT, OUTPUT);
	// Comeca em 10.1.1.14
	Ethernet.begin(mac, IPAddress(10,1,1,14));

	// Inicializa o cartao SD
	SD.begin(4);

	// Inicializa a minha classe de Arquivos
	Arquivos.init();

	// Coloca na tela o IP, se mostrar diferente
	// do posto acima, deu erro.
	Serial.println(Ethernet.localIP());

	/* I2C */

	// Inicializa o TWI
	Wire.begin();

	lcd.begin(16, 2);
	lcd.backlight();

	lcd.print(Ethernet.localIP());

	// Inicializa o módulo RTC
	rtc.begin();
	rtc.adjust(DateTime(1396575600));

	DateTime tempo = rtc.now();

	lcd.home();

	lcd.print(tempo.unixtime());

	delay(3000);
	

	/* NFC/RFID */

	nfc.begin();
	uint32_t versiondata = nfc.getFirmwareVersion();
	
	if (!versiondata) {
		Serial.print("Didn't find PN53x board");
		while (1); // halt
	}

	nfc.setPassiveActivationRetries(0x02);
	
	nfc.SAMConfig();

	

	lastmill = millis();
}



void loop()
{
	uint32_t mill = millis();

	if (mill - lastmill > 1000)
	{
		DateTime tempo = rtc.now();

		lcd.home();
		if (tempo.hour() < 10){
			lcd.print("0");
		}
		lcd.print(tempo.hour());
		lcd.print(":");
		if (tempo.minute() < 10){
			lcd.print("0");
		}
		lcd.print(tempo.minute());
		lcd.print(":");
		if (tempo.second() < 10){
			lcd.print("0");
		}
		lcd.print(tempo.second());
		lcd.print("        ");
	}


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
	boolean success = false;
	uint8_t uid [] = { 0, 0, 0, 0, 0, 0, 0 };
	uint8_t uidLength;
	
	success = nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, uid, &uidLength);

	if (success)
	{

#ifndef SEM_APITO_POR_HOJE

		digitalWrite(BUZZER_PIN, HIGH);
		delay(DELAY_APITO);
		digitalWrite(BUZZER_PIN, LOW);

#endif

		char fullpath[20] = "RFID/";
		char path[9] = "";

		for (uint8_t i = 0; i < 4; i++)
		{
			char aux[3];
			itoa(uid[i], aux, 16);
			strcat(path, aux);
		}

		strcat(fullpath, path);
		
		if (SD.exists(fullpath))
		{
			char buffer[256];
			File file = SD.open(fullpath, FILE_READ);
			uint8_t tamanho = file.available();
			file.read(buffer, tamanho);
			file.close();
			buffer[tamanho] = 0;

			lcd.setCursor(0, 1);
			lcd.print(buffer);

#ifndef SEM_APITO_POR_HOJE
			digitalWrite(BUZZER_PIN, HIGH);
			delay(50);
			digitalWrite(BUZZER_PIN, LOW);
#endif
			delay(1000);

			uint32_t idLido = atol(buffer);
			ltoa(idLido, buffer, 16);

			char colabpath[20] = "COLAB/";

			strcat(colabpath, buffer);

			file = SD.open(colabpath, FILE_READ);
			tamanho = file.available();
			file.read(buffer, tamanho);
			file.close();
			buffer[tamanho] = 0;

			lcd.setCursor(0, 1);
			lcd.print(buffer);
			delay(1000);


		}

		lcd.clear();
		lcd.print(path);
		lcd.setCursor(0, 1);
		lcd.print(fullpath);

#ifndef SEM_APITO_POR_HOJE

		delay(DELAY_PAUSA);
		digitalWrite(BUZZER_PIN, HIGH);
		delay(DELAY_APITO);
		digitalWrite(BUZZER_PIN, LOW);

#endif

	}

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
			case ALTERAR_RELOGIO:
			{
				uint32_t unixtime = atol((char*) pacote.buffer);
				rtc.adjust(DateTime(unixtime));
				rtc.begin();

				break;
			}
			case DEFINIR_CARTAO_COLABO: {

				char fullpath[20] = "RFID/";
				char path[9] = "";

				nfc.setPassiveActivationRetries(0xAA);

				success = nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, uid, &uidLength);
				if (success)
				{

					for (uint8_t i = 0; i < 4; i++)
					{
						char aux[3];
						itoa(uid[i], aux, 16);
						strcat(path, aux);
					}

					strcat(fullpath, path);

					SD.remove(fullpath);

					File file = SD.open(fullpath, FILE_WRITE);

					file.print((char*) pacote.buffer);
					file.close();
#ifndef SEM_APITO_POR_HOJE
					for (uint8_t i = 0; i < 3; i++)
					{
						digitalWrite(BUZZER_PIN, HIGH);
						delay(50);
						digitalWrite(BUZZER_PIN, LOW);
					}
#endif
				}

				nfc.setPassiveActivationRetries(0x02);

				delay(1000); // TODO: AUMENTAR ISSO AQUI DEPOIS!!!!

				break;
			}
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
						char buffer[12];

						ltoa(i, buffer, 10);
						strcat(buffer, ";");

						pacote.insertBefore(buffer);
						
						pacote.tamanho = strlen((char*) pacote.buffer);
						pacote.tipo = COLABORADOR_LISTADO;
						pacote.enviar(&cliente);
					}
				}

				pacote.tipo = PACOTE_NULO;
				pacote.tamanho = 1;

				pacote.enviar(&cliente);

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