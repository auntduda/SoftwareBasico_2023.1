#include <map>
#include <algorithm>

#define ERR_LEX(l) "ERRO LEXICO L" << l << ": "
#define ERR_SIN(l) "ERRO SINTATICO L" << l << ": "
#define ERR_SEM(l) "ERRO SEMANTICO L" << l << ": "

#define SECTION_TEXT 1
#define SECTION_DATA 2

using namespace std;

map<string,int> opcodes = {
    {"ADD", 1},
    {"SUB", 2},
    {"MUL", 3},
    {"DIV", 4},
    {"JMP", 5},
    {"JMPN", 6},
    {"JMPP", 7},
    {"JMPZ", 8},
    {"COPY", 9},
    {"LOAD", 10},
    {"STORE", 11},
    {"INPUT", 12},
    {"OUTPUT", 13},
    {"STOP", 14}
};

map<string,string> inst_section = {
    {"ADD", "DATA"},
    {"SUB", "DATA"},
    {"MUL", "DATA"},
    {"DIV", "DATA"},
    {"JMP", "TEXT"},
    {"JMPN", "TEXT"},
    {"JMPP", "TEXT"},
    {"JMPZ", "TEXT"},
    {"COPY", "DATA"},
    {"LOAD", "DATA"},
    {"STORE", "DATA"},
    {"INPUT", "DATA"},
    {"OUTPUT", "DATA"}
};

// Transforma uma string em maiusculo
void toUpperCase(string& str) {
    transform(str.begin(), str.end(), str.begin(), ::toupper);
}

// Retorna se um token eh uma label (termina com :)
bool isLabel (string token) {
    return token[token.size() - 1] == ':';
}

// Retorna se uma linha contem apenas uma label
bool labelOnly (vector<string> line) {
    return line.size() == 1 && line[0].back() == ':';
}

// Retorna verdadeiro se houver apenas caracteres alfanumericos e underscore em uma string
bool isword (string token) {
    for (char c: token) {
        if (!isalnum(c) && c != '_') { return false; }
    }
    return true;
}

// Retorna se uma label eh valida
bool isValidLabel (string token) {
    return (isalpha(token[0]) || token[0] == '_') && isword(token);
}

// Retorna o opcode de uma instrucao
int opcode (string token) {
    return opcodes[token];
}

// Retorna o proximo token (se houver), caso contrario, retorna uma string vazia
string nextToken (vector<string> tokens, int i) {
    if (i < tokens.size()) { return tokens[i]; } else { return ""; }
}

// Faz o parse de uma string
int parseInt (string token, int currLine) {
    int value = 0;
    try {
        value = stoi(token, nullptr, 0);
    } catch (invalid_argument& ia) {
        cerr << ERR_LEX(currLine) << "Valor numerico invalido" << endl;
    }
    return value;
}
