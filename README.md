# Software Baśico - 2023.1

## Alunos

- Davi Jesus de Almeida Paturi - 200016784
- Maria Eduarda Carvalho Santos - 190092556

## Trabalho 1

O trabalho 1 da disciplina de Software Básico consiste em desenvolver o montador e o ligador do assembly inventado, apresentado em aula. Utilizando a linguagem C++ de programação.

## **Montador**
- Para compilar o montador basta executar o seguinte comando
```
g++ ./montador.cpp -o ./montador.o
```
- Para rodar o montador basta executar o comando abaixo
```
./montador.o arquivo1 arquivo2 [...]
```
- A saída será um arquivo `.obj` se houver as diretivas BEGIN e END no arquivo.
Caso contrário, a saída será um arquivo `.exc`.

### Observações

- Os erros gerados pelo montador são indexados pelas linhas respectivas ao arquivo pré-processado e não ao arquivo original.
- O mnemônico da instrução de multiplicação utilizado foi o `MUL`.

## **Ligador**

[aqui é onde a gente vai escrever depois o que fez nesse trabalho](https://github.com/auntduda)