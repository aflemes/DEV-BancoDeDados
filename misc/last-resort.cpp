#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define URL "data/data.txt"
#define URL_2 "data/data_2.txt"
#define URL_NEW "data/data_new.txt"

struct data{
	int numero;
	char nome[12];
	int idade;
	float salario;
};

int sequencial = 1;

void saveToStruct (char* str, FILE *arquivo_escrita)
{
    struct data res;
    int flag = 0;
    char *token = strtok(str, ";"); 

	
	while( token != NULL )
    {	
    	if (0 == flag)
    		res.numero = atoi(token);
        else 
			if (1 == flag){			
            	strcpy(res.nome, token);            	
        	}
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
	  
    if (res.numero == 0){
		return;
    }
    	
    if (flag == 5){
    	res.numero = sequencial;
		fwrite(&res,sizeof(struct data),1,arquivo_escrita);
		sequencial++;
	}
    
}

void read(){
	int i = 1;
	FILE *fp = fopen(URL, "rb");
	struct data dataTemp;
	
	
	int leitura;
	while (fread(&dataTemp,1,sizeof(struct data),fp))
	{	
		printf("\nNumero = ");
		printf("%d ",dataTemp.numero);
		printf("--%s\n",dataTemp.nome);
	}
   
	fclose(fp);	
}

void write(){
	char ch;
	int BUFFER_SIZE = sizeof(data);
	FILE *arquivo;
	FILE *arquivo_2;
	FILE *arquivo_escrita;
	
	
	struct data dataTemp;
	
	arquivo_escrita = fopen(URL, "wb");
	arquivo = fopen(URL, "rb");
	arquivo_2 = fopen(URL, "rb");
	
	if(arquivo == NULL)
	    printf("Erro, nao foi possivel abrir o arquivo\n");
	else{
		char buffer[BUFFER_SIZE];
		int flag = 0;
		char *token;
		
		printf("processando...\n");
		while (fgets(buffer, BUFFER_SIZE, arquivo))
    	{
    		saveToStruct(buffer,arquivo_escrita);
    		flag++;
		}
		
		while (fgets(buffer, BUFFER_SIZE, arquivo_2))
    	{
    		saveToStruct(buffer,arquivo_escrita);
    		flag++;
		}
	}
			
	fclose(arquivo);
	fclose(arquivo_2);
	fclose(arquivo_escrita);	
}

int main(){
	//write();
	read();
}
