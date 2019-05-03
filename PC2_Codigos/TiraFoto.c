#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h> 
	
int main(int argc, char const *argv[])
{
	char command[100] = "fswebcam --jpeg 85 -D 1 ";
	char placa_foto[10];
	char extensao[10] = ".jpeg";	
	

	system("mkdir Fotos");	//cria a pasta Fotos
	chdir("Fotos/");	//seleciona a pasta Fotos (para ja salvar a foto na pasta)
	
	printf("Qual a placa foi tirada a foto (formato ABC1234)?: ");
	scanf("%s", placa_foto);
	
	strcat(placa_foto, extensao);
	strcat(command, placa_foto);
	printf("%s\n",command);

	system(command);	//tira a foto com nome foto.jpeg	
	printf("\nFoto tirada!\n\n");

	
	return 0;
}

/*#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h> 
	
int main(int argc, char const *argv[])
{
	int ret;
	
	system("mkdir Fotos");	//cria a pasta Fotos
	chdir("Fotos/");	//seleciona a pasta Fotos (para ja salvar a foto na pasta)
	
	system("fswebcam --jpeg 85 -D 1 foto.jpeg");	//tira a foto com nome foto.jpeg	
	printf("\nFoto tirada!\n\n");

	return 0;
}*/

