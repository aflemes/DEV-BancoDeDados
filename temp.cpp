#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define URL_BUCKET "data/bucket.txt"
#define LIM_BUCKET 10

struct index{
	int numero;
	int endereco;
};

struct bucket{
	int hash;
	struct index values[LIM_BUCKET];
};

void cria_arquivo(){
	FILE *bucket;
	struct bucket bucketTemp;
	
	bucket = fopen(URL_BUCKET, "wb");
	if(bucket == NULL)
	    printf("Erro, nao foi possivel abrir o arquivo\n");
	else{
		for (int i=0; i < 20; i++){
			bucketTemp.hash = i;
			
			for(int i=0; i < LIM_BUCKET; i++){
				bucketTemp.values[i].numero   = 0;
				bucketTemp.values[i].endereco = 0;
			}
		
			fwrite(&bucketTemp,sizeof(struct bucket),1,bucket);	
		}
		
		fclose(bucket);
	}
}

void altera_arquivo(){
	FILE *bucket = fopen(URL_BUCKET, "r+");
	struct bucket bucketTemp;
	struct bucket newStruct;
	
	fseek(bucket,sizeof(struct bucket) * 5,SEEK_SET);
	fread(&bucketTemp,sizeof(struct bucket),1,bucket);
	fseek(bucket,sizeof(struct bucket) * 5,SEEK_SET);
	
	newStruct.hash      = 5;
	newStruct.values[0].numero = 99;
	newStruct.values[0].endereco = 99;
	
	fwrite(&newStruct,sizeof(struct bucket),1,bucket);
	
	fclose(bucket);
}

void mostra_arquivo(){
	char ch;
	int BUFFER_SIZE = sizeof(bucket);
	FILE *bucket;
	
	struct bucket bucketTemp;
	
	bucket = fopen(URL_BUCKET, "r+");
	
	fseek(bucket,0,SEEK_SET);
	
	printf("Hash| Indicador\n");
	   
	while (fread(&bucketTemp,sizeof(struct bucket),1,bucket) != NULL)
	{
		printf("%d  | ", bucketTemp.hash);
	
		for(int i = 0; i < 100; i++){
			if (bucketTemp.values[i].numero == 0)
				break;
			else
				printf("%d  |-" , bucketTemp.values[i].numero);
		}
		printf("\n");
	}
	fclose(bucket);
	
	return;	
}

int main(){

	cria_arquivo();	
	altera_arquivo();
	mostra_arquivo();
	
}
	
