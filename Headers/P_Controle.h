#ifndef P_CONTROLE_H
#define P_CONTROLE_H
#include "Estruturas_de_Dados.h"

int Executar_P_Controle();
void FFilaVazia(Programa *prog);
int EstaVazia(Programa prog);
void AdicionaProgramaFila(Programa *programa, char *instrucao);
void RetiraProgramaFila(Programa *prog,char *instrucao);
#endif //P_CONTROLE_H
