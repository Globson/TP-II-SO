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
