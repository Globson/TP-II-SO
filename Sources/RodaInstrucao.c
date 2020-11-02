#include "../Headers/RodaInstrucao.h"
void RodaInstrucao(Cpu *cpu, Time *time, EstadoEmExec *estadoexec, PcbTable *pcbTable, EstadoBloqueado *estadobloqueado, EstadoPronto *estadopronto, Processo *processo){
  char comando, instrucao[20];
  FILE *arqPrograma;
  Processo novoProcesso;


  strcpy(instrucao, "");

  RetiraProgramaFila(&cpu->programa, instrucao);

  comando = instrucao[0];


  printf("\t\n-----------------Instucao -> %s",instrucao); //Debugando


  int i = 0,j=2,n1=0,n2=0;
  const char s[2] = " ";
  char *token;
  char *aux2,*aux3,ArquivoNovo[20]="", Path[40];
  strcpy(Path,"./Arquivos_Entrada/");
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
          cpu->contadorProgramaAtual++;
          time->time++;
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
          cpu->contadorProgramaAtual++;
          time->time++;
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
          printf("Valor 2: %d\n", n2);
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
          cpu->contadorProgramaAtual++;
          time->time++;
          break;
      case 'T': /* Termina esse processo simulado. */
          RetiraPcbTable(pcbTable, estadoexec->iPcbTable, processo); // Precisa desalocar o programa.
          colocarProcessoCPU(cpu, estadopronto);
          time->time++;
          break;
      case 'F': /* Cria um novo processo simulado continuando da instrucao N. */
          token = strtok(instrucao, s);
          while( token != NULL ) {
            if(i == 1)
                aux2 = token;
            token = strtok(NULL, s);
            i++;
          }
          n1 = atoi(aux2);
          printf("Valor 1: %d\n", n1);
          novoProcesso = criarProcessoSimulado(time, processo, n1);
          EnfileiraPronto(estadopronto, &novoProcesso);
          InserePcbTable(pcbTable, novoProcesso);
          cpu->contadorProgramaAtual++; // Necessário para atualizar o contador do processo pai para a instrução logo após a instrução F.
          time->time++;
          break;
      case 'R': /* Substitui o programa do processo simulado pelo programa no arquivo nome_do_arquivo e define o contador de programa para a primeira instrução desse novo programa. */
          while((instrucao[j]!='.') && (instrucao[j+1]!='t') && (j<25)){ //Acho q funciona bem
            ArquivoNovo[j-2]=instrucao[j];
            j++;
          }
          strcat(ArquivoNovo,".txt");
          strcat(Path,ArquivoNovo);
          printf("\n\t->Nome arquivo novo: %s ",Path);
          arqPrograma = fopen(Path, "r");
          if (arqPrograma == NULL) {
              printf("\n\t->Erro, nao foi possivel abrir o arquivo de entrada!");
          } else {
            printf("\n\t->Arquivo aberto com sucesso!");
            FFilaVazia(&cpu->programa);
            int Quant_Instrucoes=0;
              while ((fgets(instrucao, sizeof(instrucao), arqPrograma)) != NULL) {
                  AdicionaProgramaFila(&cpu->programa, instrucao);
                  //printf("\n\tInstrucao lida: %s",instrucao);
                  Quant_Instrucoes++;
              }
              cpu->programa.Tam = Quant_Instrucoes;
              fclose(arqPrograma);
          }

          cpu->contadorProgramaAtual = 0;
          free(cpu->valorInteiro);

          time->time++;
          break;
      default:
          printf("\n\t---->Comando não suportado! ( %c )<---\n",comando);
  }

}
