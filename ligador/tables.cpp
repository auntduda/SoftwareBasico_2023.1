#include<bits/stdc++.h>
#include "tables.h"

using namespace std;

simbolos_tipo criarTabelaSimbolos(int endereco, bool isDefinido, bool isExterno){
	simbolos_tipo simbolo;
	vector<int> dependencias;
	simbolo.endereco = endereco;
	simbolo.isDefinido = isDefinido;
	simbolo.isExterno = isExterno;
	simbolo.dependencias = dependencias;
	return simbolo;
}
