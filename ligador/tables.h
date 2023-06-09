#ifndef TABLES_H
#define TABLES_H
#include <bits/stdc++.h>

using namespace std;

typedef struct simbolos_tipo {
	int endereco;
	bool isDefinido;
	bool isExterno;
	vector<int> dependencias;
} simbolos_tipo;

typedef map<string, simbolos_tipo> tsimbolos_t;

typedef map<string, int> tdefinicao_t;

typedef map<string,vector<int>> tuso_t;

simbolos_tipo criarTabelaSimbolos(int endereco, bool isDefinido, bool isExterno);

#endif
