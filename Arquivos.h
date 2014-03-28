// ArquivosClass.h

#ifndef _ARQUIVOSCLASS_h
#define _ARQUIVOSCLASS_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "Arduino.h"
#else
	#include "WProgram.h"
#endif

#define QUANTIDADE_ARQUIVO	8

class ArquivosClass
{
private:
	

public:
	long leituraNumero(const char*, bool);
	long leituraNSR(bool);
	long leituraId(bool);


	void init();

	void incluirColaborador(aJsonObject*);
	void alterarColaborador(aJsonObject*);
	aJsonObject* ArquivosClass::leituraColaborador(int);
	aJsonObject* ArquivosClass::leituraColaboradores()
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

