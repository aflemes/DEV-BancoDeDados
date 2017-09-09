#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define URL "data/data.txt"

struct data{
	int numero;
	char nome[10];
	int idade;
	float salario;
};

struct header{
	int qtdeRegistros;
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

int pesquisa_binaria_recursiva(FILE *arquivo,int nodo,int inicio, int fim){
	struct data dataTemp;
	int meio = 0;
	
	meio = (inicio + fim) / 2;
	
	fseek(arquivo,meio * sizeof(struct data),SEEK_SET);	
	fread(&dataTemp,sizeof(struct data),1,arquivo);
	
	if (nodo < dataTemp.numero){
		/*
		printf("1- dataTemp.numero: %d nodo: %d inicio, %d meio: %d fim: %d\n",dataTemp.numero,nodo,inicio, meio, fim);
		system("pause");
		*/
		return pesquisa_binaria_recursiva(arquivo,nodo,inicio, meio);
	}
	else
		if (nodo > dataTemp.numero){
			/*
			printf("2- dataTemp.numero: %d nodo: %d inicio: %d meio: %d fim: %d\n",dataTemp.numero,nodo,inicio, meio, fim);
			system("pause");
			*/
			return pesquisa_binaria_recursiva(arquivo,nodo,meio,fim);
		}
		else
			if (dataTemp.numero == nodo){
				printf("Nro: %d\n", dataTemp.numero);
				printf("Nome: %s\n", dataTemp.nome);				
				printf("Idade: %d\n", dataTemp.idade);
				printf("Salario: %2.f\n", dataTemp.salario);
				return 1;
			}
	
	
}

void pesquisa_binaria(){
	int numero;
	char ch;
	int BUFFER_SIZE = sizeof(data);
	int BUFFER_HEADER_SIZE = sizeof(header);
	FILE *arquivo;
	struct data dataTemp;
	
	arquivo = fopen(URL, "r");
	
	if(arquivo == NULL)
	    printf("Erro, nao foi possivel abrir o arquivo\n");
	else{
		char buffer[BUFFER_SIZE];
		char buf_header[BUFFER_HEADER_SIZE];
		
		struct header header_tmp;
		int lg_encontrou = 0;
		
		printf("Qual numero deseja pesquisar?\n");
		scanf("%d",&numero);
		
		system("cls");
		printf("Procurando...\n");
		
		//seta para o fim do arquivo
		fseek(arquivo,sizeof(struct data) * -1,SEEK_END);
		
		if (fread(&dataTemp,sizeof(struct data),1,arquivo) > 0){
			int fim = dataTemp.numero;
			
			pesquisa_binaria_recursiva(arquivo,numero,1,fim);	
		}
	}
			
	fclose(arquivo);
	
	return;	
}

void mostra_dados(){
	char ch;
	int BUFFER_SIZE = sizeof(data);
	FILE *arquivo;
	
	struct data dataTemp;
	
	arquivo = fopen(URL, "rb");
	if(arquivo == NULL)
	    printf("Erro, nao foi possivel abrir o arquivo\n");
	else{
		char buffer[BUFFER_SIZE];
		int flag = 0;
		int indice = 0;
		
		printf("Numero| Nome   | Idade  | Salario\n");
    	   
		while (fread(&dataTemp,sizeof(struct data),1,arquivo) != NULL)
		{
    		lista_dados(dataTemp);
		}
	}
			
	fclose(arquivo);
	
	return;	
}

int main(){
	int in_opcao = 1;
	
	while(in_opcao != 0){
		printf("Menu de Opcoes\n");
		printf("1 - Listar Dados\n");
		printf("2 - Pesquisa Binaria\n");
		printf("3 - Consulta pela Pesquisa Binaria\n");
		
		scanf("%d",&in_opcao);
		
		switch(in_opcao){
			case 1:
				mostra_dados();
				break;
			case 2:
				pesquisa_binaria();
				break;
		}
		
		system("pause");
		system("cls");
	}
}

