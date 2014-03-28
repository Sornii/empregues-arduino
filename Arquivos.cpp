#include "Arquivos.h"
#include "SD.h"
#include "Constants.h"

// O arquivo NSR.TXT servir� para guardar e servir o
// n�mero sequencial de registro, para cada opera��o na
// "mem�ria de registro principal" ou "mem�ria de
// registro de ponto".

// O arquivo ID.TXT servir� para guardar e servir o
// n�mero de identifica��o dos empregados para o
// controle do banco de dados.


const char idp[] = "ID.TXT";
const char nsr[] = "NSR.TXT";

const char mrp_ajusteRelogio[] = "MRP_AREL.TXT";
const char mrp_historicoEmpregador[] = "MRP_EMP.TXT";
const char mrp_historicoColaborador[] = "MRP_COL.TXT";
const char mrp_marcacaoPonto[] = "MRP_MPT.TXT";
const char mt_colaborador[] = "MT_COL.TXT";
const char mt_empregador[] = "MT_EMP.TXT";

const char* arq[] = 
{
	idp, nsr, mrp_ajusteRelogio, mrp_historicoEmpregador,
	mrp_historicoColaborador, mrp_marcacaoPonto,
	mt_colaborador, mt_empregador 
};

#pragma region Private Methods

void ArquivosClass::initArray(char* buffer)
{
	initArray(buffer, 256);
}

void ArquivosClass::initArray(char* buffer, uint16_t size)
{
	for (int i = 0; i < size; i++) buffer[i] = '\0';
}

// Com o arquivo NSR.TXT separado do restante, � feito a
// sua leitura para gerar os n�meros sequenciais de
// registro que s�o usadas para armazenadar qualquer
// opera��o realizada no ERP.

// Assim como o arquivo ID.TXT tamb�m � separado e feito
// a sua leitura para gerar n�meros de identifica��o

long ArquivosClass::leituraNumero(const char* arqv, bool incrementar){
	File arquivo = SD.open(arqv, FILE_READ);
	
	char buffer[10];

	int tamanho = arquivo.available();
	buffer[tamanho] = FIM_STRING;

	for (int i = 0; i < tamanho; i++)
		buffer[i] = arquivo.read();

	arquivo.close();

	long numero = atol(buffer);

	if (incrementar == false)
		return numero;

	SD.remove((char*) arqv);

	numero += 1;

	arquivo = SD.open(arqv, FILE_WRITE);
	arquivo.print(numero);
	arquivo.close();

	return numero;
}

long ArquivosClass::leituraNSR(bool incrementar = true)
{
	return leituraNumero(nsr, incrementar);
}

long ArquivosClass::leituraId(bool incrementar = true)
{
	return leituraNumero(idp, incrementar);
}

// O circuito respons�vel por guardar a dada � chamado de
// RTC (Real-time-clock), e � deste camarada que � feito
// a recupera��o da data e hora a qualquer momento para
// auxiliar no conhecimento do tempo no exato momento
// da grava��o de dados que necessitam de data e hora.

// Nesta fun��o � feito a recupera��o da data.

String ArquivosClass::getData()
{
	return "11/02/2014";
}

// Nesta fun��o � feito a recupera��o da hora.

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

#pragma endregion

#pragma region Public Methods

// Inicializacao dos arquivos
int ArquivosClass::init()
{
	for (int i = 0; i < QUANTIDADE_ARQUIVO; i++)
	{
		SD.remove((char*) arq[i]);
		File _thisFile = SD.open(arq[i]);
		if (arq[i] == "NSR.TXT" || arq[i] == "ID.TXT")
			_thisFile.print("0");
		_thisFile.close();
	}
}

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

long ArquivosClass::incluirColaborador(char* pessoa){
	File fpessoa = SD.open(mt_colaborador, FILE_WRITE);

	long id = leituraId();

	fpessoa.print(id);
	fpessoa.print(';');
	fpessoa.print(pessoa);
	fpessoa.print('$');
	fpessoa.close();

	salvarHistoricoColaborador(pessoa, INCLUSAO);

	return id;
}

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

#pragma endregion


ArquivosClass Arquivos;

