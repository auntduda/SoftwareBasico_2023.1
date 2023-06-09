#ifndef FILES_H
#define FILES_H

#include<bits/stdc++.h>
#include "tables.h"

using namespace std;

typedef enum tipoArquivo {EXC, OBJ} tipoArquivo;

class arquivoSaida_t {
	public:
		tipoArquivo tipoA;
		vector<int> opcode;
		arquivoSaida_t() {}
		virtual void escreverNoArquivo(string arquivo) = 0;
};

class arquivoObjeto_t: public arquivoSaida_t {
	public:
		tuso_t tabelaUso;
		tdefinicao_t tabelaDefinicao;
		vector<int> enderecoRelativo;
		arquivoObjeto_t(): arquivoSaida_t() {}
		void escreverNoArquivo(string arquivo) override; 
};

class executavel_t: public arquivoSaida_t {
	public:
		executavel_t(): arquivoSaida_t(){};
		void escreverNoArquivo(string arquivo) override; 
};


#endif
