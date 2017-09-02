#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define URL "data/data_short.txt"

struct data{
	int numero;
	char nome[10];
	int idade;
	float salario;
};

struct data saveToStruct (char* str)
{
    struct data res;
    int flag = 0;
    char *token = strtok(str, ";"); 

    while( token != NULL )
    {
        if (0 == flag)
            res.numero = atoi(token);
        else if (1 == flag)
            strcpy(res.nome, token);
        else
            res.idade = atoi(token);

        flag++;
        token = strtok( NULL, ";" ); 
    }
    return res;
}

void le_arquivo(){
	char ch;
	int BUFFER_SIZE = sizeof(data);
	FILE *arquivo;
	
	struct data arr[10];
	
	arquivo = fopen(URL, "r");
	if(arquivo == NULL)
	    printf("Erro, nao foi possivel abrir o arquivo\n");
	else{
		char buffer[BUFFER_SIZE];
    	int flag = 0;
    	
    	while (fgets(buffer, BUFFER_SIZE, arquivo))
    	{
        	arr[flag] = saveToStruct(buffer);
        	printf("%\n",arr[flag].numero);
        	
        	flag++;
    	}
	}
	    
			
	fclose(arquivo);
	
	return;	
}

int main(){

	le_arquivo();	
	
	system("pause");
	
}
