// Arquivos.h

#ifndef _ARQUIVOS_h
#define _ARQUIVOS_h

#include "Arduino.h"
#include "SD.h"
#include "RTClib.h"

class ArquivosClass
{
private: 

	//uint32_t nsr;

	// Variável responsável por guardar em memória
	// o ID que está sendo usado para atribuir novos
	// cadastros e ao mesmo tempo marcar o último
	// ID usado para não realizar leituras toda via
	// que precisa-lo
	uint32_t id;

	RTC_DS1307* _rtc;

	/**************************************************
		Helpers
	***************************************************/

	void addint(int, char*);

	/*************************************************
		Counters
	**************************************************/

	/* Não iremos mais ter uso nisso por causa do tipo de memória
	void getNsrFromFile();
	void setNsrInFile();
	void incrementNsr();
	*/

	// Realiza a leitura do ID diretamente do arquivo
	// gravado no SD para a memória
	void getIdFromFile();

	// Realiza a gravação do ID em memória para
	// o arquivo no SD
	void setIdInFile();

	// Realiza um incremento no ID de memória e após
	// realiza a gravação do mesmo no arquivo SD
	void incrementId();
		
	/*************************************************
		Others
	**************************************************/

	// Formata o Mês e o Ano do objeto DateTime
	// para um formato válido para usar na leitura
	// dos arquivos
	//
	// e.g. Se o DateTime for Ano: 2014, Mês: 04
	// o resultado será 2014/04
	//
	// \DateTime& Objeto do DateTime para ser usado
	// \char* Cadeia de caracteres que recebrá o resultado
	void getMesAno(DateTime&, char*);

	// Formata o Mês e o Ano usando a função
	// getMesAno(DateTime&, char*); e logo em seguida
	// o formata diretamente para a pasta do ponto
	//
	// e.g. PONTO/2014/04
	//
	// \DateTime& DateTime para ser usado como tempo
	// \char* Cadeia de caracteres que receberá o resultado
	void pastaPonto(DateTime&, char*);

public: 
	
	void init(RTC_DS1307*);

	/**************************************************
		Getters and Setters
	***************************************************/

	// Retorna o ID registrado na memória do sistema
	uint32_t getId();
	
	/**************************************************
		Colaboradores
	***************************************************/

	// Realiza a inclusão do colaborador na memória
	//
	// \char* Parâmetro responsável pela cadeia de
	// caracteres que representa o Colaborador para
	// que seja feita sua inserção na memória,
	// o formato deve ser exclusivamente da seguinte
	// forma: <PIS>;<NOME COMPLETO>
	//
	// e.g. 21552284859;Igor Henrique Pereira Fontana
	uint32_t incluirColaborador(char*);

	bool consultarColaborador(char*, uint32_t);

	// Marca o ponto conforme o ID recebido pela função
	//
	// \uint32_t ID do colaborador responsável pelo ponto
	void marcarPonto(uint32_t);

	/************************************************** 
		Public Helper
	***************************************************/

	// Lê todo o contéudo do arquivo
	//
	// \char* Caminho do arquivo
	// \char* Resultado da leitura
	void getConteudo(char*, char*);

	// Lê todo o contéudo do arquivo
	//
	// \File* Arquivo para ser lido
	// \char* Resultado da leitura
	void getConteudo(File*, char*);
};

extern ArquivosClass Arquivos;

#endif