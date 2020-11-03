#include "Headers/P_Controle.h"
#include "Headers/P_Gerenciador_Processos.h"
#include "Headers/P_Impressao.h"
#include <time.h>
int main(int argc, char const *argv[]) {
  srand(time(NULL));
  Executar_P_Controle();
  printf("\n\tExecutou TUDO!");
  return 0;
}
