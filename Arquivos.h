// Arquivos.h

#ifndef _ARQUIVOS_h
#define _ARQUIVOS_h

#include "Arduino.h"
#include "SD.h"
#include "RTClib.h"

#define QUANTIDADE_ARQUIVO 8

#define INCLUSAO '1'
#define ALTERACAO '2'
#define EXCLUSAO '3'

class ArquivosClass
{
private:

	uint32_t nsr;
	uint32_t id;

	RTC_DS1307* _rtc;

	/*
		Helpers
	*/

	void addint(int, char*);
	void getConteudo(char*, char*);

	/*
		Counters
	*/

	void getNsrFromFile();
	void setNsrInFile();
	void incrementNsr();

	void getIdFromFile();
	void setIdInFile();
	void incrementId();
		
	/*
		Others
	*/

	void getMesAno(DateTime&, char*);
	void pastaPonto(DateTime&);
	void pastaPonto(DateTime&, char*);

public:

	uint32_t getId();

	void init(RTC_DS1307*);
	
	/*
		Colaboradores
	*/

	uint32_t incluirColaborador(char*);
	bool consultarColaborador(char*, uint32_t);

	void marcarPonto(uint32_t);
};

extern ArquivosClass Arquivos;

#endif