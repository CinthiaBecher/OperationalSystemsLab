#include<stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <curl/curl.h>

#define BUFFER_SIZE 1000

int countOccurrences(FILE *fptr, const char *word);

int main(){

        int pipe_fd1[2];
        int pipe_fd2[2];

        //Verifica erro no pipe
        if(pipe(pipe_fd2) == -1 || pipe(pipe_fd1) == -1){
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

                //Baixa as páginas usando o curl
		char url[60];
  		char file[] = "/home/cinthia/Área de Trabalho/aula05/pagina.html";
		char nameFile[] = "pagina.html";			//

  		CURL *curl;
  		CURLcode curl_res;
  		CURLINFO info;
  		long http_code;
  		double c_length;
  		FILE *tmp;

 		tmp=fopen(file, "w");
  		if(tmp==NULL) {
    			printf("Erro ao abrir o arquivo: %s\n",file);
    			exit(2);
  		}
	
  		curl = curl_easy_init();
  		printf("Qual a URL do arquivo a ser baixado pelo processo pai?\n");
  		scanf("%s",url);
  		curl_easy_setopt(curl, CURLOPT_URL, url);


  		curl_easy_setopt(curl, CURLOPT_WRITEDATA,  tmp);

  		printf("Baixando...\n");
  		curl_res = curl_easy_perform(curl);
  		if(curl_res==0) {
    			printf("Arquivo baixado!\n");
  		} else {
    			printf("Erro ao baixar o arquivo!\n");
    			fclose(tmp);
    			curl_easy_cleanup(curl);
  		}


	  	fclose(tmp);
 		curl_easy_cleanup(curl);

		printf("Passando o arquivo para o processo filho...\n");

                close (pipe_fd1[0]); // fecha a leitura 
               
	       	//Passa o arquivo para o filho
                write(pipe_fd1[1], &nameFile, sizeof(nameFile));
                close (pipe_fd1[1]);


                //Espera o filho
                wait(NULL);

		//Recebe a contagem do processo filho
                int wCount = 0;
                //Lê dados do filho
                close(pipe_fd2[1]); //Fecha a escrita
                read (pipe_fd2[0], &wCount, sizeof(wCount));
                close(pipe_fd2[0]); //Fecha a leitura


		printf("A palavra foi encontrada %d vezes.\n", wCount);

		return 0;

        }else{ //Processo Filho

                close(pipe_fd1[1]); //Fecha a escrita


		char nameFile[100]; 					
		//Recebe arquivo do pai
                read (pipe_fd1[0], nameFile, sizeof(nameFile));	
                close(pipe_fd1[0]); //Fecha a leitura


                //Conta o número de palavras

    		FILE *fptr;
    		char path[100];
    		char word[50];
		int wCount;


    		// Input palavra
    		printf("Qual a palavra que deseja encontrar? ");
    		scanf("%s", word);

    		// Abrir o file 
    		fptr = fopen(nameFile, "r");

    		// Erro ao abrir o file
    		if (fptr == NULL)
    		{
        		printf("Não foi possível abrir o arquivo.\n");
        		exit(EXIT_FAILURE);
    		}

    		// Função para a contagem de ocorrencia
    		wCount = countOccurrences(fptr, word);

    		// Fecha file
    		fclose(fptr);

                //Fecha a leitura
                close (pipe_fd2[0]);
                //Escreve os dados
                write(pipe_fd2[1], &wCount, sizeof(wCount));
                close(pipe_fd2[1]);

                return 0;
        }
}


/**
 * Returns a o numero de ocorrencia de uma palavra.
 */
int countOccurrences(FILE *fptr, const char *word)
{
    char str[BUFFER_SIZE];
    char *pos;

    int index, count;

    count = 0;

    // le linhas do arquivo até o final
    while ((fgets(str, BUFFER_SIZE, fptr)) != NULL)
    {
        index = 0;

        while ((pos = strstr(str + index, word)) != NULL)
        {
            index = (pos - str) + 1;
            count++;
        }
    }

    return count;
}

