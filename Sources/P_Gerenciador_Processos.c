#include "../Headers/P_Gerenciador_Processos.h"

void Inicializa(EstadoEmExec *estadoexec, EstadoPronto *estadopronto, EstadoBloqueado *estadobloqueado,PcbTable *pcbTable, Cpu *cpu, Time *time){
  estadoexec->iPcbTable = 0;
  FFVaziaPronto(estadopronto);
  FFVaziaBloqueado(estadobloqueado);
  FLVaziaPcbTable(pcbTable);
  FFilaVazia(&cpu->programa);
  cpu->fatiaTempoUsada = 0;
  cpu->fatiaTempo = 0;
  cpu->contadorProgramaAtual = 0;
  cpu->valorInteiro = 0;
  time->time = 0;
}
Processo criarPrimeiroSimulado(Programa *programa, Time *time, int Quant_Instrucoes, int pid_Pai){
  Processo processo;
  processo.pid = 0;
  processo.pid_do_pai = pid_Pai;
  processo.prioridade = 0;
  processo.CotaCPU = 0;
  processo.startupTime = time->time;
  processo.Estado_Processo.Inteiro = 0;
  processo.Estado_Processo.Cont = 0;
  processo.Estado_Processo.Tam = Quant_Instrucoes;
  for (int k = 0; k < Quant_Instrucoes; k++) {
      strcpy(processo.Estado_Processo.Programa[k], programa->instrucoes[k]);
  }
  strcpy(processo.estado, "PRONTO");
  return processo;
}
Processo criarProcessoSimulado(Time *time, Processo *processoPai){
  Processo processo;
  processo.pid = processoPai->pid + 1; //Acredito que forma de setar novo pid esteja errado.
  processo.pid_do_pai = processoPai->pid;
  processo.prioridade = processoPai->prioridade;
  processo.CotaCPU = 0;
  processo.startupTime = time->time;
  processo.Estado_Processo.Inteiro = processoPai->Estado_Processo.Inteiro;
  processo.Estado_Processo.Cont = processoPai->Estado_Processo.Cont + 1;
  processo.Estado_Processo.Tam = processoPai->Estado_Processo.Tam;
  for (int i = 0; i < processoPai->Estado_Processo.Tam; i++) {
      strcpy(processo.Estado_Processo.Programa[i], processoPai->Estado_Processo.Programa[i]);
  }
  strcpy(processo.estado, "PRONTO");
  return processo;
}
Processo colocarProcessoCPU(Cpu *cpu, EstadoPronto *estadopronto){
  Processo processo;

  DesenfileiraPronto(estadopronto, &processo);

  cpu->programa.Tam = processo.Estado_Processo.Tam;

  for (int i = 0; i < cpu->programa.Tam; i++) {
      AdicionaProgramaFila(&cpu->programa, processo.Estado_Processo.Programa[i]);
  }

  cpu->contadorProgramaAtual = processo.Estado_Processo.Cont;
  cpu->fatiaTempo = 10;
  cpu->fatiaTempoUsada = 0;
  cpu->valorInteiro = processo.Estado_Processo.Inteiro;

  return processo;
}

void FFVaziaPronto(EstadoPronto *estadopronto){
  estadopronto->Frente = 0;
  estadopronto->Tras = estadopronto->Frente;
}
void FFVaziaBloqueado(EstadoBloqueado *estadobloqueado){
  estadobloqueado->Frente = 0;
  estadobloqueado->Tras = estadobloqueado->Frente;
}
int VaziaPronto(EstadoPronto *estadopronto){
  return (estadopronto->Frente == estadopronto->Tras);
}
int VaziaBloqueado(EstadoBloqueado *estadobloqueado){
  return (estadobloqueado->Frente == estadobloqueado->Tras);
}
void EnfileiraPronto(EstadoPronto *estadopronto, Processo *processo){
  if (estadopronto->Tras % MAXTAM + 1 == estadopronto->Frente)
      printf("Erro fila Ready esta cheia\n");
  else {
      strcpy(processo->estado, "PRONTO");
      estadopronto->vetor[estadopronto->Tras] = *processo;
      printf("\nProcesso de PID %i adicionado a FILA PRONTO!\n", processo->pid);
      estadopronto->Tras = estadopronto->Tras % MAXTAM + 1;
  }
}
void EnfileiraBloqueado(EstadoBloqueado *estadobloqueado, Processo *processo){
  if (estadobloqueado->Tras % MAXTAM + 1 == estadobloqueado->Frente)
      printf("\nErro fila Blocked esta  cheia!\n");
  else {
      strcpy(processo->estado, "BLOQUEADO");
      estadobloqueado->vetor[estadobloqueado->Tras] = *processo;
      printf("\nProcesso de PID %i adicionado a FILA BLOQUEADO!\n", processo->pid);
      estadobloqueado->Tras = estadobloqueado->Tras % MAXTAM + 1;
  }
}
void DesenfileiraPronto(EstadoPronto *estadopronto, Processo *processo){
  if (VaziaPronto(estadopronto))
      printf("\nErro fila Ready esta vazia\n");
  else {
      *processo = estadopronto->vetor[estadopronto->Frente];
      estadopronto->Frente = estadopronto->Frente % MAXTAM + 1;
  }
}
int DesenfileiraBloqueado(EstadoBloqueado *estadobloqueado, Processo *processo){
  if (VaziaBloqueado(estadobloqueado)){
      printf("\nErro fila Blocked esta vazia\n");
      return 0;
  }
  else {
      *processo = estadobloqueado->vetor[estadobloqueado->Frente];
      estadobloqueado->Frente = estadobloqueado->Frente % MAXTAM + 1;
      return 1;
  }
}
void FLVaziaPcbTable(PcbTable *pcbTable){
  pcbTable->Primeiro = 0;
  pcbTable->Ultimo = pcbTable->Primeiro;
}
int VaziaPcbTable(PcbTable *pcbTable){
  return (pcbTable->Primeiro == pcbTable->Ultimo);
}
void InserePcbTable(PcbTable *pcbTable, Processo processo){
  if (pcbTable->Ultimo > MAXTAM) printf("Lista esta cheia\n");
  else {
      pcbTable->vetor[pcbTable->Ultimo] = processo;
      pcbTable->Ultimo++;
  }
}
void RetiraPcbTable(PcbTable *pcbTable, int indice, Processo *processo){
  int Aux;
  if (VaziaPcbTable(pcbTable) || indice >= pcbTable->Ultimo) {
      printf("Erro Posicao nao existe\n");
      return;
  }
  *processo = pcbTable->vetor[indice];
  pcbTable->Ultimo--;
  for (Aux = indice; Aux < pcbTable->Ultimo; Aux++)
      pcbTable->vetor[Aux] = pcbTable->vetor[Aux];
}

//TODO

void ExecutaCPU(Cpu *cpu, Time *time, PcbTable *pcbTable, EstadoEmExec *estadoexec, EstadoBloqueado *estadobloqueado, EstadoPronto *estadopronto, Processo *processo);

void ImprimirCPU(Cpu *cpu);
void ImprimePronto(EstadoPronto *estadopronto);
void ImprimeBloqueado(EstadoBloqueado *estadobloqueado);
void ImprimePcbTable(PcbTable *pcbTable);
