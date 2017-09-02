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

void lista_dados (struct data dataTemp){
	
	printf("%d  | ", dataTemp.numero);
	printf("%s  |", dataTemp.nome);
	printf("%d  |", dataTemp.idade);
	printf("%f  ", dataTemp.salario);
	printf("\n");
}

struct data saveToStruct (char* str)
{
    struct data res;
    int flag = 0;
    char *token = strtok(str, ";"); 

    while( token != NULL )
    {
    	if (0 == flag)
            res.numero = atoi(token);
        else 
			if (1 == flag)
            	strcpy(res.nome, token);
        	else 
				if (2 == flag)
            		res.idade = atoi(token);
            	else
            		res.salario = atof(token);
		flag++;
        token = strtok( NULL, ";" ); 
    }
    return res;
}

void pesquisa_binaria(){
	
}

void mostra_dados(){
	char ch;
	int BUFFER_SIZE = sizeof(data);
	FILE *arquivo;
	
	struct data dataTemp;
	
	arquivo = fopen(URL, "r");
	if(arquivo == NULL)
	    printf("Erro, nao foi possivel abrir o arquivo\n");
	else{
		char buffer[BUFFER_SIZE];
    	int flag = 0;
    	
    	printf("Numero| Nome   | Idade  | Salario\n");
    	while (fgets(buffer, BUFFER_SIZE, arquivo))
    	{
        	dataTemp = saveToStruct(buffer);
        	
        	lista_dados(dataTemp);
    	}
	}
			
	fclose(arquivo);
	
	return;	
}

int main(){
	int in_opcao = 1;
	
	while(in_opcao != 0){
		printf("Menu de Opcoes");
		printf("1 - Listar Dados");
		printf("2 - Pesquisa Binaria");
		printf("3 - Consulta pela Pesquisa Binaria");
		
		scanf("%d",&in_opcao);
		
		switch(in_opcao){
			case 0:
				mostra_dados();
				break;
			case 1:
				pesquisa_binaria();
				break;
		}
	}
	
	system("pause");
	
}
