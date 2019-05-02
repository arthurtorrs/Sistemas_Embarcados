#include <stdio.h>
#include <stdlib.h>
#include <string.h>
	
int main(int argc, char const *argv[])
{
	int n=0;
	char string[30];
	FILE *fp = fopen("placasregistradas.txt", "r");
	if (fp == NULL)
	{
	    printf("Não foi possível acessar o arquivo\n");
	    exit(-1);
	}
	
	while(!feof(fp))			//enquanto não chegar ao fim do arquivo
	{
	    fscanf(fp,"%s",string);
	    if(strcmp(string,argv[1]) == 0)	//se encontra uma placa igual no texto
	    n++;
	}
	
		
	if(n==0)
	{
				
		printf("\nVisitante, dirija-se a portaria para identificação!\n\n");	
	}
	else
	{
		printf("\nMorador, acesso liberado!\n\n");	
	}
	
	return 0;
}
