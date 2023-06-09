#include<bits/stdc++.h>
#include "files.h"
#include "tables.h"

using namespace std;

arquivoObjeto_t lerObjeto(string srtingArq) {
	arquivoObjeto_t arquivoObjeto;

	const char *arquivo = srtingArq.c_str();
	FILE *file = fopen(arquivo, "r");

	if (!file)
	{
		printf("O arquivo <%s> não existe.\n", arquivo);
		throw;
	}
	
	fseek(file, 0L, SEEK_END);
	size_t tamanhoArquivo = ftell(file);
	rewind(file);

	char orig[tamanhoArquivo + 1], *aux, *linha, *token;
	size_t bytes_read = fread(orig, sizeof(char), tamanhoArquivo, file);

	if (bytes_read != tamanhoArquivo)
	{
		printf("Erro na leitura do arquivo: <%s>.\n", arquivo);
		fclose(file);
		throw;
	}

	fclose(file);
	orig[tamanhoArquivo] = '\0';

	aux = orig;
	int flag = -1;

	while ((linha = strtok_r(aux, "\n", &aux)))
	{
		string id(linha); 
		if(id == "USO")
		{
			flag = 0;
		} else if(id == "DEF")
		{
			flag = 1;
		} else if(id == "RELATIVOS")
		{
			flag = 2;
		} else if(id == "CODE")
		{
			flag = 3;
		} else
		{
			cout << "Formato .obj incorreto.\n";
		}
		
		if (flag == 0)
		{
			tuso_t *uses = &arquivoObjeto.tabelaUso;
			char *key = strtok_r(linha, " ", &linha);
			auto it = uses->find(key);

			if (it == uses->end())
			{
				(*uses)[key] = vector<int>();
			}

			while ((token = strtok_r(linha, " ", &linha)))
				(*uses)[key].push_back(atoi(token));
		} else if (flag == 1)
		{
			char *key = strtok_r(linha, " ", &linha);
			char *value = strtok_r(linha, " ", &linha);
			arquivoObjeto.tabelaDefinicao[key] = atoi(value);
		} else if (flag == 2)
		{
			while ((token = strtok_r(linha, " ", &linha)))
				arquivoObjeto.enderecoRelativo.push_back(atoi(token));
		} else if (flag == 3)
		{
			while ((token = strtok_r(linha, " ", &linha)))
				arquivoObjeto.opcode.push_back(atoi(token));
		}else
		{
			cout << "Formato .obj incorreto.\n";
		}
	}
	
	return arquivoObjeto;
}

int main(int argc, char **argv)
{
	if (argc == 1)
	{
		printf("Insira na linha de comando os arquivos .obj que deseja ligar.\n");
		return 0;
	}

	tdefinicao_t TGD; 												//instancia da tabela geral de definicao
	tuso_t TGU; 													//instancia da tabela geral de uso
	map<int, int> endRelativos;
	executavel_t codigoExecutavel;

	int fatorCorrecao = 0;

	for (int i = 1; i < argc; i++)
	{
		string arquivo(argv[i]);
		string arquivoObj = arquivo + ".obj";
		arquivoObjeto_t objeto = lerObjeto(arquivoObj);

		for (const auto &elem : objeto.tabelaDefinicao)
			TGD[elem.first] = elem.second + fatorCorrecao;

		for (const auto &elem : objeto.tabelaUso)
		{
			string id = elem.first;

			auto it = TGU.find(id);

			if (it == TGU.end())
			{
				TGU[id] = vector<int>();
			}

			for (const int &endereco : elem.second)
				TGU[id].push_back(endereco + fatorCorrecao);
		}

		for (const int &op : objeto.opcode)
			codigoExecutavel.opcode.push_back(op);

		for (const int &endereco : objeto.enderecoRelativo)
			endRelativos[endereco + fatorCorrecao] = fatorCorrecao;

		fatorCorrecao += objeto.opcode.size();
	}
	
	for (const auto &elem : TGU)
	{
		int enderecoLabel = TGD[elem.first];

		for (const int &elemU : elem.second)
			endRelativos[elemU] = enderecoLabel;
	}

	for (const auto &elem : endRelativos)
		codigoExecutavel.opcode[elem.first] += elem.second;

	string filename(argv[1]);
	string executavel = filename + ".exc";
	codigoExecutavel.escreverNoArquivo(executavel);

	printf("Arquivos ligados disponiveis em %s\n", executavel.c_str());

	return 0;
}
