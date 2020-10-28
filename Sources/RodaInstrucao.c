#include "../Headers/RodaInstrucao.h"
void RodaInstrucao(Cpu *cpu, Time *time, EstadoEmExec *estadoexec, PcbTable *pcbTable, EstadoBloqueado *estadobloqueado, EstadoPronto *estadopronto, Processo *processo){
  char comando, instrucao[20];
  FILE *arqPrograma;
  int n = 0;
  Processo novoProcesso;
  Programa novoPrograma;
  FFilaVazia(&novoPrograma);

  strcpy(instrucao, "");

  RetiraProgramaFila(&cpu->programa, instrucao);

  char *p = instrucao;
  while (*p) { // While there are more characters to process...   
      if (isdigit(*p) || ((*p == '-' || *p == '+') && isdigit(*(p + 1)))) {
          // Found a number
          n = strtol(p, &p, 10); // Read number
          printf("Inteiro extraido: %d\n", n); // and print it.
      } else {
          // Otherwise, move on to the next character.
          p++;
      }
  }

  comando = instrucao[0];

  printf("Comando: %c\n", comando);

  //Todo interpretação do comando

}
