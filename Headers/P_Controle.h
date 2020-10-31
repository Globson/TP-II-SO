#ifndef P_CONTROLE_H
#define P_CONTROLE_H
#include "Estruturas_de_Dados.h"
#include "../Headers/P_Gerenciador_Processos.h"
int Executar_P_Controle();
void FFilaVazia(Programa *prog);
void LerTerminal(char *str_enviada);
int LerArquivo(char *str_enviada);
int EstaVazia(Programa prog);
void AdicionaProgramaFila(Programa *programa, char *instrucao);
void RetiraProgramaFila(Programa *prog,char *instrucao);
#endif //P_CONTROLE_H
