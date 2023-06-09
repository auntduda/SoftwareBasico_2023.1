#include <sstream>

#include "files.h"

using namespace std;

void arquivoObjeto_t::escreverNoArquivo(string arquivo) {
  stringstream ans;
  
  ans << "USO\n";
  for (const auto &use : tabelaUso)
  {
      ans << use.first << " ";
      for(const auto &elem : use.second) // tem um erro aqui
        ans << elem << " ";
      ans << '\n';
  }

  ans << "DEF\n";
  for (const auto &def : tabelaDefinicao)
      ans << def.first << " " << def.second << '\n';

  ans << "RELATIVOS\n";
  for(const auto &elem : enderecoRelativo)
    ans << elem << " ";
  ans << '\n';

  ans << "CODE\n";
  for(const auto &elem : opcode)
    ans << elem << " ";
  ans << '\n';
  
  FILE *arq;
  arq = fopen(arquivo.c_str(), "w+");
  fputs(ans.str().c_str(), arq);
  fclose(arq);
} 

void executavel_t::escreverNoArquivo(string arquivo) {
  FILE *arq;
  arq = fopen(arquivo.c_str(), "w+");
  for (const int &byte : opcode)
    fprintf(arq, "%d ", byte);
  fclose(arq);
}
