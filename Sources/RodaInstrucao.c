#include "../Headers/RodaInstrucao.h"
void RodaInstrucao(Cpu *cpu, Time *time, EstadoEmExec *estadoexec, PcbTable *pcbTable, EstadoBloqueado *estadobloqueado, EstadoPronto *estadopronto, Processo *processo){
  char comando, instrucao[20];
  FILE *arqPrograma;
  Processo novoProcesso;
  Programa novoPrograma;
  FFilaVazia(&novoPrograma);

  strcpy(instrucao, "");

  RetiraProgramaFila(&cpu->programa, instrucao);

  comando = instrucao[0];


  printf("\t\n-----------------Instucao -> %s",instrucao); //Debugando


  int i = 0,n1,n2;
  const char s[2] = " ";
  char *token;
  char *aux2,*aux3;
  switch (comando) {
      case 'N':
          token = strtok(instrucao, s);
          while( token != NULL ) {
            if(i == 1)
                aux2 = token;
            token = strtok(NULL, s);
            i++;
          }
          n1 = atoi(aux2);
          printf("Valor 1: %d\n", n1);
          cpu->Quant_Inteiros = n1; //debugando
          printf("Valor guardado em CPU: %d",cpu->Quant_Inteiros);
          break;
      case 'D':
          token = strtok(instrucao, s);
          while( token != NULL ) {
            if(i == 1)
                aux2 = token;
            token = strtok(NULL, s);
            i++;
          }
          n1 = atoi(aux2);
          printf("Valor 1: %d\n", n1);
          printf("Valor guardado em CPU: %d",cpu->Quant_Inteiros);
          if(cpu->Alocado_V_inteiros == 0){
            printf("\nENTROU em nao alocado");
            cpu->valorInteiro = (int*) malloc(sizeof(int)*cpu->Quant_Inteiros);
            cpu->valorInteiro[n1]=0;
            cpu->Alocado_V_inteiros =1; //Foi alocado, porem apenas posição especificada foi inicializada com 0;
          }
          else{
            printf("\nENTROU alocado");
            cpu->valorInteiro[n1]=0; //Caso ja encontre alocado,basta inicializar tal posicao.
          }
          break;
      case 'V':  /* Define o valor da variável inteira para n, onde n é um inteiro. */
          token = strtok(instrucao, s);
          while( token != NULL ) {
            if(i == 1)
              aux2 = token;
            else
              aux3 = token;
            token = strtok(NULL, s);
            i++;
          }
          n1 = atoi(aux2);
          n2 = atoi(aux3);
          printf("Valor 1: %d\n", n1);
          printf("Valor 2:%d\n", n2);
          cpu->valorInteiro[n1] = n2;
          printf("Variavel inteira: %d\n", cpu->valorInteiro[n1]);
          cpu->contadorProgramaAtual++;
          time->time++;
          break;
      case 'A': /* Adiciona n ao valor da variável inteira, onde n é um inteiro. */
          token = strtok(instrucao, s);
          while( token != NULL ) {
            if(i == 1)
              aux2 = token;
            else
              aux3 = token;
            token = strtok(NULL, s);
            i++;
          }
          n1 = atoi(aux2);
          n2 = atoi(aux3);
          printf("Valor 1: %d\n", n1);
          printf("Valor 2:%d\n", n2);
          cpu->valorInteiro[n1] += n2;
          printf("Variavel inteira: %d\n", cpu->valorInteiro[n1]);
          cpu->contadorProgramaAtual++;
          time->time++;
          break;
      case 'S': /* Subtrai n do valor da variável inteira, onde n é um inteiro. */
          token = strtok(instrucao, s);
          while( token != NULL ) {
            if(i == 1)
              aux2 = token;
            else
              aux3 = token;
          token = strtok(NULL, s);
          i++;
          }
          n1 = atoi(aux2);
          n2 = atoi(aux3);
          printf("Valor 1: %d\n", n1);
          printf("Valor 2:%d\n", n2);
          cpu->valorInteiro[n1] -= n2;
          printf("Variavel inteira: %d\n", cpu->valorInteiro[n1]);
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
          token = strtok(instrucao, s);
          while( token != NULL ) {
            if(i == 1)
                aux2 = token;
            token = strtok(NULL, s);
            i++;
          }
          n1 = atoi(aux2);
          printf("Valor 1: %d\n", n1);
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
