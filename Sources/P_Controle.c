#include "../Headers/P_Controle.h"

int Executar_P_Controle(){
  int fd[2],fd_s[2]; // File descriptor para Pipe
  pid_t pid;

  char str_enviada[BUFFER],str_recebida[BUFFER], Instrucao[20];
  FILE *ArquivoPrograma;
  int Quant_Instrucoes=0;
  Programa Prog;
  if(pipe(fd) < 0){ //pipe
    perror("pipe");
    return -1; //retorna -1 em caso de Erro
  }
  if((pid = fork())<0){ //processo filho, caso fork retorne numero negativo, houve erro
    perror("fork");
    exit(1);
  }
  /* //identificação de processos pai e filho
  if(pid == 0)
    {
        printf("pid do Filho: %d\n", getpid());
    }
    else
    {
        printf("pid do Pai: %d\n", getpid());
    }
  */
  if(pid > 0){ //processo pai
    LerArquivo(str_enviada); // Ler a partir de um arquivo
    //Para escrever no pai, teremos que fechar a leitura do Pipe.
    close(fd[0]);
    printf("String enviada pelo Controle(PID %i) para o Gerenciador: %s \n", getpid(),str_enviada);

    //Escrevendo no pipe:
    write(fd[1], str_enviada,sizeof(str_enviada)+1);
    exit(0);
    return 1; // return 1 se entrou em else
  }
  else{ //Processo filho, o gerenciador.
    EstadoEmExec estadoexec;
    EstadoPronto estadopronto;
    EstadoBloqueado estadobloqueado;
    PcbTable pcbTable;
    Cpu cpu;
    Time time;

    Processo processoDesbloqueado;
    int desenfileirou;
    //int pidProximoSimulado = 0;

    /* Criando Pipe. */
    if (pipe(fd_s) < 0) {
        perror("pipe");
        return -1;
    }
    Inicializa(&estadoexec, &estadopronto, &estadobloqueado, &pcbTable, &cpu, &time);

    ArquivoPrograma = fopen("./Arquivos_Entrada/Programa.txt", "r");

    if (ArquivoPrograma == NULL) {
        printf("Erro, nao foi possivel abrir o arquivo ArquivoProgramaIni.txt\n");
    } else {
        while((fgets(Instrucao, sizeof(Instrucao), ArquivoPrograma)) != NULL ){
                AdicionaProgramaFila(&Prog, Instrucao);
                Quant_Instrucoes++;
              }
    }

    // printf("Quantidade de Instruções: %d\n", Quant_Instrucoes);

    fclose(ArquivoPrograma);

    Prog.Tam = Quant_Instrucoes;

    Processo processo = criarPrimeiroSimulado(&Prog, &time, Quant_Instrucoes, getpid());
    EnfileiraPronto(&estadopronto, &processo);

    // ImprimePronto(&estadopronto);

    InserePcbTable(&pcbTable, processo);

    /* No filho, vamos ler. Então vamos fechar a entrada de ESCRITA do pipe. */
    close(fd[1]);

    /* Lendo o que foi escrito no pipe, e armazenando isso em 'str_recebida'. */
    read(fd[0], str_recebida, sizeof(str_recebida));

    printf("String LIDA pelo MANAGER de PID %i recebida pelo COMMANDER: '%s'\n\n", getpid(), str_recebida);

    processo = colocarProcessoCPU(&cpu, &estadopronto);

    for (unsigned int j = 0; j < strlen(str_recebida); j+=2) {
        //printf("\n%c\n", str_recebida[j]);
        switch (str_recebida[j]) {
            case 'U': // Fim de uma unidade de tempo. Executa próxima instrução.
                ExecutaCPU(&cpu, &time, &pcbTable, &estadoexec, &estadobloqueado, &estadopronto, &processo); //Comentei por estar incompleto e com bug
                // ImprimePcbTable(&pcbTable);
                // ImprimirCPU(&cpu);
                break;
            case 'L': // Desbloqueia o primeiro processo simulado na fila bloqueada.
                desenfileirou = DesenfileiraBloqueado(&estadobloqueado, &processoDesbloqueado);
                if(desenfileirou)
                    EnfileiraPronto(&estadopronto, &processoDesbloqueado);
                break;
            case 'I': // Imprime o estado atual do sistema.
                //TODO chamar funções imprime
                break;
            case 'M': // Imprime o tempo médio do ciclo e finaliza o sistema.
                //TODO
                break;
            default:
                printf("Comando '%c' não aceito!\n", str_recebida[j]);
                break;
        }
    }


            //exit(0);
    return 0; // return 0 se entrou em else
  }
}

int LerArquivo(char *str_enviada){ //Simplifiquei algumas coisas
    FILE *ArquivoControle;
    char str[2];
    ArquivoControle = fopen("./Arquivos_Entrada/Controle.txt","r");
    if( ArquivoControle == NULL){
      printf("Erro ao abrir arquivo de entrada Controle.txt\n");
      return 0; //ERRO
    }
    else{
      while(fscanf(ArquivoControle,"%s",str) != EOF){
        strcat(str_enviada,str);
        strcat(str_enviada," ");
      }
    }
    fclose(ArquivoControle);
    return 1;
}

void LerTerminal(char *str_enviada){
    char comando;
    int i = 0;
    do{
        scanf(" %c",&comando);
        str_enviada[i] = comando;
        str_enviada[i+1]= ' ';
        i+=2; //Adicionei espaços entre os caracteres para manter padrão da string.
    }while(comando != 'M');

}

void FFilaVazia(Programa *prog){
  prog->Frente = 0;
  prog->Tras = 0;
}
int EstaVazia(Programa prog){
  return (prog.Frente == prog.Tras);
}

void AdicionaProgramaFila(Programa *prog, char *instrucao){
  if(prog->Tras% MAXTAM +1 == prog->Frente)
    printf("Erro! Fila de programas cheia!\n");
  else{
    strcpy(prog->instrucoes[prog->Tras],instrucao);
    prog->Tras = prog->Tras % MAXTAM +1;
  }
}
void RetiraProgramaFila(Programa *prog,char *instrucao){
  if(EstaVazia(*prog))
    printf("Erro! A fila esta vazia.\n");
  else{
    strcpy(instrucao,prog->instrucoes[prog->Frente]);
    prog->Frente = prog->Frente % MAXTAM +1;
  }
}
