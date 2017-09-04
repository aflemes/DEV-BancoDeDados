#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define URL "data/data.txt"
#define URL_NEW "data/data_shorter.txt"

struct data{
	int numero;
	char nome[10];
	int idade;
	float salario;
};

/*void write_new_file(struct data dataTemp){
	char numero[7];
	sprintf(numero, "%d", dataTemp.numero);	
	char idade[2];
	sprintf(idade, "%d", dataTemp.idade);
	char nome[11] = "";
	char salario[7] = "";
	char linha[22] = "";
	//
	strncpy(nome, dataTemp.nome, 10);
	
	nome[11] = '\0';
	
	strcat(salario,dataTemp.salario);
	
	if (atoi(numero) == 0)
		return;
		
	FILE *arquivo = fopen(URL_NEW, "a");
	
	if (strlen(numero) <= 7){
		int inicio = strlen(numero);
		
		for(int i = inicio; i < 7;i++){
			strcat(numero," ");
		}
	}
	strcat(numero,";");
		
	if (strlen(nome) < 10){
		int inicio = strlen(nome);
	
		for(int i = inicio; i < 10;i++){
			strcat(nome," ");
		}
	}
		
	strcat(nome,";");
	
	if (strlen(idade) < 2){
		int inicio = strlen(idade);
	
		for(int i = inicio; i < 2;i++){
			strcat(idade," ");
		}
	}
	strcat(idade,";");
	
	if (strlen(salario) < 7){
		int inicio = strlen(salario);
	
		for(int i = inicio; i < 2;i++){
			strcat(salario," ");
		}
	}
	
	if (salario[7] == 0){
		strcat(salario,"\n");	
	}
	//
	strcat(linha,numero);
	strcat(linha,nome);
	strcat(linha,idade);
	strcat(linha,salario);
	//
	fprintf(arquivo, linha);
		
	fclose(arquivo);
}
*/

void write_new_file(struct data dataTemp){
	FILE *arquivo = fopen(URL_NEW, "a");
	
	fwrite(&dataTemp,sizeof(struct data),1,arquivo);
	
	fclose(arquivo);
	
	printf("%d\n",dataTemp.numero);
}

void saveToStruct (char* str)
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
            		if (3 == flag){
						res.salario = atof(token);
            		}
		flag++;
        token = strtok( NULL, ";" ); 
    }
    
    if (flag == 4)
    	write_new_file(res);
    
}


void le_arquivo(){
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
		char *token;
		
		while (fgets(buffer, BUFFER_SIZE, arquivo))
    	{
    		 saveToStruct(buffer);
		}
	}
			
	fclose(arquivo);
	
	return;	
}

void read_arquivo(){
	int x;
	FILE *arquivo = fopen(URL_NEW, "r");
	struct data dataTemp;
	
	printf("\nEnter record number : ");
	scanf("%d",&x);
	arquivo = fopen("data/data_shorter.txt","r");
	
	if(arquivo==NULL)
	{
		printf("\nerror opening file...");
		exit(0);
	}
	
	fseek(arquivo,(x-1)*sizeof(struct data),SEEK_SET);
	if(fread(&dataTemp,sizeof(struct data),1,arquivo)==1)
	{
		printf("\nName = ");
		printf("%s\n",dataTemp.nome);
	}
	fclose(arquivo);	
}
 

int main(){
	le_arquivo();
	//read_arquivo();
	
	system("pause");
}









