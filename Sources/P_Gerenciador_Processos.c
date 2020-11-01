#include "../Headers/P_Gerenciador_Processos.h"
#include "../Headers/RodaInstrucao.h"

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

void ExecutaCPU(Cpu *cpu, Time *time, PcbTable *pcbTable, EstadoEmExec *estadoexec, EstadoBloqueado *estadobloqueado, EstadoPronto *estadopronto, Processo *processo){
  

  RodaInstrucao(cpu, time, estadoexec, pcbTable, estadobloqueado, estadopronto, processo);

  switch (processo->prioridade){
      case 0:
          cpu->fatiaTempoUsada += 1; break;
      case 1:
          cpu->fatiaTempoUsada += 2; break;
      case 2:
          cpu->fatiaTempoUsada += 4; break;
      case 3:
          cpu->fatiaTempoUsada += 8; break;
      default:
          printf("Erro ao atualizar fatia de tempo CPU!\n");
  }

  //Atualizando processo simulado
  processo->Estado_Processo.Inteiro = cpu->valorInteiro;
  processo->Estado_Processo.Cont = cpu->contadorProgramaAtual;
  processo->CotaCPU = cpu->fatiaTempoUsada;
  for (int i = 0; i < processo->Estado_Processo.Tam; i++) {
      strcpy(processo->Estado_Processo.Programa[i], cpu->programa.instrucoes[i]);
  }
  pcbTable->vetor[estadoexec->iPcbTable] = *processo;
  if(cpu->fatiaTempoUsada >= cpu->fatiaTempo) // caso a fatia ultrapassar a cota estabelecida, programa é bloqueado e aguarda novo escalonamento.
      EnfileiraBloqueado(estadobloqueado, processo);
}

void ImprimirCPU(Cpu *cpu){
      printf("\n\tInformacoes da CPU: \n");
      printf("\nPrograma na CPU: \n");
      for (int i = 0; i < cpu->programa.Tam; i++)
          printf("%s", cpu->programa.instrucoes[i]);
      printf("\n");
      printf("Contador de Programa Atual: %d\n", cpu->contadorProgramaAtual);
      printf("Valor Inteiro: %d\n", cpu->valorInteiro);
      printf("Fatia de Tempo Disponivel: %d\n", cpu->fatiaTempo);
      printf("Fatia de Tempo Usada: %d\n", cpu->fatiaTempoUsada);
      printf("\n\t--Fim da Lista--\n");
}
void ImprimePronto(EstadoPronto *estadopronto){
  int Aux;
  printf("\n\tFila de Processsos Prontos:\n");
  for (Aux = estadopronto->Frente; Aux <= (estadopronto->Tras - 1); Aux++){
      printf("PID: %i\n", estadopronto->vetor[Aux].pid);
      printf("PID Pai: %i\n", estadopronto->vetor[Aux].pid_do_pai);
      printf("Estado: %s\n", estadopronto->vetor[Aux].estado);
      printf("Tempo CPU: %d\n", estadopronto->vetor[Aux].CotaCPU);
      printf("Tempo Inicio: %d\n", estadopronto->vetor[Aux].startupTime);
      printf("Prioridade: %d\n", estadopronto->vetor[Aux].prioridade);
      printf("Valor Inteiro: %d\n", estadopronto->vetor[Aux].Estado_Processo.Inteiro);
      printf("Contador de Programa: %d\n", estadopronto->vetor[Aux].Estado_Processo.Cont);
      printf("Programa: \n");
      for (int i = 0; i < estadopronto->vetor[Aux].Estado_Processo.Tam; i++)
          printf("%s", estadopronto->vetor[Aux].Estado_Processo.Programa[i]);
      printf("\n\t--Fim da Lista--\n");
  }
}
void ImprimeBloqueado(EstadoBloqueado *estadobloqueado){
  int Aux;
  printf("\n\tFila de Processsos Bloqueados:\n");
  for (Aux = estadobloqueado->Frente; Aux <= (estadobloqueado->Tras - 1); Aux++){
      printf("PID: %i\n", estadobloqueado->vetor[Aux].pid);
      printf("PID Pai: %i\n", estadobloqueado->vetor[Aux].pid_do_pai);
      printf("Estado: %s\n", estadobloqueado->vetor[Aux].estado);
      printf("Tempo CPU: %d\n", estadobloqueado->vetor[Aux].CotaCPU);
      printf("Tempo Inicio: %d\n", estadobloqueado->vetor[Aux].startupTime);
      printf("Prioridade: %d\n", estadobloqueado->vetor[Aux].prioridade);
      printf("Valor Inteiro: %d\n", estadobloqueado->vetor[Aux].Estado_Processo.Inteiro);
      printf("Contador de Programa: %d\n", estadobloqueado->vetor[Aux].Estado_Processo.Cont);
      printf("Programa: \n");
      for (int i = 0; i < estadobloqueado->vetor[Aux].Estado_Processo.Tam; i++)
          printf("%s", estadobloqueado->vetor[Aux].Estado_Processo.Programa[i]);
      printf("\n\t--Fim da Lista--\n");
  }
}
void ImprimePcbTable(PcbTable *pcbTable){
  int Aux;
  printf("\n\tLista de Processos na Tabela:\n");
  for (Aux = pcbTable->Primeiro; Aux <= (pcbTable->Ultimo - 1); Aux++) {
      printf("PID: %i\n", pcbTable->vetor[Aux].pid);
      printf("PID Pai: %i\n", pcbTable->vetor[Aux].pid_do_pai);
      printf("Estado: %s\n", pcbTable->vetor[Aux].estado);
      printf("Tempo CPU: %d\n", pcbTable->vetor[Aux].CotaCPU);
      printf("Tempo Inicio: %d\n", pcbTable->vetor[Aux].startupTime);
      printf("Prioridade: %d\n", pcbTable->vetor[Aux].prioridade);
      printf("Valor Inteiro: %d\n", pcbTable->vetor[Aux].Estado_Processo.Inteiro);
      printf("Contador de Programa: %d\n", pcbTable->vetor[Aux].Estado_Processo.Cont);
      printf("Programa: \n");
      for (int i = 0; i < pcbTable->vetor[Aux].Estado_Processo.Tam; i++)
          printf("%s", pcbTable->vetor[Aux].Estado_Processo.Programa[i]);
      printf("\n\t--Fim da Lista--\n");
  }
}
void escalonamentoMultiplasFilas(Cpu *cpu, Time *time, PcbTable *pcbTable, EstadoEmExec *estadoexec, EstadoBloqueado *estadobloqueado, EstadoPronto *estadopronto, Processo *processo){
    int prioridade = 0;
    //Modificar esse for depois, para somente colocar número de processos necessarios, e não MAXTAM.
    //Coloca os processos que vão entrar em execução na tabela de processos
    for(int i = 0;i < MAXTAM-1;i++){
        RodaInstrucao(cpu, time, estadoexec, pcbTable, estadobloqueado, estadopronto, processo);
        pcbtable->vetor[i] = *processo;
        strcpy(pcbtable->vetor[i]->Estado_Processo.Programa[i], cpu->programa.instrucoes[i]);
    }
    //Escalonador
    while(prioridade < 3){
        for(int j = pcbtable->Primeiro;j < pcbtable->Ultimo;j++){
            //atualiza fatia de tempo disponivel
            if(pcbtable->vetor[j]->prioridade == prioridade){
                switch (pcbtable->vetor[i]){
            case 0:
                cpu->fatiaTempo += 1; break;
            case 1:
                cpu->fatiaTempo += 2; break;
            case 2:
                cpu->fatiaTempo += 4; break;
            case 3:
                cpu->fatiaTempo += 8; break;
            default:
                printf("Erro ao atualizar fatia de tempo CPU!\n");
        }
                strcpy(pcbtable->vetor[j]->estado, "EM EXECUCAO");
                pcbtable->vetor[j]->Estado_Processo.Inteiro = cpu->valorInteiro;
                pcbtable->vetor[j]->Estado_Processo.Cont = cpu->contadorProgramaAtual;
                pcbtable->vetor[j]->CotaCPU= cpu->fatiaTempoUsada;
                    if((cpu->fatiaTempoUsada >= cpu->fatiaTempo)){
                        EnfileiraBloqueado(estadobloqueado, processo);
                        if(pcbtable->vetor[j]->prioridade < 3){
                            pcbtable->vetor[j]->prioridade += 1;
                        }
                    }
                }
            }
        prioridade++;
    }
}
