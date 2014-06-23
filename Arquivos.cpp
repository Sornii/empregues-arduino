#include "Arquivos.h"
#include "SD.h"
#include "Constants.h"

// O arquivo ID.TXT servir� para guardar e servir o
// n�mero de identifica��o dos empregados para o
// controle do banco de dados.

char idFile [] = "ID";
char idPontoFile [] = "PONTO/ID";

char pastaCol [] = "COLAB/";
char pastaRfid [] = "RFID/";
char pastaPonto [] = "PONTO/";
char empregador [] = "EMPDOR";

#pragma region Private Methods

/* N�o usaremos mais NSR por causa do tipo de mem�ria
void ArquivosClass::getNsrFromFile()
{
	char buffer[20];

	File file = SD.open(nsrFile, FILE_READ);
	uint8_t tamanho = file.available();
	
	file.read(buffer, tamanho);
	file.close();

	nsr = atol(buffer);
}

void ArquivosClass::setNsrInFile()
{
	SD.remove(nsrFile);

	File file = SD.open(nsrFile, FILE_WRITE);
	file.print(nsr);
	file.close();
}

void ArquivosClass::incrementNsr()
{
nsr += 1;
setNsrInFile();
}
*/

void ArquivosClass::getIdColaboradorFromFile()
{
	char buffer[20];

	File file = SD.open(idFile, FILE_READ);
	uint8_t tamanho = file.available();

	file.read(buffer, tamanho);
	file.close();

	idColaborador = atol(buffer);
}

void ArquivosClass::setIdColaboradorInFile()
{
	SD.remove(idFile);

	File file = SD.open(idFile, FILE_WRITE);
	file.print(idColaborador);
	file.close();
}

void ArquivosClass::incrementIdColaborador()
{
	idColaborador += 1;
	setIdColaboradorInFile();
}

void ArquivosClass::getIdPontoFromFile()
{
	char buffer[20];

	File file = SD.open(idFile, FILE_READ);
	uint8_t tamanho = file.available();

	file.read(buffer, tamanho);
	file.close();

	idPonto = atol(buffer);
}

void ArquivosClass::setIdPontoInFile()
{
	SD.remove(idFile);

	File file = SD.open(idFile, FILE_WRITE);
	file.print(idPonto);
	file.close();
}

void ArquivosClass::incrementIdPonto()
{
	idPonto += 1;
	setIdColaboradorInFile();
}

void ArquivosClass::addint(int i, char* c)
{
	char aux[10];
	itoa(i, aux, 10);
	strcat(c, aux);
}

void ArquivosClass::getMesAno(DateTime &d, char* out)
{
	char caminho[10];
	char caux[10];

	uint8_t mes = d.month();
	uint16_t ano = d.year();

	//adiciona o ano na string e.g. "2014"
	itoa(ano, caminho, 10);

	//adiciona um slash para uma nova e.g. "2014/"
	strcat(caminho, "/");

	//aidiona o mes e.g. "2014/1"
	itoa(mes, caux, 10);
	strcat(caminho, caux);

	//Serial.println(caminho);

	strcpy(out, caminho);
}

void ArquivosClass::pastaPonto(DateTime &now, char* out)
{
	char caminhoTotal[20] = "PONTO/";
	char caminhoParcial[10];

	getMesAno(now, caminhoParcial);

	//ok, temos e.g. 2014/1

	//vamos adicionar ao caminhototal para a verificacao
	strcat(caminhoTotal, caminhoParcial);

	if (!SD.exists(caminhoTotal))
		SD.mkdir(caminhoTotal);

	//da a saida do caminho e.g. PONTO/2014/1
	strcat(caminhoTotal, "/");
	strcpy(out, caminhoTotal);
}

void ArquivosClass::getConteudo(char* path, char* out)
{
	File file = SD.open(path, FILE_READ);
	getConteudo(&file, out);
}

void ArquivosClass::getConteudo(File* file, char* out)
{
	uint32_t tam = file->available();
	file->read(out, tam);
	file->close();

	out[tam] = 0;
}

#pragma endregion

#pragma region Public Methods

// Inicializacao dos arquivos
void ArquivosClass::init(RTC_DS1307* rtc)
{
	_rtc = rtc;

	idColaborador = idPonto = 0;

	SD.remove(idFile);
	SD.remove(idPontoFile);
	
	File file = SD.open("COLAB/");
	File entry;

	while (entry = file.openNextFile())
	{
		char nome[20] = "COLAB/";
		strcat(nome, entry.name());
		entry.close();
		SD.remove(nome);
	}

	file.close();

	file = SD.open("RFID/");

	while (entry = file.openNextFile())
	{
		char nome[20] = "RFID/";
		strcat(nome, entry.name());
		entry.close();
		SD.remove(nome);
	}

	file.close();

	file = SD.open("PONTO/2014/4/");

	while (entry = file.openNextFile())
	{
		char nome[20] = "PONTO/2014/4/";
		strcat(nome, entry.name());
		entry.close();
		SD.remove(nome);
	}

	file.close();
}

uint32_t ArquivosClass::getIdColaborador()
{
	return idColaborador;
}

uint32_t ArquivosClass::getIdPonto()
{
	return idPonto;
}

void ArquivosClass::marcarPonto(uint32_t id)
{
	incrementIdPonto();

	char caminhoPonto[40];
	char aux[20];

	//precisamos chegar na pasta de ponto atual
	//primeiro vamos checar se ela existe, se nao, criaremos.
	DateTime now = _rtc->now();

	pastaPonto(now, caminhoPonto);

	if (SD.exists(caminhoPonto))
	{
		DEBUG("Criado");
	}

	//vamos agora adicionar o id do colaborador ao caminho
	ultoa(id, aux, 16);
	strcat(caminhoPonto, aux);

	DEBUG(caminhoPonto);

	//vamos abrir um arquivo com o id no colaborador
	//e entao colocarmos o o ponto que sera no seguinte formato
	//DIA;HORA;MINUTO

	//31;23;59

	File file = SD.open(caminhoPonto, FILE_WRITE);

	char ponto[10] = "";

	addint(idPonto, ponto);
	strcat(ponto, ";");
	addint(now.day(), ponto);
	strcat(ponto, ";");
	addint(now.hour(), ponto);
	strcat(ponto, ";");
	addint(now.minute(), ponto);

	DEBUG(ponto);

	file.println(ponto);
	file.close();
}



uint32_t ArquivosClass::incluirColaborador(char* pessoa){
	char fullpath[30];
	char path[10];

	strcpy(fullpath, pastaCol);
	
	incrementIdColaborador();
	ltoa(idColaborador, path, 16);

	strcat(fullpath, path);

	File file = SD.open(fullpath, FILE_WRITE);

	file.print(pessoa);
	file.close();

	return idColaborador;
}

bool ArquivosClass::consultarColaborador(char* out, uint32_t thisid)
{
	char fullpath[30];
	strcpy(fullpath, pastaCol);

	char hexid[8];
	ltoa(thisid, hexid, 16);

	strcat(fullpath, hexid);

	if (SD.exists(fullpath))
	{
		getConteudo(fullpath, out);
		return true;
	}

	return false;
}


bool ArquivosClass::incluirEmpregador(char* empregadorStr)
{
	if (SD.exists(empregador))
		return false;

	File file = SD.open(empregador, FILE_WRITE);
	file.print(empregadorStr);
	file.close();
}

bool ArquivosClass::consultarEmpregador(char* out)
{
	if (!SD.exists(empregador))
		return false;

	char bufferEmpregador[BUFFER_SIZE];

	getConteudo(empregador, bufferEmpregador);

	strcpy(out, bufferEmpregador);

	return true;
}

bool ArquivosClass::alterarEmpregador(char* empregadorStr)
{
	if (!SD.exists(empregador))
		return false;

	SD.remove(empregador);

	File file = SD.open(empregador, FILE_WRITE);
	file.print(empregadorStr);
	file.close();
}


#pragma endregion


ArquivosClass Arquivos;

