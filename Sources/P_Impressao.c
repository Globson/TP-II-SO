#include "../Headers/P_Impressao.h"
void ImprimeSistemaCompleto(Cpu* cpu, PcbTable *pcbTable, EstadoBloqueado *estadobloqueado, EstadoPronto *estadopronto){
  printf("\n--Inicio impresssao do estado do sistema---\n\n");
  ImprimirCPU(cpu);
  printf("\n\n");
  ImprimePronto(estadopronto);
  printf("\n\n");
  ImprimeBloqueado(estadobloqueado);
  printf("\n\n");
  ImprimePcbTable(pcbTable);
  printf("\n\n--Fim da impresssao do estado do sistema---\n\n");
}
