#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(){

        int a[10][10],b[10][10],c[10][10],n,i,j,k;

        printf("Qual a ordem das matrizes? (N <= 10): ");
        scanf("%d",&n);

        int pipe_fd[2];

        //Detecta Erro no pipe
        if(pipe(pipe_fd) == -1){
                fprintf(stderr, "Erro no Pipe");
                return 1;
        }


        pid_t pid_t = fork();

        if(pid_t == -1) {
                printf("erro no fork");
                return 1;
        }

        if(pid_t > 0){

                wait(NULL);

		//Criacao matiz A
 		printf("\n------------------------------------\nInsira os elementos da Matriz - Processo Pai: \n");

                for(i=0;i<n;i++)
                {
                        for(j=0;j<n;j++)
                        {
                                scanf("%d",&a[i][j]);
                        }
                }

		//Lê a matriz b do processo filho
		close(pipe_fd[1]);
                read (pipe_fd[0], b, sizeof(b));

		//Multiplicacao
                for(i=0;i<n;i++)
                {
                        for(j=0;j<n;j++)
                        {
                                c[i][j]=0;
                                for(k=0;k<n;k++)
                                {
                                        c[i][j]+=a[i][k]*b[k][j];
                                }
                        }
                }

		//Print do resultado
                printf("\n------------------------------------\nO produto das matrizes: \n");
                for(i=0;i<n;i++)
                {
                        for(j=0;j<n;j++)
                        {
                                printf("%d\t",c[i][j]);
                        }
                        printf("\n");

                }

		close (pipe_fd[0]);


                printf("\nMULTIPLICAÇÃO FINALIZADA\n");
        }else{

		//Cria a Matriz B
		printf("\n------------------------------------\nInsira os elementos da Matriz - Processo Filho : \n");

		for(i=0;i<n;i++)
   		{		
        		for(j=0;j<n;j++)
        		{
            			scanf("%d",&b[i][j]);
        		}
    		}

		close(pipe_fd[0]);
                write(pipe_fd[1], b, sizeof(b));
                close (pipe_fd[1]);

	return 0;
	}
}
