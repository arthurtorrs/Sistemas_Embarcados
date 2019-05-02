#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

int main()
{
	char placa[10], status[10];
	int opcao;
	
	FILE *fptr;
	fptr = (fopen("placasregistradas.txt", "a"));
	if(fptr == NULL)
	{
		printf("Deu erro!");
		exit(1);
	}

	printf("Digite a placa (formato ABC1234): ");
	scanf("%s", placa);
	
	//pega a data e hora atual    
	//time_t t = time(NULL);
	//struct tm tm = *localtime(&t);  
	
	//fprintf(fptr,"%d/%d/%d		%d:%d:%d	\n", tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900,   tm.tm_hour, tm.tm_min, tm.tm_sec);
	fprintf(fptr,"%s\n", placa);

	fclose(fptr);
	return 0;
}


