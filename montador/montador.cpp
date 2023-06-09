#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <cstring>

#include "helpers.h"
#include "preprocessor.h"

using namespace std;

struct simbolo {
    int pos;
    bool defined;
    bool external;
    bool publico;
    pair<string, int> firstUsed;
    vector<int> listaPendencias;
};

map<string, simbolo> tabelaSimbolos;
map<string, int> tabelaDefinicoes;
vector<pair<string, int>> tabelaUso;
vector<int> listaRelativos;

bool salvarPP = false;

void montar (string fileName)
{
    // Pre-processar
    vector<vector<string>> tokens = tokenize(fileName + ".asm");
    if (salvarPP) {
        ofstream outputpp(fileName + "_pp.asm");
        for (int i = 0; i < tokens.size(); i++) {
            for (int j = 0; j < tokens[i].size(); j++) {
                outputpp << tokens[i][j] << " ";
            }
            outputpp << endl;
        }
        outputpp.close();
    }
    
    // Passagem
    vector<int> code;
    bool modulo = false;
    bool begin = false;

    int currSection = SECTION_TEXT;
    int currLine = 1;
    int currPos = 0;
    for (unsigned int i = 0; i < tokens.size(); i++, currLine++) {
        int j = 0;
        string token = tokens[i][j];

        if (isLabel(token)) {
            token.pop_back(); // Remover : do final da string
            if (token == "EXTERN") {
                // EXTERN
                if (!begin) { cerr << ERR_SEM(currLine) << "EXTERN utilizado fora de um BEGIN/END" << endl; }

                token = nextToken(tokens[i], ++j);
                if (token.empty()) {
                    cerr << ERR_SIN(currLine) << "EXTERN sem parametro" << endl;
                } else if (tabelaSimbolos.count(token)) {
                    cerr << ERR_SEM(currLine) << "Label externa '" << token << "' ja foi definida dentro do programa." << endl;
                } else {
                    simbolo symb;
                    symb.pos = 0;
                    symb.defined = false;
                    symb.external = true;
                    symb.publico = false;
                    symb.firstUsed = {"EXTERN", currLine};
                    tabelaSimbolos[token] = symb;
                }
                continue;
            } else {
                // LABEL
                if (!isValidLabel(token)) {
                    cerr << ERR_LEX(currLine) << "Label '" << token << "' contendo caracteres ilegais" << endl;
                }

                if (tabelaSimbolos.count(token)) {
                    if (tabelaSimbolos[token].defined) {
                        cerr << ERR_SEM(currLine) << "Label '" << token << "' ja foi definida." << endl;
                    }

                    if (tabelaSimbolos[token].external) {
                        cerr << ERR_SEM(currLine) << "Label '" << token << "' ja foi definida como externa dentro do programa. " << endl;
                    } else {
                        tabelaSimbolos[token].pos = currPos;
                        tabelaSimbolos[token].defined = true;
                    }

                    // Resolve lista de pendencias
                    if (tabelaSimbolos[token].listaPendencias.size()) {
                        for (unsigned int p = 0; p < tabelaSimbolos[token].listaPendencias.size(); p++) {
                            code[tabelaSimbolos[token].listaPendencias[p]] += currPos;
                        }
                    }
                } else {
                    simbolo symb;
                    symb.pos = currPos;
                    symb.defined = true;
                    symb.external = false;
                    symb.publico = false;
                    tabelaSimbolos[token] = symb;
                }

                token = nextToken(tokens[i], ++j);
                if (isLabel(token)) {
                    cerr << ERR_SIN(currLine) << "Dois rotulos na mesma linha" << endl;
                    token = tokens[i][++j];
                }
            }
        }

        if (token.empty()) {
            continue;
        } else if (token == "SECTION") {
            token = nextToken(tokens[i], ++j);
            if (token == "TEXT") { currSection = SECTION_TEXT; }
            else if (token == "DATA") { currSection = SECTION_DATA; }
        } else if (token == "BEGIN") {
            // BEGIN
            begin = true;
            modulo = true;
        } else if (token == "END") {
            // END
            begin = false;
        } else if (token == "PUBLIC") {
            // PUBLIC
            token = nextToken(tokens[i], ++j);
            if (token.empty()) {
                cerr << ERR_SIN(currLine) << "PUBLIC sem parametro" << endl;
            } else {
                if (tabelaSimbolos.count(token)) {
                    tabelaSimbolos[token].publico = true;
                } else {
                    simbolo symb;
                    symb.pos = 0;
                    symb.defined = false;
                    symb.external = false;
                    symb.publico = true;
                    symb.firstUsed = {"PUBLIC", currLine};
                    tabelaSimbolos[token] = symb;
                }
            }
        } else if (opcode(token)) {
            // INSTRUCOES
            if (currSection != SECTION_TEXT) {
                cerr << ERR_SEM(currLine) << "Instrucao '" << token << "' sendo utilizada fora da SECTION TEXT" << endl;
            }

            int op = opcode(token);
            code.push_back(op);
            currPos++;

            if (token == "COPY") {
                string param = nextToken(tokens[i], ++j);
                if (param.empty()) {
                    cerr << ERR_SIN(currLine) << "Instrucao 'COPY' sem primeiro parametro" << endl;
                    code.push_back(0);
                    currPos++;
                } else {
                    int posToAdd = 0;
                    if (tabelaSimbolos.count(param)) {
                        if (tabelaSimbolos[param].defined) {
                            posToAdd = tabelaSimbolos[param].pos;
                        } else {
                            tabelaSimbolos[param].listaPendencias.push_back(currPos);
                        }
                    } else {
                        simbolo symb;
                        symb.pos = 0;
                        symb.defined = false;
                        symb.external = false;
                        symb.publico = false;
                        symb.firstUsed = {token, currLine};
                        symb.listaPendencias.push_back(currPos);
                        tabelaSimbolos[param] = symb;
                    }

                    token = nextToken(tokens[i], ++j);
                    if (token == "+") {
                        token = nextToken(tokens[i], ++j);
                        if (token.empty() || token == ",") {
                            cerr << ERR_SIN(currLine) << "Valor do lado direito da soma faltando" << endl;
                        } else {
                            int value = parseInt(token, currLine);
                            posToAdd += value;

                            token = nextToken(tokens[i], ++j);
                        }
                    }

                    listaRelativos.push_back(currPos);
                    code.push_back(posToAdd);
                    currPos++;

                    if (token == ",") {
                        param = nextToken(tokens[i], ++j);
                        if (param.empty()) {
                            cerr << ERR_SIN(currLine) << "Instrucao 'COPY' sem segundo parametro" << endl;
                            posToAdd = 0;
                        }
                        if (tabelaSimbolos.count(param)) {
                            if (tabelaSimbolos[param].defined) {
                                posToAdd = tabelaSimbolos[param].pos;
                            } else {
                                tabelaSimbolos[param].listaPendencias.push_back(currPos);
                            }
                        } else {
                            simbolo symb;
                            symb.pos = 0;
                            symb.defined = false;
                            symb.external = false;
                            symb.publico = false;
                            symb.firstUsed = {"COPY", currLine};
                            symb.listaPendencias.push_back(currPos);
                            tabelaSimbolos[param] = symb;
                        }

                        token = nextToken(tokens[i], j + 1);
                        if (token == "+") {
                            j += 2;
                            token = nextToken(tokens[i], j);
                            if (token.empty()) {
                                cerr << ERR_SIN(currLine) << "Valor do lado direito da soma faltando" << endl;
                            } else {
                                int value = parseInt(token, currLine);
                                posToAdd += value;
                            }
                        }
                    } else {
                        cerr << ERR_SIN(currLine) << "Instrucao 'COPY' faltando virgula" << endl;
                    }

                    listaRelativos.push_back(currPos);
                    code.push_back(posToAdd);
                    currPos++;
                }
            } else if (token != "STOP") {
                string param = nextToken(tokens[i], ++j);
                if (param.empty()) {
                    cerr << ERR_SIN(currLine) << "Instrucao '" << token << "' sem parametro" << endl;
                    code.push_back(0);
                    currPos++;
                } else {
                    int posToAdd = 0;
                    if (tabelaSimbolos.count(param)) {
                        if (tabelaSimbolos[param].defined) {
                            posToAdd = tabelaSimbolos[param].pos;
                        } else {
                            tabelaSimbolos[param].listaPendencias.push_back(currPos);
                        }
                    } else {
                        simbolo symb;
                        symb.pos = 0;
                        symb.defined = false;
                        symb.external = false;
                        symb.publico = false;
                        symb.firstUsed = {token, currLine};
                        symb.listaPendencias.push_back(currPos);
                        tabelaSimbolos[param] = symb;
                    }

                    token = nextToken(tokens[i], j + 1);
                    if (token == "+") {
                        j += 2;
                        token = nextToken(tokens[i], j);
                        if (token.empty()) {
                            cerr << ERR_SIN(currLine) << "Valor do lado direito da soma faltando" << endl;
                        } else {
                            int value = parseInt(token, currLine);
                            posToAdd += value;
                        }
                    }

                    listaRelativos.push_back(currPos);
                    code.push_back(posToAdd);
                    currPos++;
                }
            }
        } else if (token == "SPACE") {
            // SPACE
            if (currSection != SECTION_DATA) {
                cerr << ERR_SEM(currLine) << "Diretiva '" << token << "' sendo utilizada fora da SECTION DATA" << endl;
            }

            string param = nextToken(tokens[i], ++j);
            if (param.empty()) {
                code.push_back(0);
                currPos++;
            } else {
                int spaces = parseInt(param, currLine);
                for (int x = 0; x < spaces; x++) {
                    code.push_back(0);
                    currPos++;
                }
            }
        } else if (token == "CONST") {
            // CONST
            if (currSection != SECTION_DATA) {
                cerr << ERR_SEM(currLine) << "Diretiva '" << token << "' sendo utilizada fora da SECTION DATA" << endl;
            }

            string param = nextToken(tokens[i], ++j);
            if (param.empty()) {
                cerr << ERR_SIN(currLine) << "Diretiva CONST usada sem parametro" << endl;
                code.push_back(0);
                currPos++;
            } else {
                int value = parseInt(param, currLine);
                code.push_back(value);
                currPos++;
            }
        } else {
            cerr << ERR_SIN(currLine) << "Token " << token << " invalido" << endl;
        }
    }

    // Checar simbolos nao definidos
    for (const auto& kv : tabelaSimbolos) {
        if (!kv.second.defined) {
            // labels utilizadas nao definidas (que nao sao externas)
            if(!kv.second.external && kv.second.listaPendencias.size()) {
                cerr << ERR_SEM(kv.second.firstUsed.second) << "LABEL " << kv.first << " nao foi definida";
                if (inst_section.count(kv.second.firstUsed.first)) {
                    string sectionMiss = inst_section[kv.second.firstUsed.first];
                    cerr << " na SECTION " << sectionMiss;
                }
                cerr << endl;
            }

            // labels declaradas como PUBLIC nao definidas
            if (kv.second.publico) {
                cerr << ERR_SEM(kv.second.firstUsed.second) << "LABEL " << kv.first << " foi declarada como PUBLIC nao foi definida" << endl;
            }
        }

        // Gera tabela de uso
        if (kv.second.external && kv.second.listaPendencias.size()) {
            for (unsigned int i = 0; i < kv.second.listaPendencias.size(); i++) {
                tabelaUso.push_back({ kv.first, kv.second.listaPendencias[i] });
            }
        }

        // Gera tabela de definicoes
        if (kv.second.publico) {
            tabelaDefinicoes[kv.first] = kv.second.pos;
        }

        // cout << kv.first << ": " << kv.second.pos << endl;
    }
    
    if (modulo) {
        // Saida .obj
        ofstream output(fileName + ".obj");

        // Tabela de uso
        output << "USO" << endl;
        for (auto uso: tabelaUso) {
            output << uso.first << " " << uso.second << endl;
        }

        // Tabela de definicoes
        output << "DEF" << endl;
        for (const auto& kv : tabelaDefinicoes) {
            output << kv.first << " " << kv.second << endl;
        }

        // Enderecos relativos
        output << "RELATIVOS" << endl;
        for (auto rel: listaRelativos) {
            output << rel << " ";
        }
        output << endl;

        // Codigo maquina
        output << "CODE" << endl;
        for (auto b: code) {
            output << b << " ";
        }
        output.close();

        cout << "Arquivo " << fileName << ".obj gerado" << endl;
    } else {
        // Saida .exc
        ofstream output(fileName + ".exc");
        for (auto b: code) {
            output << b << " ";
        }
        output.close();

        cout << "Arquivo " << fileName << ".exc gerado" << endl;
    }
}

int main (int argc, char *argv[])
{
    if (argc <= 1) {
        cout << "Forneca um ou mais nomes de arquivos para serem montados (sem extensao)" << endl;
        cout << "Se o arquivo for um modulo (existir BEGIN e END), a saida sera um arquivo .obj" << endl;
        cout << "Caso contrario, a saida sera um arquivo .exc" << endl;
        return 0;
    }

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-pp") == 0) {
            salvarPP = true;
            continue;
        }

        tabelaSimbolos.clear();
        tabelaDefinicoes.clear();
        tabelaUso.clear();
        listaRelativos.clear();

        cout << "--- Montagem arquivo: " << argv[i] << ".asm ---" << endl;
        montar(argv[i]);
    }

    return 0;
}
