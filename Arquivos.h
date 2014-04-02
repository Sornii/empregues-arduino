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

	String getHora();
	String getData();

	void gravarEmpregador(String, char);
	void salvarHistoricoColaborador(char*, char);

	void getNsr();
	void setNsr();
	void incrementNsr();

	void getId();
	void setId();
	void incrementId();
		
	uint32_t nsr;
	uint32_t id;

public:

	uint32_t Id();

	int init();
	String leituraEmpregador();
	void incluirEmpregador(String);
	void alterarEmpregador(String);
	
	void alterarColaborador(char*);
	void excluirColaborador(char*);
	bool proximoColaborador(File, char*);
	

	uint32_t incluirColaborador(char*);
	bool consultarColaborador(char*, uint32_t);
};

extern ArquivosClass Arquivos;

#endif