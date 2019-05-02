#include <stdio.h>
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
}

 
