#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define URL 	    "data/data.txt"
#define URL_INDEX   "data/index.txt"
//-----------------------------------
#define URL_SHUFFLE "data/data_shuffle.txt"
#define URL_SHUFFLE_INDEX "data/index_shuffle.txt"

struct data{
	int numero;
	char nome[10];
	int idade;
	float salario;
};

struct index{
	int numero;
	int endereco;
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

void lista_dados_index (struct index indexTemp){
	printf("%d  | ", indexTemp.numero);
	printf("%d  |" , indexTemp.endereco);
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

/*
* metodo responsavel por fazer a pesquisa binaria diretamente no arquivo de dados
*/ 
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

/*
* metodo responsavel por fazer a pesquisa binaria no arquivo de índice
*/ 
int pesquisa_binaria_index(FILE *indice,int nodo,int inicio, int fim){
	struct index indexTemp;
	int meio = 0;
	
	meio = (inicio + fim) / 2;
	
	fseek(indice,meio * sizeof(struct index),SEEK_SET);	
	fread(&indexTemp,sizeof(struct index),1,indice);
	
	if (nodo < indexTemp.numero){
		return pesquisa_binaria_index(indice,nodo,inicio, meio);
	}
	else
		if (nodo > indexTemp.numero){
			return pesquisa_binaria_index(indice,nodo,meio,fim);
		}
		else
			if (indexTemp.numero == nodo){
				return indexTemp.endereco;
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

void mostra_dados_index(){
	char ch;
	int BUFFER_SIZE = sizeof(index);
	FILE *index;
	
	struct index indexTemp;
	
	index = fopen(URL_INDEX, "rb");
	if(index == NULL)
	    printf("Erro, nao foi possivel abrir o arquivo\n");
	else{
		char buffer[BUFFER_SIZE];
		int flag = 0;
		int indice = 0;
		
		printf("Numero| Endereco\n");
    	   
		while (fread(&indexTemp,sizeof(struct index),1,index) != NULL)
		{
    		lista_dados_index(indexTemp);
		}
	}
			
	fclose(index);
	
	return;	
}

void mostra_dados(){
	char ch;
	int BUFFER_SIZE = sizeof(data), opcao = 0;
	FILE *arquivo;
	
	struct data dataTemp;
	
	system("cls");
	printf("Qual arquivo deseja listar?\n");
	printf("1 - Arquivo Sequencial\n");
	printf("2 - Arquivo nao Sequencial?\n");
	scanf("%d",&opcao);
	
	if (opcao == 1){
		arquivo = fopen(URL, "rb");	
	}
	else
		arquivo = fopen(URL_SHUFFLE, "rb");
	
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
void indexar(FILE *index,int numero, int endereco){
	struct index indexTemp;
	
	indexTemp.numero   = numero;
	indexTemp.endereco = endereco;
	
	fwrite(&indexTemp,sizeof(struct index),1,index);
}

void reindexar(){
	char ch;
	int BUFFER_SIZE = sizeof(data);
	int qtdeRegistros, opcao;
	FILE *arquivo;
	
	struct data dataTemp;
	
	system("cls");
	printf("Qual arquivo deseja listar?\n");
	printf("1 - Arquivo Sequencial\n");
	printf("2 - Arquivo nao Sequencial?\n");
	scanf("%d",&opcao);
	
	if (opcao == 1){
		arquivo = fopen(URL, "rb");	
	}
	else
		arquivo = fopen(URL_SHUFFLE, "rb");
		
	if(arquivo == NULL)
	    printf("Erro, nao foi possivel abrir o index\n");
	else{
		char buffer[BUFFER_SIZE];
		   
		system("cls");
		printf("reindexando arquivo, aguarde...\n");
		//seta para o inicio
		fseek(arquivo,0,SEEK_SET);
		//apaga o arquivo de indice e cria um novo
		FILE *index;
		if (opcao == 1)
			index = fopen(URL_INDEX, "wb");	
		else 
			index = fopen(URL_SHUFFLE_INDEX, "wb");
				
		while (fread(&dataTemp,sizeof(struct data),1,arquivo) != NULL)
		{
    		indexar(index,dataTemp.numero,ftell(arquivo));
		}
		
		fclose(index);
	}
			
	fclose(arquivo);
	
	return;		
}

void mostrar_por_index(){
	char ch;
	int BUFFER_SIZE = sizeof(data);
	int numero;
	struct index indexTemp;
	struct data dataTemp;
	char buffer[BUFFER_SIZE];
	FILE *index,*arquivo;
	
	index = fopen(URL_INDEX, "rb");	
	
	if(index == NULL){
	    printf("Erro, nao foi possivel abrir o index\n");
	    return;
	}
	
	printf("Qual numero deseja pesquisar?\n");
	scanf("%d",&numero);
		
	system("cls");
	printf("Procurando...\n");
		
	//seta para o fim do arquivo
	fseek(index,sizeof(struct index) * -1,SEEK_END);
	
	if (fread(&indexTemp,sizeof(struct index),1,index) > 0){
		int fim = indexTemp.numero;
		int endereco = pesquisa_binaria_index(index,numero,1,fim);	
		
		if (endereco > 0){
			arquivo = fopen(URL, "rb");			
			
			if (index == NULL)
				return;
			
			fseek(arquivo,sizeof(struct data) * endereco,SEEK_SET);	
			fread(&dataTemp,sizeof(struct data),1,arquivo);
			
			lista_dados(dataTemp);
		}
	}
			
	fclose(index);
	
	return;
	
}

void shuffle(){
	char ch;
	int BUFFER_SIZE = sizeof(data);
	int qtdeRegistros = 0;
	struct data dataTemp;
	char buffer[BUFFER_SIZE];
	FILE *shuffle,*arquivo;
	
	arquivo = fopen(URL, "rb");	
	
	if(arquivo == NULL){
	    printf("Erro, nao foi possivel abrir o index\n");
	    return;
	}
	//seta para o fim do arquivo
	fseek(arquivo,sizeof(struct data) * -1,SEEK_END);
	if (fread(&dataTemp,sizeof(struct data),1,arquivo) > 0){
		qtdeRegistros = dataTemp.numero;	
	}
	else return;
	
	system("cls");
	printf("Escrevendo novo arquivo, aguarde...\n");
	
	shuffle = fopen(URL_SHUFFLE, "wb");
	
	for (int i=qtdeRegistros; i > 0; i--){
		fseek(arquivo,sizeof(struct data) * i,SEEK_SET);	
		
		if (fread(&dataTemp,sizeof(struct data),1,arquivo) > 0){
			fwrite(&dataTemp,sizeof(struct data),1,shuffle);	
		}
	}
	
	fclose(arquivo);
	fclose(shuffle);
}

int main(){
	int in_opcao = 1;
	
	while(in_opcao != 0){
		printf("Menu de Opcoes\n");
		printf("1 - Listar Dados\n");
		printf("2 - Pesquisa Binaria\n");
		printf("3 - Reindexar\n");
		printf("4 - Pesquisa por Index\n");
		printf("5 - Listar Dados Index\n");
		printf("6 - Novo arquivo s/ ordem\n");
		
		scanf("%d",&in_opcao);
		
		switch(in_opcao){
			case 1:
				mostra_dados();
				break;
			case 2:
				pesquisa_binaria();
				break;
			case 3:
				reindexar();
				break;
			case 4:
				mostrar_por_index();
				break;
			case 5:
				mostra_dados_index();
				break;
			case 6:
				shuffle();
				break;
		}
		
		system("pause");
		system("cls");
	}
}

