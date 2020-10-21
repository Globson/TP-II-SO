#ifndef P_CONTROLE_H
#define P_CONTROLE_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "Estruturas_de_Dados.h"
#define MAXTAM 100
#define BUFFER 256
int Executar_P_Controle();
void FilaVazia(Programa *prog);
int EstaVazia(Programa prog);
void AdicionaProgramaFila(Programa *programa, char *instrucao);
void RetiraProgramaFila(Programa *prog,char *instrucao);

#endif //P_CONTROLE_H
