#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

int main()
{
	char placa[10];
	
	FILE *fptr;
	fptr = (fopen("placasregistradas.txt", "a"));
	if(fptr == NULL)
	{
		printf("Deu erro!");
		exit(1);
	}

	printf("Digite a placa (formato ABC1234): ");
	scanf("%s", placa);
	
	fprintf(fptr,"%s\n", placa);

	fclose(fptr);
	return 0;
}


