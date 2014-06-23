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

void buzz(uint16_t tempo)
{

}

void setup()
{
	bool somethingWrong = false;

	Serial.begin(9600);

	// Quick fix
	pinMode(FIX_OUTPUT, OUTPUT);

	// Inicializa o servidor
	if (DIRECTCONNECTION) {
		Ethernet.begin(mac, IPADDRESS, DNSSERVER, GATEWAY, SUBNETMASK);
	} else {
		Ethernet.begin(mac, IPADDRESS);
	}

	// Inicializa o cartao SD
	if (!SD.begin(4))
	{
		DEBUG("SD nao iniciou");
		somethingWrong = true;
	}

	// Inicializa o TWI
	Wire.begin();

	lcd.begin(16, 2);
	lcd.backlight();
	
	// Inicializa o módulo RTC
	rtc.begin();
	
	// Inicializa o NFC modulo
	nfc.begin();
	
	if (!nfc.getFirmwareVersion()) {
		DEBUG("PN532 nao iniciou");
		somethingWrong = true;
	}

	nfc.setPassiveActivationRetries(0x02);
	nfc.SAMConfig();
	
	// Inicializa a minha classe de Arquivos
	Arquivos.init(&rtc);

	// Debugging
	DEBUG(Ethernet.localIP());

	lcd.setCursor(0, 1);
	lcd.print(Ethernet.localIP());

	// Atualiza o millis

	contadorHora = millis();
	contadorMensagem = millis();

	delay(1000);

	if (somethingWrong)
	{
		lcd.clear();
		lcd.print("ERROR");
		while (true);
	}
}

void mostraHora()
{
	uint32_t mill = millis();

	if (mill - contadorHora > 1000)
	{
		if (++contadorUltimoId == 10)
		{
			ultimoId = 0;
			contadorUltimoId = 0;
		}

		DateTime tempo = rtc.now();

		DEBUG(tempo.unixtime());

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

			Arquivos.getConteudo(fullpath, buffer);

			uint32_t idcolaborador = atol(buffer);

			if (ultimoId != idcolaborador)
			{
				ultimoId = idcolaborador;
				Arquivos.marcarPonto(ultimoId);
				
				lcd.setCursor(0, 1);
				lcd.print("Ponto marcado");

				//reset delay mensagem
				contadorMensagem = millis();

				//Delay do ultimoId
				contadorUltimoId = 0;
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
			case CONSULTAR_EMPREGADOR:
			{
				char empregador[256];

				bool resultado = Arquivos.consultarEmpregador(empregador);

				if (!resultado)
				{
					pacote.enviarNulo(&cliente);
					break;
				}

				strcpy((char*) pacote.buffer, empregador);
				pacote.tamanho = strlen((char*) pacote.buffer);
				pacote.tipo = EMPREGADOR_CONSULTADO;
				pacote.enviar(&cliente);
				break;
			}
			case CADASTRAR_EMPREGADOR:
			{
				bool resultado = Arquivos.incluirEmpregador((char*)pacote.buffer);

				if (!resultado)
				{
					pacote.enviarNulo(&cliente);
					break;
				}

				pacote.enviar(&cliente, EMPREGADOR_CADASTRADO);
				break;
			}
			case ALTERAR_EMPREGADOR: 
			{
				bool resultado = Arquivos.alterarEmpregador((char*)pacote.buffer);

				if (!resultado)
				{
					pacote.enviarNulo(&cliente);
					break;
				}

				pacote.enviar(&cliente, EMPREGADOR_CADASTRADO);
				break;
			}
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
					char base[10];
					char aux[15];

					uint32_t idc = strtoul(entry.name(), NULL, 16);

					ultoa(idc, base, 10);
					strcat(base, ";");

					char nextread = entry.read();

					while (entry.available())
					{
						for (int i = 0; nextread != '\n' && nextread != '\r' && entry.available(); i++, nextread = entry.read())
						{
							aux[i] = nextread;
							aux[i + 1] = '\0';
						}

						while (nextread == '\r' || nextread == '\n')
						{
							nextread = entry.read();
						}

						strcpy((char*) pacote.buffer, base);
						strcat((char*) pacote.buffer, aux);

						DEBUG((char*) pacote.buffer);

						pacote.tipo = PONTO_LISTADO;
						pacote.tamanho = strlen((char*) pacote.buffer);
						pacote.enviar(&cliente);
					}
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

					if (SD.exists(fullpath))
					{
						strcpy((char*) pacote.buffer, "0");
						pacote.tamanho = 1;
						pacote.enviar(&cliente);
						break;

						delay(3000);
					}

					File file = SD.open(fullpath, FILE_WRITE);

					file.print((char*) pacote.buffer);
					file.close();
				}

				strcpy((char*) pacote.buffer, path);
				pacote.tamanho = strlen(path);
				pacote.tipo = CARTAO_DEFINIDO;
				pacote.enviar(&cliente);

				nfc.setPassiveActivationRetries(0x02);

				delay(3000); // TODO: AUMENTAR ISSO AQUI DEPOIS!!!! OU FAZER O TRECO LOCO LA DO ULTIMO ID =)

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

											//nao realizado para o prototipo
				
				break;
			}
			case LISTAR_COLABORADORES: {

				for (uint32_t i = 1; i <= Arquivos.getIdColaborador(); i++)
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

				//nao realizado para o prototipo

				break;
			}
			case EXCLUIR_COLABORADOR: {

				//nao realizado para o prototipo

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