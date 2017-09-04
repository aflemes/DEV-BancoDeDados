#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define URL "data/data_short.txt"
/*
struct data{
	int numero;
	char nome[10];
	int idade;
	float salario;
};
*/

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

int pesquisa_binaria_recv(FILE *arquivo, int qtdeRegistros){
	int BUFFER_SIZE = 31; //sizeof(data);
	char linha[BUFFER_SIZE] = """";
	int position =  20;
	
	//struct data dataTemp;
	struct data *dataTemp = (struct data *)malloc(sizeof(struct data));
	
	fseek(arquivo, qtdeRegistros * BUFFER_SIZE , SEEK_SET);	
	fgets(linha, BUFFER_SIZE, arquivo);
	printf("%s\n",linha);
	
	//fread(arquivo, "%d%s%d%f", &dataTemp.numero, &dataTemp.nome, dataTemp.idade, dataTemp.salario);
	//fread(linha,BUFFER_SIZE,1,arquivo);
	
	return 1;
	
}

void pesquisa_binaria(){
	int numero;
	char ch;
	int BUFFER_SIZE = sizeof(data);
	int BUFFER_HEADER_SIZE = sizeof(header);
	FILE *arquivo;
	
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
		
		
		//next
		//primeira linha do arquivo e o header
		/*
		fgets(buf_header, BUFFER_HEADER_SIZE, arquivo);
		char *token = strtok(buf_header, ";"); 

    	while( token != NULL )
    	{
    		header_tmp.qtdeRegistros = atoi(token);
			token = strtok( NULL, ";" ); 
		}
		*/
		
		lg_encontrou = pesquisa_binaria_recv(arquivo,numero);
		
	}
			
	fclose(arquivo);
	
	return;	
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
		
		fgets(buffer, BUFFER_SIZE, arquivo);
		//next
		//primeira linha do arquivo e o header
		
    	
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
