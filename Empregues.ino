#define TWI_FREQ 400000L

#include <Wire.h>
#include <SPI.h>

#include <Adafruit_NFCShield_I2C.h>
#include <LiquidCrystal_I2C.h>
#include <RTClib.h>
#include <Ethernet.h>
#include <SD.h>

#include "Pacote.h"
#include "Arquivos.h"
#include "Constants.h"

uint32_t ultimoId = 0;
uint32_t contadorUltimoId = 0;

uint32_t contadorHora;
uint32_t contadorMensagem;

LiquidCrystal_I2C lcd(LCD_ADDRESS, 2, 1, 0, 4, 5, 6, 7, BACKLIGHT_PIN, POSITIVE);

RTC_DS1307 rtc;

Adafruit_NFCShield_I2C nfc(NFC_IRQ, NFC_RESET);

EthernetServer server(SERVER_PORTA);
EthernetUDP Udp;

byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };

void setup()
{
	//pinMode(BUZZER_PIN, OUTPUT);

	Serial.begin(9600);

	// Quick fix
	pinMode(FIX_OUTPUT, OUTPUT);

	// Inicializa o servidor
	Ethernet.begin(mac, IPAddress(10, 1, 1, 14));

	// Inicializa o cartao SD
	if (!SD.begin(4))
	{
		Serial.println("SD nao iniciou");
	}

	Serial.println(Ethernet.localIP());

	// Inicializa o TWI
	Wire.begin();

	lcd.begin(16, 2);
	lcd.backlight();
	
	// Inicializa o m�dulo RTC
	rtc.begin();
	//rtc.adjust(DateTime(1396575600));

	lcd.print(rtc.now().unixtime());
	
	// Inicializa o NFC modulo
	nfc.begin();
	
	if (!nfc.getFirmwareVersion()) {
		Serial.print("PN532 nao iniciou");
	}

	nfc.setPassiveActivationRetries(0x02);
	nfc.SAMConfig();
	
	// Atualiza o millis

	contadorHora = millis();

	// Inicializa a minha classe de Arquivos
	Arquivos.init(&rtc);
}

void mostraHora()
{
	uint32_t mill = millis();

	if (mill - contadorHora > 1000)
	{
		contadorUltimoId++;

		DateTime tempo = rtc.now();

		Serial.println(tempo.unixtime());

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
		lcdvazio(8);

		contadorHora = mill;
	}
}

void lcdvazio(uint8_t quantidade)
{
	for (int i = 0; i < quantidade; i++){
		lcd.print(" ");
	}
}

void limpaMensagem()
{
	uint32_t mill = millis();

	if (mill - contadorMensagem > 5000)
	{
		lcd.setCursor(0, 1);
		lcdvazio(16);

		contadorMensagem = mill;
	}
}

void uint8_cat(char* out, uint8_t in)
{
	char aux[5];
	itoa(in, aux, 10);
	strcat(out, aux);
	strcat(out, "/");
}

void iatohexa(uint8_t* ia, char* out)
{
	//Inicializa a string
	strcpy(out, "");

	//Copia o conteudo convertendo-o para hex
	for (uint8_t i = 0; i < 4; i++)
	{
		char aux[3];
		itoa(ia[i], aux, 16);
		strcat(out, aux);
	}
}

void loop()
{
	mostraHora();
	limpaMensagem();

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

#pragma region Marca ponto

	boolean success = false;
	uint8_t uid [] = { 0, 0, 0, 0, 0, 0, 0 };
	uint8_t uidLength;
	
	success = nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, uid, &uidLength);

	if (success)
	{
		char fullpath[20] = "RFID/";
		char path[9];

		iatohexa(uid, path);
		strcat(fullpath, path);
		
		if (SD.exists(fullpath))
		{
			char buffer[20];

			getConteudo(fullpath, buffer);

			uint32_t idcolaborador = atol(buffer);

			if (ultimoId != idcolaborador)
			{
				ultimoId = idcolaborador;
								
				Arquivos.marcarPonto(ultimoId);
				
				lcd.setCursor(0, 1);
				lcd.print("Ponto marcado");

			}
		}
	}

#pragma endregion

#pragma region Comunicacao rede

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
			case LISTAR_PONTOS:
			{
				char fullpath[40] = "PONTO/";

				strcat(fullpath, (char*) pacote.buffer);

				//e.g. PONTO/2014/1

				File dir = SD.open(fullpath);
				File entry;

				//envio e.g. idcolab;dia;hora;minuto

				while (entry = dir.openNextFile())
				{
					char conteudo[30];

					uint32_t idc = strtoul(entry.name(), NULL, 16);

					ultoa(idc, (char*) pacote.buffer, 10);
					strcat((char*) pacote.buffer, ";");

					getConteudo(&entry, conteudo);

					strcat((char*) pacote.buffer, conteudo);

					pacote.tipo = PONTO_LISTADO;
					pacote.tamanho = strlen((char*) pacote.buffer);
					pacote.enviar(&cliente);
				}

				pacote.enviarNulo(&cliente);

				break;
			}
			case LISTAR_CARTOES:
			{
				File file = SD.open("RFID/");

				for (File entry = file.openNextFile(); entry; entry = file.openNextFile())
				{
					strcpy((char*) pacote.buffer, entry.name());

					char read[20];

					uint32_t tam = entry.available();
					entry.read(read, tam);
					entry.close();

					read[tam] = 0;

					strcat((char*) pacote.buffer, ";");
					strcat((char*) pacote.buffer, read);

					pacote.tamanho = strlen((char*) pacote.buffer);
					pacote.tipo = CARTAO_CONSULTADO;

					pacote.enviar(&cliente);
				}

				pacote.enviarNulo(&cliente);

				break;
			}
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

				nfc.setPassiveActivationRetries(0xFA);

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

				strcpy((char*) pacote.buffer, path);
				pacote.tamanho = strlen(path);
				pacote.tipo = CARTAO_DEFINIDO;
				pacote.enviar(&cliente);

				nfc.setPassiveActivationRetries(0x02);

				delay(1000); // TODO: AUMENTAR ISSO AQUI DEPOIS!!!!

				break;
			}
			case CADASTRAR_COLABORADOR: {

				/*
				
				Cadastrado do colaborador 
				
				A informa��o deve vir obrigatoriamente da seguinte forma

				PIS     |  NOME
				38231283;Fulano da Silva
				
				*/

				uint32_t idCadastrado = Arquivos.incluirColaborador((char*) pacote.buffer);

				/*
				
				Envia a resposta com o id cadastrado
				
				O valor ser� enviado da seguinte forma

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

				for (uint32_t i = 1; i <= Arquivos.getId(); i++)
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

				pacote.enviarNulo(&cliente);

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

#pragma endregion
}