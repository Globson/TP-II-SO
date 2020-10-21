#include "../Headers/P_Controle.h"
int Executar_P_Controle(){
  int fd[2]; // File descriptor para Pipe
  pid_t pid;

  char str[2], str_enviada[BUFFER];
  FILE *ArquivoControle, *ArquivoPrograma;


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
    ArquivoControle = fopen("./Arquivos_Entrada/Controle.txt","r");
    if( ArquivoControle == NULL){
      printf("Erro ao abrir arquivo de entrada Controle.txt\n");
    }
    else{
      while(fscanf(ArquivoControle,"%s",str) != EOF){
        strcat(str_enviada,str);
        strcat(str_enviada," ");
      }
    }
    fclose(ArquivoControle);
    //Para escrever no pai, teremos que fechar a leitura do Pipe.
    close(fd[0]);
    printf("String enviada pelo Controle(PID %i) para o Gerenciador: %s \n", getpid(),str_enviada);

    //Escrevendo no pipe:
    write(fd[1], str_enviada,sizeof(str_enviada)+1);
    exit(0);
    return 1; // return 1 se entrou em else
  }
  else{ //Processo filho, o gerenciador.
    //TODO
    return 0; // return 0 se entrou em else
  }
}

void FilaVazia(Programa *prog){
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
