# Software Básico - 2023.1

# Alunos

- Davi Jesus de Almeida Paturi - 200016784
- Maria Eduarda Carvalho Santos - 190092556

# Trabalho 1

O trabalho 1 da disciplina de Software Básico consiste em desenvolver o montador e o ligador do assembly inventado, apresentado em aula. Utilizando a linguagem C++ de programação.

## Versão do Compilador

A versão utilizada para elaboração do trabalho foi dada pelo compilador GNU G++17 7.3.0 (C++17).

## SO de Desenvolvimento

O projeto foi desenvolvido em dois diferentes sistemas operacionais:

- Ubuntu 22.04.2 LTS
- Windows 10

O projeto foi executado em ambos os SOs, e não houve erro de execução.

## **Montador**
- Para compilar o montador, basta executar o seguinte comando
```
g++ ./montador.cpp -o ./montador.o
```
- Em seguida, basta executar o comando abaixo
```
./montador.o arquivo1 arquivo2 [...]
```
- A saída será um arquivo `.obj` se houver as diretivas BEGIN e END no arquivo.
Caso contrário, a saída será um arquivo `.exc`.

### Observações

- Os erros gerados pelo montador são indexados pelas linhas respectivas ao arquivo pré-processado e não ao arquivo original.
- O mnemônico da instrução de multiplicação utilizado foi o `MUL`.
- Para gerar os arquivos pré-processados, basta adicionar a flag `-pp` na chamada do programa. Exemplo: `./montador.o -pp arquivo1 arquivo2 [...]`. Os arquivos gerados terão o nome do arquivo `.asm` original com `_pp` adicionado ao final (Ex.: `arquivo1_pp.asm`).

## **Ligador**

Para compilar o ligador, basta executar a seguinte linhas de comando:

```
g++ LINKER.cpp -o l
```

Em seguida, basta executar a seguinte linha de comando, para executar as atividades do ligador, caso deseje ligar os arquivos `MOD_A.obj` e `MOD_B.obj`

```
./l MOD_A MOD_B
```

Caso deseje ligar mais `arquivos.obj`, é necessário chamá-los na linha de execução do ligador.

### Disponibilização do Trabalho

Caso a entrega corrompa os arquivos descritos acima, acesse [GitHub/Software Básico - 2023.1](https://github.com/auntduda/SoftwareBasico_2023.1) para verificar a funcionalidade do projeto.