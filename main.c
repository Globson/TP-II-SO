#include "Headers/P_Controle.h"
#include "Headers/P_Gerenciador_Processos.h"
#include "Headers/P_Impressao.h"
#include <time.h>
int main(int argc, char const *argv[]) {
  srand(time(NULL));
  // Testando entrada por arquivo e por teclado.
  /*FILE *ArquivoControle;
  char ch[2], str_enviada[100];
  ArquivoControle = fopen("./Arquivos_Entrada/Controle.txt","r");
  while(fscanf(ArquivoControle,"%s",ch) != EOF){
    strcat(str_enviada,ch);
    strcat(str_enviada," ");
}
  printf("\t%s\n",str_enviada);

  char comando,str_enviada[100];;
  int i = 0;
  do{
      scanf(" %c",&comando);
      str_enviada[i] = comando;
      str_enviada[i+1]= ' ';
      i+=2;

    }while(comando != 'M');
    printf("\t%s\n",str_enviada);
*/
  Executar_P_Controle(); //Comecei os testes mesmo estado incompleto
  printf("\n\tExecutou TUDO!");
  return 0;
}
