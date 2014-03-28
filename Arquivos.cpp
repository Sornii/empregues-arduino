// 
// 
// 

#include "Arquivos.h"
#include "SD.h"
#include "aJSON.h"

const char idp[] = "ID.TXT";
const char nsr[] = "NSR.TXT";

const char mrp_ajusteRelogio[] = "MRP_AREL.TXT";
const char mrp_historicoEmpregador[] = "MRP_EMP.TXT";
const char mrp_historicoColaborador[] = "MRP_COL.TXT";
const char mrp_marcacaoPonto[] = "MRP_MPT.TXT";
const char mt_colaborador[] = "MT_COL.TXT";
const char mt_empregador[] = "MT_EMP.TXT";

const char* arq[] = { idp, nsr, mrp_ajusteRelogio, mrp_historicoEmpregador,
	mrp_historicoColaborador, mrp_marcacaoPonto,
	mt_colaborador, mt_empregador };

long ArquivosClass::leituraNumero(const char* arqv, bool incrementar){
	File arquivo = SD.open(arqv, FILE_READ);
	String nsrString = arquivo.readString();
	arquivo.close();

	if(incrementar == false)
		return nsrString.toInt();

	SD.remove((char*)arqv);

	long nsrNumber = nsrString.toInt() + 1;
	arquivo = SD.open(arqv, FILE_WRITE);
	arquivo.print(nsrNumber);
	arquivo.close();

	return nsrNumber;
}

long ArquivosClass::leituraNSR(bool incrementar = true)
{
	return leituraNumero(nsr, incrementar);
}

long ArquivosClass::leituraId(bool incrementar = true)
{
	return leituraNumero(idp, incrementar);
}


void ArquivosClass::init()
{
	for(int i = 0; i < QUANTIDADE_ARQUIVO; i++)
	{
		SD.remove((char*)arq[i]);
		File _thisFile = SD.open(arq[i]);
		if(arq[i] == "NSR.TXT" || arq[i] == "ID.TXT") {

			aJsonObject* root = aJson.createObject();
			aJson.addItemToObject(root, "Valor", 0);

			_thisFile.print(aJson.print(root));

			aJson.deleteItem(root);
		}
		_thisFile.close();
	}
}

void ArquivosClass::incluirColaborador(aJsonObject* colaborador) {
	File fpessoa = SD.open(mt_colaborador, FILE_WRITE);
	aJsonObject* id = aJson.getObjectItem(colaborador, "Id");
	id->valueint = leituraId();
	fpessoa.print(aJson.print(colaborador));
	fpessoa.print('$');
	fpessoa.close();
}

aJsonObject* ArquivosClass::leituraColaborador(int id) {
	File fpessoa = SD.open(mt_colaborador, FILE_READ);
	
	while(true){
		String pessoa = fpessoa.readStringUntil('$');

		if (pessoa.length() == 0)
			break;

		char* buffer = new char[pessoa.length()];
		pessoa.toCharArray(buffer, pessoa.length());
		if(buffer[0] == '!') continue;

		aJsonObject* m = aJson.parse(buffer);
		aJsonObject* idJson = aJson.getObjectItem(m, "Id");
		if (idJson->valueint == id){
			fpessoa.close();
			return m;
		}
	}

	fpessoa.close();

	return nullptr;
}

aJsonObject* ArquivosClass::leituraColaboradores(){
	File fpessoa = SD.open(mt_colaborador, FILE_READ);
	String pessoas = "[";

	while(true){
		
	}

	pessoas += "]";
}

void ArquivosClass::alterarColaborador(aJsonObject* colaborador) {
}



ArquivosClass Arquivos;

