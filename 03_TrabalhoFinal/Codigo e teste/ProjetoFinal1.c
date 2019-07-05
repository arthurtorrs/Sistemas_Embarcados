#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <wiringPi.h>

#define MAX 256
#define TRIG 23
#define ECHO 24

void setup()
{
	wiringPiSetupGpio();
	wiringPiSetup();
	pinMode(TRIG, OUTPUT);
	pinMode(ECHO, INPUT);

	//pino TRIG começa em LOW
	digitalWrite(TRIG, LOW);
	delay(30);
}

int getCM()
{

	//manda o pulso no TRIG
	digitalWrite(TRIG, HIGH);
	delayMicroseconds(20);
	digitalWrite(TRIG, LOW);

	//espera o ECHO começar
	while (digitalRead(ECHO) == LOW);

	//espera o ECHO terminar
	long startTime = micros();
	while (digitalRead(ECHO) == HIGH);
	long travelTime = micros() - startTime;

	//pega a distância em cm
	int distance = travelTime / 58;

	return distance;
}

int main(void)
{

	setup();
	while (1)
	{

		/************************************************************************************************** 
		Essa parte faz o sensor ultrassonico, para detectar se algum carro chegou ao local
		**************************************************************************************************/
		if (getCM() > 150) //caso o sensor não encontre um carro em menos de 1,5m
		{
			printf("\nESPERANDO...\n\n");
			delay(500);
		}

		else //caso encontre um carro na distância de 1,5m
		{

			/************************************************************************************************** 
			Essa parte tira a foto
			**************************************************************************************************/

			system("fswebcam -d /dev/video0 -r 1280x720 -S 10 -F 5 --no-banner foto.jpg");

			/************************************************************************************************** 
			Essa parte faz o ALPR da foto
			**************************************************************************************************/

			//system("alpr -c br foto.jpg -n 1 > saida.txt");
			system("alpr -c br Placas/1.jpg -n 1 > saida.txt");
			//system("alpr -c br Placas/tracker0.jpg -n 1 > saida.txt");

			/************************************************************************************************** 
			Essa parte trata a saída do ALPR, para que fique sendo apenas o numero da placa, ignorando os 
			* demais elementos.
			**************************************************************************************************/

			system("tail -1 saida.txt > saidatratada.txt"); //pego somente a ultima linha do arquivo
			int i = 0;
			char lerarq[1000];

			FILE *f = fopen("saidatratada.txt", "r");

			while (!feof(f)) //enquanto não chegar ao fim do arquivo
			{
				fscanf(f, "%s", lerarq);
				if (strcmp(lerarq, "No") == 0) //se encontra "No license" no arquivo
				{
					i = 1;
				}
			}
			if (i != 0)
			{
				printf("\nPlaca não identificada\n");
				//Se não houver placa identificada na foto, termina a execução do programa.
			}
			else
			{
				fseek(f, 6, SEEK_SET); // determino a posição que ele vavi começar a ler no arquivo
				char part[10] = { 0 }; //determino o tamanho da string
				fread(part, 1, 12, f); // lê 12 itens do tamanho de 1 caracteredo arquivo para part	

				FILE *s = fopen("out.txt", "w");
				fprintf(s, "%s", part);
				fclose(s);

				/************************************************************************************************** 
				Essa parte corrige o numero da placa caso haja alguma confusão no ALPR (Ex.: 0 com O, 8 com B...) 
				**************************************************************************************************/

				FILE *fp = fopen("out.txt", "r+");
				char array[100];
				fscanf(fp, "%s", array);
				int counter = 0;

				while (array[counter] != '\0')
				{
					if (counter < 3) //substituo as letras confundidas com números
					{
						if (array[counter] == '0')
						{
							array[counter] = 'O';
						}
						if (array[counter] == '1')
						{
							array[counter] = 'I';
						}
						if (array[counter] == '2')
						{
							array[counter] = 'Z';
						}
						if (array[counter] == '4')
						{
							array[counter] = 'A';
						}
						if (array[counter] == '5')
						{
							array[counter] = 'S';
						}
						if (array[counter] == '8')
						{
							array[counter] = 'B';
						}
					}

					else //substituo os numeros confundidss com letras
					{
						if (array[counter] == 'O')
						{
							array[counter] = '0';
						}
						if (array[counter] == 'Q')
						{
							array[counter] = '0';
						}
						if (array[counter] == 'D')
						{
							array[counter] = '0';
						}
						if (array[counter] == 'I')
						{
							array[counter] = '1';
						}
						if (array[counter] == 'Z')
						{
							array[counter] = '2';
						}
						if (array[counter] == 'A')
						{
							array[counter] = '4';
						}
						if (array[counter] == 'S')
						{
							array[counter] = '5';
						}
						if (array[counter] == 'B')
						{
							array[counter] = '8';
						}
					}
					counter++;
				}

				fprintf(fp, "%s", array);
				fclose(fp);

				FILE *f2 = fopen("out.txt", "r");
				fseek(f2, 7, SEEK_SET);
				char part2[10] = { 0 };
				fread(part2, 1, 7, f2);

				FILE *s2 = fopen("out1.txt", "w");
				fprintf(s2, "%s", part2);
				fclose(s2);

				/************************************************************************************************** 
				Essa parte confere se o carro entrando no condomínio pertence a um morador ou não 
				**************************************************************************************************/

				int n = 0;
				char arq_txt[10000];
				char status[20];

				FILE *db = fopen("placasregistradas.txt", "r");

				if (db == NULL)
				{
					printf("Não foi possível acessar o arquivo\n");
					exit(-1);
				}

				while (!feof(db)) //enquanto não chegar ao fim do arquivo
				{
					fscanf(db, "%s", arq_txt);
					if (strcmp(arq_txt, part2) == 0) //se encontra uma placa igual no texto
						n++;
				}

				if (n == 0)
				{
					printf("\n\nVISITANTE\n\n");
				}

				else
				{
					printf("\n\nMORADOR!\n\n");
				}

				/************************************************************************************************** 
				Essa parte salva a data atual numa string
				**************************************************************************************************/

				time_t raw;
				time(&raw);

				struct tm *time_ptr;
				time_ptr = localtime(&raw);

				char date[11];
				strftime(date, 11, "%d-%m-%Y", time_ptr);

				/************************************************************************************************** 
				Essa parte cria o banco de dados, ou seja, salva a foto cujo nome é a propria placa numa pasta do 
				dia atual 
				**************************************************************************************************/

				int ch;
				FILE *mv1, *mv2;
				char dest[MAX];
				char extensao[10] = ".jpg";
				char placafinal[10];

				strcpy(placafinal, part2);
				strcat(part2, extensao);
				mv1 = fopen("foto.jpg", "r"); //abre o arquivo de origem da foto para leitura	

				chdir("Fotos/");
				char cmd[100]; //cria a pasta com o nome sendo a data atual
				strcpy(cmd, "mkdir ");
				strcat(cmd, date);
				strcat(cmd, "/");

				system(cmd);
				chdir(date);

				mv2 = fopen(part2, "w"); //abre o arquivo de destino da foto para escrita	

				//copia o arquivo da origem para o destino
				while (!feof(mv1))
				{
					ch = fgetc(mv1);
					fputc(ch, mv2);
				}

				//fecha os arquivos e volta pra pasta de trabalho */
				fclose(mv1);
				fclose(mv2);
				chdir("..");
				chdir("..");

				/************************************************************************************************** 
				Essa parte cria a planilha no Google Spreadsheets para melhor identificação por parte do cliente
				**************************************************************************************************/

				char faz_planilha[300] = "curl https://docs.google.com/forms/d/1y-zQ3rgFStE4fzM7xMzqd6ODabnfis54RCHbdOdIjRo/formResponse -d ifq -d \"entry.2131156253=";
				char faz_planilha1[50] = "\" -d \"entry.257414832=";
				char faz_planilha2[50] = "\" -d submit=Submit";

				strcat(faz_planilha, placafinal);
				strcat(faz_planilha, faz_planilha1);

				if (n == 0)
				{
					strcpy(status, "VISITANTE");
				}
				else
				{
					strcpy(status, "MORADOR");
				}

				strcat(faz_planilha, status);
				strcat(faz_planilha, faz_planilha2);

				system(faz_planilha);
				delay(10000); //espera 20s
			}
		}
	}
}

