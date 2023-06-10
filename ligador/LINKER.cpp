#include <bits/stdc++.h>

using namespace std;

#define USO 1
#define DEF 2
#define REL 3
#define CODE 4

#define nextToken getline(lineStream, token, ' ')
#define intToken stoi(token, nullptr, 10)

struct modulo {
  map<string, vector<int>> tabelaUso;
  map<string, int> tabelaDefinicao;
  vector<int> relativos;
  vector<int> code;
  
};

modulo parseFile (string fileName)
{
  modulo mod;

  ifstream file(fileName + ".obj");
  
  // FLAG modo
  int modo = 0;

  string line;
  while (getline(file, line)) {
    stringstream lineStream(line);
    string token;
    while (nextToken) {
      if (token.empty()) { continue; }
      // Troca de modos
      if (token == "USO") { modo = USO; }
      else if (token == "DEF") { modo = DEF; }
      else if (token == "RELATIVOS") { modo = REL; }
      else if (token == "CODE") { modo = CODE; }
      
      // Modos
      else if (modo == USO) {
        string label = token;
        vector<int> usos;
        while (nextToken) {
          usos.push_back(intToken);
        }
        mod.tabelaUso[label] = usos;
      } else if (modo == DEF) {
        string label = token;
        nextToken;
        mod.tabelaDefinicao[label] = intToken;
      } else if (modo == REL) {
        vector<int> relativos = {intToken};
        while (nextToken) {
          relativos.push_back(intToken);
        }
        mod.relativos = relativos;
      } else if (modo == CODE) {
        vector<int> code = {intToken};
        while (nextToken) {
          code.push_back(intToken);
        }
        mod.code = code;
      }
    }
  }

  file.close();

  return mod;
}

int main (int argc, char *argv[])
{
  map<string, int> TGD;
  map<string, vector<int>> TGU;
  map<int, int> relativos;
  vector<int> exec;

  int fatorCorrecao = 0;
  for (int i = 1; i < argc; i++) {
    string fileName = argv[i];
    modulo mod = parseFile(fileName);

    for (auto kv: mod.tabelaDefinicao) {
      TGD[kv.first] = kv.second + fatorCorrecao;
    }

    for (auto kv: mod.tabelaUso) {
      if (!TGU.count(kv.first)) { TGU[kv.first] = vector<int>(); }
      for (auto end: kv.second) {
        TGU[kv.first].push_back(end + fatorCorrecao);
      }
    }

    for (int rel: mod.relativos) {
      relativos[rel + fatorCorrecao] = fatorCorrecao;
    }

    for (int b: mod.code) {
      exec.push_back(b);
    }

    fatorCorrecao += mod.code.size();
  }

  for (auto uso: TGU) {
    int labelAddr = TGD[uso.first];
    for (auto end: uso.second) {
      relativos[end] = labelAddr;
    }
  }

  for (auto rel: relativos) {
    exec[rel.first] += rel.second;
  }

  string outputFile(argv[1]);
  outputFile = outputFile + ".exc";
  ofstream output(outputFile);
  for (auto str: exec) {
    output << str << " ";
  }
  output.close();

  cout << "Arquivos ligados disponiveis em " << outputFile << endl;
  return 0;
}
