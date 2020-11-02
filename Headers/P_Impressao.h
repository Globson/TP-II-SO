#ifndef P_IMPRESSAO_H
#define P_IMPRESSAO_H
#include <stdio.h>
#include <stdlib.h>
#include "RodaInstrucao.h"
void ImprimeSistemaCompleto(Cpu* cpu, PcbTable *pcbTable, EstadoBloqueado *estadobloqueado, EstadoPronto *estadopronto);
void ImprimeTempoMedioCiclo(Time *time);
#endif //P_IMPRESSAO_H
