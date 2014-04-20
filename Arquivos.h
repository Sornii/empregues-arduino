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

	// Vari�vel respons�vel por guardar em mem�ria
	// o ID que est� sendo usado para atribuir novos
	// cadastros e ao mesmo tempo marcar o �ltimo
	// ID usado para n�o realizar leituras toda via
	// que precisa-lo
	uint32_t idColaborador;
	uint32_t idPonto;

	RTC_DS1307* _rtc;

	/**************************************************
		Helpers
	***************************************************/

	void addint(int, char*);

	/*************************************************
		Counters
	**************************************************/

	/* N�o iremos mais ter uso nisso por causa do tipo de mem�ria
	void getNsrFromFile();
	void setNsrInFile();
	void incrementNsr();
	*/

	// Realiza a leitura do ID diretamente do arquivo
	// gravado no SD para a mem�ria
	void getIdColaboradorFromFile();

	// Realiza a grava��o do ID em mem�ria para
	// o arquivo no SD
	void setIdColaboradorInFile();

	// Realiza um incremento no ID de mem�ria e ap�s
	// realiza a grava��o do mesmo no arquivo SD
	void incrementIdColaborador();

	void getIdPontoFromFile();
	void setIdPontoInFile();
	void incrementIdPonto();
		
	/*************************************************
		Others
	**************************************************/

	// Formata o M�s e o Ano do objeto DateTime
	// para um formato v�lido para usar na leitura
	// dos arquivos
	//
	// e.g. Se o DateTime for Ano: 2014, M�s: 04
	// o resultado ser� 2014/04
	//
	// \DateTime& Objeto do DateTime para ser usado
	// \char* Cadeia de caracteres que recebr� o resultado
	void getMesAno(DateTime&, char*);

	// Formata o M�s e o Ano usando a fun��o
	// getMesAno(DateTime&, char*); e logo em seguida
	// o formata diretamente para a pasta do ponto
	//
	// e.g. PONTO/2014/04
	//
	// \DateTime& DateTime para ser usado como tempo
	// \char* Cadeia de caracteres que receber� o resultado
	void pastaPonto(DateTime&, char*);

public: 
	
	void init(RTC_DS1307*);

	/**************************************************
		Getters and Setters
	***************************************************/

	// Retorna o ID registrado na mem�ria do sistema
	uint32_t getIdColaborador();
	uint32_t getIdPonto();
	
	/**************************************************
		Colaboradores
	***************************************************/

	// Realiza a inclus�o do colaborador na mem�ria
	//
	// \char* Par�metro respons�vel pela cadeia de
	// caracteres que representa o Colaborador para
	// que seja feita sua inser��o na mem�ria,
	// o formato deve ser exclusivamente da seguinte
	// forma: <PIS>;<NOME COMPLETO>
	//
	// e.g. 21552284859;Igor Henrique Pereira Fontana
	uint32_t incluirColaborador(char*);

	bool consultarColaborador(char*, uint32_t);

	// Marca o ponto conforme o ID recebido pela fun��o
	//
	// \uint32_t ID do colaborador respons�vel pelo ponto
	void marcarPonto(uint32_t);

	/************************************************** 
		Public Helper
	***************************************************/

	// L� todo o cont�udo do arquivo
	//
	// \char* Caminho do arquivo
	// \char* Resultado da leitura
	void getConteudo(char*, char*);

	// L� todo o cont�udo do arquivo
	//
	// \File* Arquivo para ser lido
	// \char* Resultado da leitura
	void getConteudo(File*, char*);
};

extern ArquivosClass Arquivos;

#endif