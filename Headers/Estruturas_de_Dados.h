#ifndef ESTRUTURAD_DE_DADOS_H
#define ESTRUTURAD_DE_DADOS_H
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <sys/types.h>
#define MAXTAM 100
#define BUFFER 256

typedef char Instrucao[20]; //Armazena uma instrução

typedef struct Programa{
  Instrucao instrucoes[20]; //Armazena instruções de um programa
  int Tam; //Tamanho do programa;
  int Frente,Tras; //Fila de programas
}Programa;

typedef struct EstadoProcesso{
  int Inteiro;
  int Cont;
  int Tam;
  Instrucao Programa[20];
}EstadoProcesso;

#endif //ESTRUTURAD_DE_DADOS_H
