//primos.c – identifica todos os numeros primos ate um certo valor
// complile com -lm. Por exemplo: “gcc primo.c -o primo -lm”

#include<stdio.h>
#include <math.h>
#include <stdlib.h>
//#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>


int verifica_se_primo(long int);
long int numero = 0;
short int result, imprimir;


int main(int argc, char *argv[]) {

	int pipe_fd[2];

        //Verifica erro no pipe
        if(pipe(pipe_fd) == -1){
                fprintf(stderr, "Pipe - ERRO");
                return 1;
        }

        pid_t pid_t = fork();
	//Verifica erro no fork
	if(pid_t == -1) {
                printf("erro no fork");
                return 1;
        }

        if(pid_t > 0){ //Processo Pai 

                //Espera o filho
                wait(NULL);

		numero = atol(argv[1]);
                long int div = 0;

		//Lê dados do filho
                close(pipe_fd[1]); //Fecha a escrita
                read (pipe_fd[0], &div, sizeof(&div));
                close (pipe_fd[0]); //Fecha a leitura


                imprimir = atoi(argv[2]);
                for (long int num_int = div; num_int <= numero; num_int++) {
                        result = verifica_se_primo(num_int);
                        if (imprimir == 1)
                                if (result == 1)
                                        printf("%ld eh primo.\n", num_int);
                }
		return 0;

	}else{ //Processo Filho
                long int numero = 0;
                short int result, imprimir;
                if (argc != 3) {
                        printf ("Uso correto: %s <numero> <imprimir=1,nao_imprimir=0>\n\n", argv[0]);
                        return 1;
                }

                numero = atol(argv[1]);
                long int div = numero/2;

		//Fecha a leitura
                close (pipe_fd[0]);
		//Escreve os dados
                write(pipe_fd[1], &div, sizeof(div));
                close (pipe_fd[1]); 

                imprimir = atoi(argv[2]);
                for (long int num_int = 1; num_int < div; num_int++) {
                        result = verifica_se_primo(num_int);
                        if (imprimir == 1)
                                if (result == 1)
                                        printf("%ld eh primo.\n", num_int);
                }
                return 0;
        }
}

int verifica_se_primo(long int numero) {
	long int ant;
	for (ant = 2; ant <= (long int)sqrt(numero); ant++) {
		if (numero%ant == 0)
	return 0;
	}
	if (ant*ant >= numero)
	return 1;
}

