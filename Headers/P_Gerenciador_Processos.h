#ifndef P_GERENCIADOR_PROCESSOS_H
#define P_GERENCIADOR_PROCESSOS_H
#include "P_Controle.h"


typedef struct Time{
  int time; //tempo de Cpu
  int QuantProcessosCriados; //Quantidade de processos criados ao todo.
}Time;

typedef struct Processo{
  pid_t pid;
  pid_t pid_do_pai;
  int prioridade;
  char estado[15];
  int startupTime;
  int CotaCPU;
  EstadoProcesso Estado_Processo;
} Processo;

typedef struct Cpu {
  Programa programa;
  int contadorProgramaAtual;
  int *valorInteiro;
  int Alocado_V_inteiros;
  int Quant_Inteiros;
  int fatiaTempo;
  int fatiaTempoUsada;
} Cpu;

typedef struct EstadoBloqueado {
    Processo vetor[MAXTAM];
    int Frente, Tras;
} EstadoBloqueado;

typedef struct EstadoPronto {
    Processo vetor[MAXTAM];
    int Frente, Tras;
} EstadoPronto;

typedef struct EstadoEmExec {
    int iPcbTable; //armazena o índice PcbTable do processo simulado atualmente em execução
} EstadoEmExec;

typedef struct PcbTable { //é um array com uma entrada para cada processo simulado que não terminou sua execução ainda.
    Processo vetor[MAXTAM];
    int Primeiro, Ultimo;
} PcbTable;

void Inicializa(EstadoEmExec *estadoexec, EstadoPronto *estadopronto, EstadoBloqueado *estadobloqueado,PcbTable *pcbTable, Cpu *cpu, Time *time);
Processo criarPrimeiroSimulado(Programa *programa, Time *time, int Quant_Instrucoes, int pid_Pai);
Processo criarProcessoSimulado(Time *time, Processo *processoPai, int Num_instrucao);
Processo colocarProcessoCPU(Cpu *cpu, EstadoPronto *estadopronto);
Processo ColocaOutroProcessoCPU(Cpu *cpu, EstadoPronto *estadopronto);
void FFVaziaPronto(EstadoPronto *estadopronto);
void FFVaziaBloqueado(EstadoBloqueado *estadobloqueado);
int VaziaPronto(EstadoPronto *estadopronto);
int VaziaBloqueado(EstadoBloqueado *estadobloqueado);
void EnfileiraPronto(EstadoPronto *estadopronto, Processo *processo);
void EnfileiraBloqueado(EstadoBloqueado *estadobloqueado, Processo *processo);
int DesenfileiraPronto(EstadoPronto *estadopronto, Processo *processo);
int DesenfileiraBloqueado(EstadoBloqueado *estadobloqueado, Processo *processo);
void FLVaziaPcbTable(PcbTable *pcbTable);
int VaziaPcbTable(PcbTable *pcbTable);
void InserePcbTable(PcbTable *pcbTable, Processo processo);
void RetiraPcbTable(PcbTable *pcbTable, int indice, Processo *processo);
void ExecutaCPU(Cpu *cpu, Time *time, PcbTable *pcbTable, EstadoEmExec *estadoexec, EstadoBloqueado *estadobloqueado, EstadoPronto *estadopronto, Processo *processo);
void ExecutaCPU2(Cpu *cpu, Time *time, PcbTable *pcbTable, EstadoEmExec *estadoexec, EstadoBloqueado *estadobloqueado, EstadoPronto *estadopronto, Processo *processo);
void ImprimirCPU(Cpu *cpu);
void ImprimePronto(EstadoPronto *estadopronto);
void ImprimeBloqueado(EstadoBloqueado *estadobloqueado);
void ImprimePcbTable(PcbTable *pcbTable);
#endif //P_GERENCIADOR_PROCESSOS_H
