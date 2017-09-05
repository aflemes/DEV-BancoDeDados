#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define URL "data/data.txt"
#define URL_NEW "data/data_shorter_er.txt"

struct data{
	int numero;
	char nome[10];
	int idade;
	float salario;
};


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
    		printf("%s",buffer);
    		 saveToStruct(buffer);
		}
	}
			
	fclose(arquivo);
	
	return;	
}

void read_arquivo(){
	int i = 1;
	FILE *arquivo = fopen(URL_NEW, "r");
	struct data dataTemp;
	
	/*printf("\nEnter record number : ");
	scanf("%d",&x);*/
		
	if(arquivo==NULL)
	{
		printf("\nerror opening file...");
		exit(0);
	}
	
	int leitura;
	while (fread(&dataTemp,sizeof(struct data),1,arquivo))
	{		
		printf("\nName = ");
		printf("%s\n",dataTemp.nome);
	}
	fclose(arquivo);	
}
 

int main(){
	//le_arquivo();
	read_arquivo();
	
	system("pause");
}









