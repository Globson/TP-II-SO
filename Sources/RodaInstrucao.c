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
  printf("\t-----------------Instucao -> %s",instrucao); //Debugando
  printf("Comando: %c\n", comando);

  switch (comando) {
      case 'N':
          break;
      case 'D':

          break;
      case 'V':  /* Define o valor da variável inteira para n, onde n é um inteiro. */
          cpu->valorInteiro = n;
          printf("Variavel inteira: %d\n", cpu->valorInteiro);
          cpu->contadorProgramaAtual++;
          time->time++;
          break;
      case 'A': /* Adiciona n ao valor da variável inteira, onde n é um inteiro. */
          cpu->valorInteiro += n;
          printf("Variavel inteira: %d\n", cpu->valorInteiro);
          cpu->contadorProgramaAtual++;
          time->time++;
          break;
      case 'S': /* Subtrai n do valor da variável inteira, onde n é um inteiro. */
          cpu->valorInteiro -= n;
          printf("Variavel inteira: %d\n", cpu->valorInteiro);
          cpu->contadorProgramaAtual++;
          time->time++;
          break;
      //
      case 'B': /* Bloqueia esse processo simulado. */
          EnfileiraBloqueado(estadobloqueado, processo);
          colocarProcessoCPU(cpu, estadopronto);
          //cpu->contadorProgramaAtual++; // Remover???
          time->time++;
          break;
      case 'T': /* Termina esse processo simulado. */
          RetiraPcbTable(pcbTable, estadoexec->iPcbTable, processo); // Precisa desalocar o programa.
          colocarProcessoCPU(cpu, estadopronto);
          time->time++;
          break;
      case 'F': /* Cria um novo processo simulado. */
          novoProcesso = criarProcessoSimulado(time, processo);
          EnfileiraPronto(estadopronto, &novoProcesso);
          InserePcbTable(pcbTable, novoProcesso);
          cpu->contadorProgramaAtual++; // Necessário para atualizar o contador do processo pai para a instrução logo após a instrução F.
          time->time++;
          break;
      case 'R': /* Substitui o programa do processo simulado pelo programa no arquivo nome_do_arquivo e define o contador de programa para a primeira instrução desse novo programa. */
          arqPrograma = fopen("./Arquivos_Entrada/Programa.txt", "r");

          if (arqPrograma == NULL) {
              printf("Erro, nao foi possivel abrir o arquivo ArquivoPrograma.txt\n");
          } else {
            int Quant_Instrucoes=0;
              while ((fgets(instrucao, sizeof(instrucao), arqPrograma)) != NULL) {
                  AdicionaProgramaFila(&novoPrograma, instrucao);
                  Quant_Instrucoes++;
              }
              novoPrograma.Tam = Quant_Instrucoes;
          }

          fclose(arqPrograma);

          cpu->contadorProgramaAtual = 0;
          cpu->valorInteiro = 0; // Indefinido?????
          time->time++;

          break;
      default:
          printf("Comando não suportado!\n");
  }

}
