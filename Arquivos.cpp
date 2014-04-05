#include "Arquivos.h"
#include "SD.h"
#include "Constants.h"

// O arquivo NSR.TXT servirá para guardar e servir o
// número sequencial de registro, para cada operação na
// "memória de registro principal" ou "memória de
// registro de ponto".

// O arquivo ID.TXT servirá para guardar e servir o
// número de identificação dos empregados para o
// controle do banco de dados.


char idFile [] = "ID";
char nsrFile [] = "NSR";

char pastaCol [] = "COLAB/";
char pastaRfid [] = "RFID/";

/*

const char mrp_ajusteRelogio[] = "MRP_AREL.TXT";
const char mrp_historicoEmpregador[] = "MRP_EMP.TXT";
const char mrp_historicoColaborador[] = "MRP_COL.TXT";
const char mrp_marcacaoPonto[] = "MRP_MPT.TXT";
const char mt_colaborador[] = "MT_COL.TXT";
const char mt_empregador[] = "MT_EMP.TXT";

const char* arq[] = 
{
	mrp_ajusteRelogio, mrp_historicoEmpregador,
	mrp_historicoColaborador, mrp_marcacaoPonto,
	mt_colaborador, mt_empregador 
};

*/

#pragma region Private Methods

/*
void ArquivosClass::initArray(char* buffer)
{
	initArray(buffer, 256);
}

void ArquivosClass::initArray(char* buffer, uint16_t size)
{
	for (int i = 0; i < size; i++) buffer[i] = '\0';
}*/

// Com o arquivo NSR.TXT separado do restante, é feito a
// sua leitura para gerar os números sequenciais de
// registro que são usadas para armazenadar qualquer
// operação realizada no ERP.

// Assim como o arquivo ID.TXT também é separado e feito
// a sua leitura para gerar números de identificação

void ArquivosClass::getNsr()
{
	char buffer[20];

	File file = SD.open(nsrFile, FILE_READ);
	uint8_t tamanho = file.available();
	
	file.read(buffer, tamanho);
	file.close();

	nsr = atol(buffer);
}

void ArquivosClass::setNsr()
{
	SD.remove(nsrFile);

	File file = SD.open(nsrFile, FILE_WRITE);
	file.print(nsr);
	file.close();
}

void ArquivosClass::getId()
{
	char buffer[20];

	File file = SD.open(idFile, FILE_READ);
	uint8_t tamanho = file.available();

	file.read(buffer, tamanho);
	file.close();

	id = atol(buffer);
}

void ArquivosClass::setId()
{
	SD.remove(idFile);

	File file = SD.open(idFile, FILE_WRITE);
	file.print(id);
	file.close();
}

void ArquivosClass::incrementId()
{
	id += 1;
	setId();
}

void ArquivosClass::incrementNsr()
{
	nsr += 1;
	setNsr();
}

// O circuito responsável por guardar a dada é chamado de
// RTC (Real-time-clock), e é deste camarada que é feito
// a recuperação da data e hora a qualquer momento para
// auxiliar no conhecimento do tempo no exato momento
// da gravação de dados que necessitam de data e hora.

// Nesta função é feito a recuperação da data.
/*
String ArquivosClass::getData()
{
	return "11/02/2014";
}

// Nesta função é feito a recuperação da hora.

String ArquivosClass::getHora()
{
	return "15:57";
}

void ArquivosClass::gravarEmpregador(String empregador, char metodo)
{
	String historico = String("");

	if (SD.exists((char*) mt_empregador)){
		SD.remove((char*) mt_empregador);
	}

	File arquivo = SD.open(mt_empregador, FILE_WRITE);
	arquivo.print(empregador);
	arquivo.close();

	arquivo = SD.open(mrp_historicoEmpregador, FILE_WRITE);

	historico += leituraNSR(false);
	historico += ';' + getData();
	historico += ';' + getHora();
	historico += ';' + metodo;
	historico += ';' + empregador;

	arquivo.print(historico);
	arquivo.print('$');

	arquivo.close();
}



void ArquivosClass::salvarHistoricoColaborador(char* pessoa, char operacao)
{
	File file = SD.open(mrp_historicoColaborador, FILE_WRITE);

	file.print(leituraNSR());
	file.print(SEPARADOR);
	file.print(getData());
	file.print(SEPARADOR);
	file.print(getHora());
	file.print(SEPARADOR);
	file.print(operacao);
	file.print(SEPARADOR);
	file.print(pessoa);
	file.print(SEPARADOR_GERAL);

	file.close();
}

*/

#pragma endregion

#pragma region Public Methods

// Inicializacao dos arquivos
int ArquivosClass::init()
{
	/*
	for (int i = 0; i < QUANTIDADE_ARQUIVO; i++)
	{
		SD.remove((char*) arq[i]);
		File _thisFile = SD.open(arq[i]);
		if (arq[i] == "NSR.TXT" || arq[i] == "ID.TXT")
			_thisFile.print("0");
		_thisFile.close();
	}
	*/

	nsr = 0;
	id = 0;

	SD.remove(nsrFile);
	SD.remove(idFile);

	File file = SD.open(nsrFile);
	file.print("0");
	file.close();

	file = SD.open(idFile);
	file.print("1");
	file.close();

	SD.remove("COLAB/1");
	SD.remove("COLAB/2");
	SD.remove("COLAB/3");
	SD.remove("COLAB/4");
	SD.remove("COLAB/5");
	SD.remove("COLAB/6");
	SD.remove("COLAB/7");
	SD.remove("COLAB/8");
	SD.remove("COLAB/9");
}

uint32_t ArquivosClass::Id()
{
	return id;
}

/*

String ArquivosClass::leituraEmpregador()
{
	char* buff;
	File arquivo = SD.open(mt_empregador, FILE_READ);
	int empSize = arquivo.available();

	buff = new char[empSize + 1];

	for (int i = 0; arquivo.available() > 0; i++)
		buff[i] = arquivo.read();

	buff[empSize] = '\0';

	arquivo.close();
	String empregador = String(buff);

	return buff;
}

void ArquivosClass::incluirEmpregador(String empregador)
{
	gravarEmpregador(empregador, INCLUSAO);
}

void ArquivosClass::alterarEmpregador(String empregador)
{
	gravarEmpregador(empregador, ALTERACAO);
}


void ArquivosClass::alterarColaborador(char* pessoa){
	File fpessoa = SD.open(mt_colaborador, FILE_WRITE);

	int _id = 0;
	int idAux = 0;
	char* pch = strtok(pessoa, ";");
	char nAux[20];
	_id = atoi(pch);
	int lastPos = 0;
	bool existe = true;

	while (idAux != _id){
		initArray(nAux);

		lastPos = fpessoa.position();

		if (fpessoa.readBytesUntil(';', nAux, 20) <= 0) {
			existe = false;
			break;
		}

		if (nAux[0] != '!')
			idAux = atoi(nAux);

		if (idAux != _id) fpessoa.find("$");
	}

	if (existe){
		fpessoa.seek(lastPos);
		fpessoa.print('!');
		fpessoa.seek(fpessoa.size());
		fpessoa.print(pessoa);
		fpessoa.print('$');

		salvarHistoricoColaborador(pessoa, ALTERACAO);
	}

	fpessoa.close();
}

*/

uint32_t ArquivosClass::incluirColaborador(char* pessoa){
	char fullpath[30];
	char path[10];

	strcpy(fullpath, pastaCol);
	
	incrementId();
	ltoa(id, path, 16);

	strcat(fullpath, path);

	File file = SD.open(fullpath, FILE_WRITE);

	file.print(pessoa);
	file.close();

	return id;
}

bool ArquivosClass::consultarColaborador(char* out, uint32_t thisid)
{
	char fullpath[30];
	char path[8];

	strcpy(fullpath, pastaCol);
	ltoa(thisid, path, 16);
	strcat(fullpath, path);

	if (SD.exists(fullpath))
	{
		File file = SD.open(fullpath, FILE_READ);
		uint8_t tamanho = file.available();
		file.read(out, tamanho);
		file.close();

		out[tamanho] = 0;

		return true;
	}

	return false;
}

/*
bool ArquivosClass::proximoColaborador(File fpessoa, char* pessoa){
	char buffer[256];
	initArray(buffer);
	if (fpessoa.readBytesUntil('$', buffer, 256) <= 0) return false;
	if (buffer[0] == '!'){
		initArray(buffer);
		fpessoa.readBytesUntil('$', buffer, 256);
	}
	strcpy(pessoa, buffer);
	return true;
}

bool ArquivosClass::consultarColaborador(char* pessoa, int id){
	File fpessoa = SD.open(mt_colaborador, FILE_READ);

	int _id = 0;
	char bufferId[20];

	char buffer[256];
	char aux[256];

	initArray(buffer);
	initArray(aux);
	initArray(pessoa);

	while (_id != id)
	{
		initArray(bufferId, 20);
		if (fpessoa.readBytesUntil(';', bufferId, 20) <= 0) {
			fpessoa.close();
			return false;
		}

		if (bufferId[0] != '!')
			_id = atoi(bufferId);

		if (_id != id) fpessoa.find("$");
	}

	fpessoa.readBytesUntil('$', aux, 256);
	fpessoa.close();

	strcpy(buffer, bufferId);
	strcat(buffer, ";");
	strcat(buffer, aux);

	strcpy(pessoa, buffer);

	return true;
}
*/


#pragma endregion


ArquivosClass Arquivos;

