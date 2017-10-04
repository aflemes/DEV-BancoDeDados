#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define URL 	    "data/data.txt"
#define URL_INDEX   "data/index.txt"
//-----------------------------------
#define URL_SHUFFLE "data/data_shuffle.txt"
#define URL_SHUFFLE_INDEX "data/index_shuffle.txt"
//-----------------------------------
#define URL_BUCKET "data/bucket.txt"
#define BLOCOS 12000
#define LIM_BUCKET 100

/*
-------------------------------------------------------------------------- STRUCTS ------------------------------------------------------------------------------
*/
struct data{
	int numero;
	char nome[10];
	int idade;
	float salario;
};

struct integridade{
	long registrosArquivo;
	long registrosEncontrados;
	int integro;
};

struct index{
	int numero;
	int endereco;
};

struct nodo{
	char numero[10];
	char endereco[10];
};

struct bucket{
	char hash[10];
	struct nodo values[LIM_BUCKET];
};

struct header{
	int qtdeRegistros;
};

/*
-------------------------------------------------------------------------- METODOS ------------------------------------------------------------------------------
* Lista os dados do arquivo principal referente ao arquivo do tipo sequencial
*/
void lista_dados (struct data dataTemp){
	printf("%d  | ", dataTemp.numero);
	printf("%s  |" , dataTemp.nome);
	printf("%d  |" , dataTemp.idade);
	printf("%f  "  , dataTemp.salario);
	printf("\n");
}
/*
* Lista as informacoes do arquivo 'indice' referente ao tipo 'sequencial indexado' conforme a struct
*/
void lista_dados_index (struct index indexTemp){
	printf("%d  | ", indexTemp.numero);
	printf("%d  |" , indexTemp.endereco);
	printf("\n");
}
/*
* Lista as informacoes do arquivo 'bucket' referente ao acesso direto conforme a struct
*/
void lista_dados_bucket(struct bucket bucketTemp){
	printf("%s  | ", bucketTemp.hash);
	
	for(int i = 0; i < LIM_BUCKET; i++){
		if (strcmp(bucketTemp.values[i].numero,"") == 0)
			break;
		else
			printf("%s  |" , bucketTemp.values[i].numero);
	}
	printf("\n");
}

/*
* Metodo responsavel por fazer a pesquisa binaria diretamente no arquivo de dados
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

int pesquisa_sequencial(FILE *indice,int nodo){
	struct index indexTemp;
	rewind(indice);
	
	while (fread(&indexTemp,sizeof(struct index),1,indice) != NULL)
	{
		if (indexTemp.numero == nodo){
			return indexTemp.endereco;
		}
	}
	
	return 0;
}

/*
* Metodo responsavel por fazer a pesquisa binaria no arquivo de índice
*/ 
int pesquisa_binaria_index(FILE *indice,int nodo,int inicio, int fim){
	struct index indexTemp;
	int meio = 0;
	
	meio = (inicio + fim) / 2;
	
	fseek(indice,meio * sizeof(struct index),SEEK_SET);	
	fread(&indexTemp,sizeof(struct index),1,indice);
	
	if (nodo < indexTemp.numero){		
		printf("1- dataTemp.numero: %d nodo: %d inicio, %d meio: %d fim: %d\n",indexTemp.numero,nodo,inicio, meio, fim);
		system("pause");
		
		return pesquisa_binaria_index(indice,nodo,inicio, meio);
	}
	else
		if (nodo > indexTemp.numero){			
			printf("2- indexTemp.numero: %d nodo: %d inicio: %d meio: %d fim: %d\n",indexTemp.numero,nodo,inicio, meio, fim);
			system("pause");
						
			return pesquisa_binaria_index(indice,nodo,meio,fim);
		}
		else
			if (indexTemp.numero == nodo){
				return indexTemp.endereco;
			}
}
/*
* Metodo responsavel por fazer a pesquisa no arquivo sequencial, utilizando o algoritmo de pesquisa binaria;
*/
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
		fseek(arquivo,(long)sizeof(struct data) * -1,SEEK_END);
		
		if (fread(&dataTemp,sizeof(struct data),1,arquivo) > 0){
			int fim = dataTemp.numero;
			
			pesquisa_binaria_recursiva(arquivo,numero,1,fim);	
		}
	}
			
	fclose(arquivo);
	
}
/*
* Lista as informacoes no arquivo principal conforme o endereco encontrado no bucket
*/
void acessa_arquivo_principal(char endereco[10]){
	FILE *arquivo = fopen(URL,"r");	
	int posicao = atoi(endereco);
	struct data dataTemp;
	
	if (arquivo == NULL)
		return;
	
	rewind(arquivo);
	fseek(arquivo,sizeof(struct data) * posicao,SEEK_SET); 
	fread(&dataTemp,sizeof(struct data),1,arquivo);
	
	lista_dados(dataTemp);
}
/*
* Mostra os registros que estão 'indexados' pelo hashing utilizando a metodologia de acesso direto
*/
void mostra_dados_bucket(){
	char ch;
	int BUFFER_SIZE = sizeof(bucket), opcao = 0, numero = 0, func_hash = 0;
	FILE *bucket;
	
	struct bucket bucketTemp;
	
	bucket = fopen(URL_BUCKET, "rb");
	if(bucket == NULL)
	    printf("Erro, nao foi possivel abrir o arquivo\n");
	else{
		system("cls");
		printf("0 - Todos\n");
		printf("1 - Informado\n");
		scanf("%d",&opcao);
		
		if (opcao == 0){
			printf("Hash| Endereco\n");
			while (fread(&bucketTemp,sizeof(struct bucket),1,bucket) != NULL)
			{
				lista_dados_bucket(bucketTemp);
			}	
		}
		else{
			
			printf("Informe o numero que deseja pesquisar\n");
			scanf("%d",&numero);
			
			char numero_tmp[10];
			
			itoa(numero, numero_tmp, 10);
			
			func_hash = numero % BLOCOS;
			fseek(bucket,sizeof(struct bucket) * func_hash,SEEK_SET); 
			fread(&bucketTemp,sizeof(struct bucket),1,bucket);
			
			printf("Hash| Endereco\n");
			for (int i=0; i < sizeof(bucketTemp.values);i++){
				if (strcmp(bucketTemp.values[i].numero,numero_tmp) == 0){
					acessa_arquivo_principal(bucketTemp.values[i].endereco);
					break;
				}
				
				if (strcmp(bucketTemp.values[i].numero,"") == 0)
					break;
			}			
		}
	}			
	fclose(bucket);
}
/*
* Metodo responsavel por listar a quantidade utilizado do bucket
* o recomendado e que se utilize entre 70 e 90% do bucket
*/
void alocacao_hashing(){
	FILE *bucket;
	
	struct bucket bucketTemp;
	
	bucket = fopen(URL_BUCKET, "r");
	if(bucket == NULL)
	    printf("Erro, nao foi possivel abrir o arquivo\n");
	else{
		long qtdeTotal = BLOCOS * LIM_BUCKET;
		long qtdeUtilizado = 0;
		
		for (int i=0; i < BLOCOS;i++){
			fseek(bucket,sizeof(struct bucket) * i,SEEK_SET); 
			fread(&bucketTemp,sizeof(struct bucket),1,bucket);
			
			for (int j=0;j < LIM_BUCKET;j++){
				if (strcmp(bucketTemp.values[j].numero,"") == 0)
					break;
				else
					qtdeUtilizado++;
			}				
		}
		
		printf("Total utilizado: %d\n",qtdeUtilizado);
		printf("Total possivel : %d\n",qtdeTotal);
		float prct = (qtdeUtilizado * 100) / qtdeTotal;
		
		printf("Percentual Utilizado %.2f\n",prct);
	}			
	fclose(bucket);
	
}
/*
* Lista os registros que estao indexados
*/
void mostra_dados_index(){
	char ch;
	int contador = 0;
	FILE *index;
	
	struct index indexTemp;
	
	index = fopen(URL_SHUFFLE_INDEX, "rb");
	if(index == NULL)
	    printf("Erro, nao foi possivel abrir o arquivo\n");
	else{		
		printf("Numero| Endereco\n");
    	   
		while (fread(&indexTemp,sizeof(struct index),1,index) != NULL)
		{
    		lista_dados_index(indexTemp);
    		
    		contador++;
    		
    		if (contador > 1000){
    			break;
			}
		}
	}
			
	fclose(index);	
}
void inicializa(){
	FILE *arquivo = fopen(URL,"rb");
	FILE *index    = fopen(URL_SHUFFLE_INDEX,"wb");
	struct data dataTemp;
	struct index newStruct;
	
	fseek(arquivo,(long)sizeof(struct data) * -1,SEEK_END);
	fread(&dataTemp,sizeof(struct data),1,arquivo);	
	int qtdeRegistrosArquivo = dataTemp.numero;	
	
	for (int i=0; i < qtdeRegistrosArquivo;i++){
		newStruct.numero   = i;
		newStruct.endereco = sizeof(struct index) * i;	
	
		fwrite(&newStruct,sizeof(struct index),1,index);
	}	
	
	fclose(index);	
}
/*
* Metodo responsavel por listar os dados conforme o tipo de arquivo informado (sequencial ou nao)
*/
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
		
		printf("Numero| Nome   | Idade  | Salario\n");
    	   
		while (fread(&dataTemp,sizeof(struct data),1,arquivo) != NULL)
		{
    		lista_dados(dataTemp);
		}
	}			
	fclose(arquivo);
}
/*
* Escreve no arquivo de indice a struct
*/
void indexar_shuffle(FILE *index,int numero, int endereco){
	struct index newStruct;
	
	newStruct.numero   = numero;
	newStruct.endereco = endereco;
	
	fseek(index,sizeof(struct index) * numero,SEEK_SET);
	fwrite(&newStruct,sizeof(struct index),1,index);
	
}
/*
* Escreve no arquivo de indice a struct
*/
void indexar(FILE *index,int numero, int endereco){
	struct index indexTemp;
	
	indexTemp.numero   = numero;
	indexTemp.endereco = endereco;
	
	fwrite(&indexTemp,sizeof(struct index),1,index);
}

void write_shuffle(int numero,int endereco){
	FILE *index = fopen(URL_SHUFFLE_INDEX, "rb");
	struct index newStruct;
	
	newStruct.numero   = numero;
	newStruct.endereco = endereco;
	
	fseek(index,sizeof(struct index) * newStruct.numero,SEEK_SET);
	fwrite(&newStruct,sizeof(struct index),1,index);
	
	fclose(index);	
}

void reindexar_shuffle(){
	FILE *arquivo = fopen(URL,"rb");
	struct data dataTemp;
	struct index newStruct;
	int contador;
	
	inicializa();
	
	fseek(arquivo,0,SEEK_SET);
	
	while (fread(&dataTemp,sizeof(struct data),1,arquivo) != NULL)
	{		
		write_shuffle(dataTemp.numero,ftell(arquivo));	
		
		if (dataTemp.numero % 1000 == 0)	{
			printf("%d\n",dataTemp.numero);
		}
	}
}
/*
* Cria um novo arquivo de indice para o arquivo informado (Sequencial ou nao)
*/
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
		int qtdeRegistros;
		   
		system("cls");
		printf("reindexando arquivo, aguarde...\n");
		//seta para o inicio
		fseek(arquivo,0,SEEK_SET);
		//apaga o arquivo de indice e cria um novo
		FILE *index;
		if (opcao == 1){		
			index = fopen(URL_INDEX, "wb");	
			
			while (fread(&dataTemp,sizeof(struct data),1,arquivo) != NULL)
			{
	    		indexar(index,dataTemp.numero,ftell(arquivo));
			}
		}
		else	
			reindexar_shuffle();
		
		fclose(index);
	}			
	fclose(arquivo);
}
/*
* Pesquisa um determinado registro (pela chave principal, no caso numero)
* a pesquisa sera feita pelo indice, utilizando a pesquisa binaria no indice
*/
void mostrar_por_index(){
	char ch;
	int BUFFER_SIZE = sizeof(data);
	int numero,opcao;
	struct index indexTemp;
	struct data dataTemp;
	char buffer[BUFFER_SIZE];
	FILE *index,*arquivo;
	
	system("cls");
	printf("Qual arquivo deseja listar?\n");
	printf("1 - Arquivo Sequencial\n");
	printf("2 - Arquivo nao Sequencial?\n");
	scanf("%d",&opcao);
	
	if (opcao == 1){
		index = fopen(URL_INDEX, "rb");	
	}
	else
		index = fopen(URL_SHUFFLE_INDEX, "rb");
		
	if(index == NULL){
	    printf("Erro, nao foi possivel abrir o index\n");
	    return;
	}
	
	printf("Qual numero deseja pesquisar?\n");
	scanf("%d",&numero);
		
	system("cls");
	printf("Procurando...\n");
		
	//seta para o fim do arquivo
	fseek(index,(long)sizeof(struct index) * -1,SEEK_END);
	
	if (fread(&indexTemp,sizeof(struct index),1,index) > 0){
		int fim = indexTemp.numero;
		int endereco = 0;
		
		endereco = pesquisa_binaria_index(index,numero,1,fim);	
		
		if (endereco > 0){
			arquivo = fopen(URL, "rb");			
			
			if (index == NULL)
				return;
			
			fseek(arquivo,endereco - sizeof(struct data),SEEK_SET);	
			fread(&dataTemp,sizeof(struct data),1,arquivo);
			
			lista_dados(dataTemp);
		}
	}
			
	fclose(index);
}
/*
* Pega o arquivo principal 'sequencial' e cria um novo sem a sequencialidade
*/
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
	fseek(arquivo,(long)sizeof(struct data) * -1,SEEK_END);
	if (fread(&dataTemp,sizeof(struct data),1,arquivo) > 0){
		qtdeRegistros = dataTemp.numero;	
	}
	else return;
	
	system("cls");
	printf("Escrevendo novo arquivo, aguarde...\n");
	
	shuffle = fopen(URL_SHUFFLE, "wb");
	
	for (int i=0; i < qtdeRegistros; i+=5){
		for (int j = 5; j > 0 ;j--){
			fseek(arquivo,sizeof(struct data) * (j + i),SEEK_SET);
		
			if (fread(&dataTemp,sizeof(struct data),1,arquivo) > 0){
				fwrite(&dataTemp,sizeof(struct data),1,shuffle);	
			}
		}
	}
	
	fclose(arquivo);
	fclose(shuffle);
}
/*
* Inicializa o arquivo referente ao acesso direto 'hashing'
*/
void init_hashing(){
	FILE *bucket = fopen(URL_BUCKET, "wb");	
	FILE *arquivo = fopen(URL, "rb");
	struct data   dataTemp;
	struct bucket bucketTemp,bucketInitialize, newStruct;
	
	if(arquivo == NULL){
		printf("Nao foi possivel ler o arquivo.");
		return;
	}
	system("cls");
	printf("Inicializando o bucket...\n");
	//inicializa o bucket
	for (int i=0;i < BLOCOS; i++){
		itoa(i, bucketInitialize.hash, 10);
		
		for(int i=0; i < LIM_BUCKET; i++){
			strcpy(bucketInitialize.values[i].numero,"");
			//bucketInitialize.values[i].endereco = 0;
		}
		fwrite(&bucketInitialize,sizeof(struct bucket),1,bucket);
	}
	//commit
	fclose(bucket);
	bucket = fopen(URL_BUCKET, "r+");
	
	//seta para o fim do arquivo
	fseek(arquivo,(long)sizeof(struct data) * -1,SEEK_END);
	
	if (fread(&dataTemp,sizeof(struct data),1,arquivo) > 0){
		int qtdeRegistros = dataTemp.numero;
		int funcao_hash = 0;
		int value = 0;
		struct nodo nodoTemp;
		
		//le todos os registros do arquivo
		
		for (int i=0; i < qtdeRegistros; i++){
			fseek(arquivo,sizeof(struct data) * i,SEEK_SET);
		
			//le a linha
			if (fread(&dataTemp,sizeof(struct data),1,arquivo) > 0){
				value = dataTemp.numero;
				
				//funcao do hashing
				funcao_hash = value % BLOCOS;
				//seta a posicao cfe a funcao
				fseek(bucket,sizeof(struct bucket) * funcao_hash,SEEK_SET);	
				
				if (fread(&bucketTemp,sizeof(struct bucket),1,bucket) >= 0){
					fseek(bucket,sizeof(struct bucket) * funcao_hash,SEEK_SET);	
					
					int encontrei_posicao = 0;
					
					//varre os elementos do bucket
					for(int j=0;j < LIM_BUCKET;j++){
						//encontrei uma posicao valida 
						if (strcmp(bucketTemp.values[j].numero,"") == 0){
							itoa(value, nodoTemp.numero  , 10);
							itoa(i    , nodoTemp.endereco, 10);
							
							newStruct = bucketTemp;
							for (int k=0;k < LIM_BUCKET; k++){
								newStruct.values[k] = bucketTemp.values[k];	
							}							
							newStruct.values[j] = nodoTemp;
							encontrei_posicao = 1;
							break;
						}
					}
					
					if (encontrei_posicao == 1)						
						fwrite(&newStruct,sizeof(struct bucket),1,bucket);										
				}
			}
		}
	}
	fclose(bucket);
	fclose(arquivo);	
}
/*
* Verifica a integridade do arquivo sequencial 'default'
* metodo: vou ate a ultima posicao e busco o numero do ultimo registro, pelo fato do arquivo ser sequencial (1,2,3...) sem quebra,
* depois leio todos os registros e conto os registros
*/
struct integridade verif_sequencial(){
	int ret = 0, qtdeRegistros = 0,qtdeRegistrosArquivo = 0;
	FILE *arquivo = fopen(URL,"rb");
	struct integridade intTemp;
	struct data dataTemp;
	
	if (arquivo == NULL)
		return intTemp;
		
	rewind(arquivo);
	fseek(arquivo,(long)sizeof(struct data) * -1,SEEK_END);
	ret = fread(&dataTemp,sizeof(struct data),1,arquivo);
	
	if (ret > 0)
		qtdeRegistrosArquivo = dataTemp.numero;	
		
	rewind(arquivo);
	while (fread(&dataTemp,sizeof(struct data),1,arquivo) != NULL){
		qtdeRegistros++;	
	}
	
	intTemp.registrosArquivo     = qtdeRegistrosArquivo;
	intTemp.registrosEncontrados = qtdeRegistros;
	
	if (qtdeRegistros != qtdeRegistrosArquivo){
		printf("%d - %d",qtdeRegistros,qtdeRegistrosArquivo);
		intTemp.integro = 0;
	}
	else
		intTemp.integro = 1;
}
/*
* Verifica a integridade do arquivo indexado
*/
void verif_indexado(){
	FILE *arquivo = fopen(URL_INDEX,"rb");
	struct index indexTemp;
	struct integridade intTemp;
	int qtdeRegistros;
	
	if (arquivo == NULL)
		return;	
		
	while (fread(&indexTemp,sizeof(struct index),1,arquivo) != NULL){
		qtdeRegistros++;	
	}
	
	intTemp = verif_sequencial();
	
	if (intTemp.registrosArquivo != qtdeRegistros){
		printf("Arquivo nao integro\n");		
	}
	else
		printf("Arquivo integro\n");
}
/*
* Verifica a integridade do arquivo referente ao acesso direto 'hashing'
*/
void verif_direto(){
	FILE *bucket = fopen(URL_BUCKET,"rb");
	struct bucket bucketTemp;
	struct integridade intTemp;
	int qtdeRegistros = 0;
	
	if (bucket == NULL)
		return;	
		
	rewind(bucket);
	while (fread(&bucketTemp,sizeof(struct bucket),1,bucket) != NULL)
	{		
		for(int i =0; i < LIM_BUCKET; i++){
			if (strcmp(bucketTemp.values[i].numero,"") == 0)
				break;
			else
				qtdeRegistros++;
		}
	}
	
	intTemp = verif_sequencial();
	
	if (intTemp.registrosArquivo != qtdeRegistros){
		printf("intTemp.registrosArquivo %d\n",intTemp.registrosArquivo);
		printf("qtdeRegistros %d\n",qtdeRegistros);
		
		printf("Arquivo nao integro\n");		
	}
	else
		printf("Arquivo integro\n");
	
}
/*
* Procedimento para verificar a integridade dos arquivos.
*/
void verif_integridade(){
	int opcao = 0;
	struct integridade intTemp;
	system("cls");
	printf("Informe qual arquivo deseja verificar a integridade\n");
	printf("0 - Sequencial\n");
	printf("1 - Indexado\n");
	printf("2 - Acesso Direto\n");
	scanf("%d",&opcao);
	
	switch(opcao){
		case 0:
			intTemp = verif_sequencial();
			
			if (intTemp.integro ==1){
				printf("Registro Integro\n");				
			}
			else
				printf("Registro Nao Integro\n");
			
			break;
		case 1:
			verif_indexado();
			break;
		case 2:
			verif_direto();
			break;
	}	
}
/*
inicio do bloco
*/
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
		printf("7 - Inicializa Hashing\n");
		printf("8 - Listar o bucket\n");
		printf("9 - Alocacao do bucket\n");
		printf("10 - Verificar Integridade\n");
		
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
			case 7:
				init_hashing();
				break;
			case 8:
				mostra_dados_bucket();
				break;
			case 9:
				alocacao_hashing();
				break;
			case 10:
				verif_integridade();
				break;			
		}
		
		system("pause");
		system("cls");
	}
}

