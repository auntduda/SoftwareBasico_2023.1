#include <bits/stdc++.h>

#define MAX_SIZE numeric_limits<streamsize>::max()

using namespace std;

string getToken (istringstream& line) {
    ostringstream tokenStream;
    char nextChar;
    while ((nextChar = line.get()) != EOF) {
        // Parar em caracteres vazios
        if (
            nextChar == '\t' ||
            nextChar == '\n' ||
            nextChar == '\r' ||
            nextChar == ' '
        ) {
            if (tokenStream.tellp()) { break; }
            else { continue; }
        }

        // Parar em comentários
        if (nextChar == ';') {
            line.ignore(MAX_SIZE, EOF);
            break;
        }

        tokenStream << nextChar;
    }
    string str = tokenStream.str();
    toUpperCase(str);
    return str;
}

vector<vector<string>> tokenize(string fileName) {
    ifstream file(fileName);
    if (file.fail()) {
        cerr << "Falha ao ler arquivo " << fileName << endl;
        exit(1);
    }

    vector<vector<string>> textTokens;
    vector<vector<string>> dataTokens;
    int currentSection = SECTION_TEXT;
    bool sectionText = false;
    int currLine = 1;

    string lineString;
    vector<string> line;
    while (getline(file, lineString)) {
        istringstream lineStream(lineString);
        for (string token = getToken(lineStream); !token.empty(); token = getToken(lineStream)) {
            // Mudar as sections
            if (token == "SECTION") {
                string sectionName = getToken(lineStream);
                if (!sectionName.empty()) {
                    if (sectionName == "TEXT") { sectionText = true; currentSection = SECTION_TEXT; }
                    else if (sectionName == "DATA") { currentSection = SECTION_DATA; }
                    else { cerr << ERR_SIN(currLine) << "SECTION com nome invalida" << endl; }
                } else {
                    cerr << ERR_SIN(currLine) << "SECTION sem nome" << endl;
                }

                line.push_back(token);
                line.push_back(sectionName);
            } else {
                if (token == "BEGIN") { currentSection = SECTION_TEXT; }
                line.push_back(token);
                // cout << token << endl;
            }
        }

        if (line.size()) {
            if (!labelOnly(line)) {
                if (currentSection == SECTION_TEXT) { textTokens.push_back(line); }
                else if (currentSection == SECTION_DATA) { dataTokens.push_back(line); }
                else {
                    cerr << ERR_SEM(currLine) << "Codigo fora de uma section valida!" << endl;
                }
                line.clear();
            }
        }
        currLine++;
    }
    file.close();

    // Checa se existe SECTION TEXT
    if (!sectionText) {
        cerr << ERR_SEM(currLine) << "SECTION TEXT faltando." << endl;
    }

    // Retorna a concatenacao do text e data
    textTokens.insert(textTokens.end(), dataTokens.begin(), dataTokens.end());
    return textTokens;
}
