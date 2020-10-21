all: main.c ./Sources/P_Controle.c ./Sources/P_Gerenciador_Processos.c ./Sources/P_Impressao.c
	gcc main.c -o EXEC ./Sources/P_Controle.c ./Sources/P_Gerenciador_Processos.c ./Sources/P_Impressao.c

clear: EXEC
	rm EXEC
