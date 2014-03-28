// Arquivos.h

#ifndef _ARQUIVOS_h
#define _ARQUIVOS_h

#include "Arduino.h"
#include "SD.h"

#define QUANTIDADE_ARQUIVO 8

#define INCLUSAO '1'
#define ALTERACAO '2'
#define EXCLUSAO '3'

class ArquivosClass
{
private:
	void initArray(char*);
	void initArray(char*, uint16_t);
	long leituraNumero(const char*, bool);
	long leituraNSR(bool);
	long leituraId(bool);
	String getHora();
	String getData();
	void gravarEmpregador(String, char);
	void salvarHistoricoColaborador(char*, char);
public:
	int init();
	String leituraEmpregador();
	void incluirEmpregador(String);
	void alterarEmpregador(String);
	long incluirColaborador(char*);
	void alterarColaborador(char*);
	void excluirColaborador(char*);
	bool proximoColaborador(File, char*);
	bool consultarColaborador(char*, int);
};

extern ArquivosClass Arquivos;

extern const char idp[];
extern const char nsr[];
extern const char mrp_ajusteRelogio[];
extern const char mrp_historicoEmpregador[];
extern const char mrp_historicoColaborador[];
extern const char mrp_marcacaoPonto[];
extern const char mt_colaborador[];
extern const char mt_empregador[];

#endif