1. Crie um programa em C que cria um processo-filho e um pipe de comunicação. Faça com que o processo-pai envie os valores 1, 2, 3, 4, 5, 6, 7, 8, 9 e 10 para o processo-filho, com intervalos de 1 segundo entre cada envio. Depois de o processo-pai enviar o número 10, ele aguarda 1 segundo e termina a execução. O processo-filho escreve na tela cada valor recebido, e quando ele receber o valor 10, ele termina a execução.

	#include <stdio.h>
	#include <stdlib.h>
	#include <signal.h>
	#include <unistd.h>
	#include <string.h>
	#include <sys/types.h>
	#include <sys/wait.h>	

	int main()
	{
		int pid, f=0, p=0;	// PID do processo filho
		int fd[2];		// Descritores do pipe
		
		pipe(fd);		// Cria o pipe
		pid = fork();		// Cria o processo
		
		// Codigo do filho
		if(pid == 0)
		{
			for(f=0; f<11; f++)
			{
			read(fd[0], &f, sizeof(int));
			printf("\n%d", f);
			}
		}
			
		// Codigo do pai
		else
		{
			for(p=0; p<11; p++)
			{
	 		write(fd[1], &p, sizeof(int));
			}
		}
	wait(NULL);
	return 0;
	} 

2. Crie um programa em C que cria um processo-filho e um pipe de comunicação. Utilize o pipe para executar a seguinte conversa entre processos:

```
FILHO: Pai, qual é a verdadeira essência da sabedoria?
PAI: Não façais nada violento, praticai somente aquilo que é justo e equilibrado.
FILHO: Mas até uma criança de três anos sabe disso!
PAI: Sim, mas é uma coisa difícil de ser praticada até mesmo por um velho como eu...
```

Neste exercício, quem recebe a mensagem via pipe é quem as escreve no terminal.

	#include <stdio.h>
	#include <stdlib.h>
	#include <signal.h>
	#include <unistd.h>
	#include <string.h>
	#include <sys/wait.h>
	
	#define N 100
	
	int main()
	{
		int pid;
		int fd[2];
		pipe(fd);
		pid = fork();
		// Codigo do filho
		if(pid == 0)
		{
			char buffer_filho[N], msg_filho[N];
			sprintf(msg_filho, "Pai, qual é a verdadeira essência da sabedoria?");
			write(fd[1], msg_filho, N);
			sleep(1);
			read(fd[0], buffer_filho, N);
			printf("PAI: %s\n", buffer_filho);
			sprintf(msg_filho, "Mas até uma criança de três anos sabe disso!");
			write(fd[1], msg_filho, N);
			sleep(1);
			read(fd[0], buffer_filho, N);
			printf("PAI: %s\n", buffer_filho);
		}
		// Codigo do pai
		else
		{
			char buffer_pai[N], msg_pai[N];
			read(fd[0], buffer_pai, N);
			printf("FILHO: %s\n", buffer_pai);
			sprintf(msg_pai, "Não façais nada violento, praticai somente aquilo que	é justo e equilibrado.");
			write(fd[1], msg_pai, N);
			sleep(1);
			read(fd[0], buffer_pai, N);
			printf("FILHO: %s\n", buffer_pai);
			sprintf(msg_pai, "Sim, mas é uma coisa difícil de ser praticada até mesmo por um velho como eu...");
			write(fd[1], msg_pai, N);
			wait(NULL);
		}
		return 0;
	}


3. Crie um programa em C que cria dois processos-filhos e um pipe de comunicação. Utilize o pipe para executar a seguinte conversa entre processos:

```
FILHO1: Quando o vento passa, é a bandeira que se move.
FILHO2: Não, é o vento que se move.
PAI: Os dois se enganam. É a mente dos senhores que se move.
```

Neste exercício, quem recebe a mensagem via pipe é quem as escreve no terminal.

	#include <stdio.h>
	#include <stdlib.h>
	#include <unistd.h>
	#include <signal.h>
	#include <sys/signal.h>
	#include <errno.h>

	int main() {
		int pid;	// PIDs dos processos
		int fd[2];	// Descritores do pipe
		char mensagem[100];
		char mensagem_1[]= "FILHO1: Quando o vento passa, é a bandeira que se move.";
		char mensagem_2[]= "FILHO2: Não, é o vento que se move.";
		char mensagem_3[]= "PAI: Os dois se enganam. É a mente dos senhores que se 	move.";
		// Cria o pipe
		pipe(fd);
		// Cria o processo
		pid = fork();
		// Codigo do FILHO_1
		if(pid == 0)
		{
			if (write(fd[1], mensagem_1, sizeof(mensagem_1)) < 0) //Escrita do filho 1 p/ 2
				printf("\nErro na escrita do pipe\n");
			sleep(2);
	
			if (write(fd[1], mensagem_1, sizeof(mensagem_1)) < 0) //Escrita do filho 1 p/ pai
				printf("\nErro na escrita do pipe\n");
			sleep(3);
	
			if(read(fd[0], mensagem, 100) < 0) //Leitura da mensagem do filho 2
				printf("\nErro na leitura do pipe\n");
			else
				printf("\nFILHO 1 leu: %s\n", mensagem);
			sleep(2);
	
			if(read(fd[0], mensagem, 100) < 0) //Leitura da mensagem do pai
				printf("\nErro na leitura do pipe\n");
			else
				printf("\nFILHO 1 leu: %s\n", mensagem);
		sleep(2);
		}
		else
		{
			pid = fork();
			// Codigo do FILHO 2
			if(pid == 0)
			{	
			sleep(1);
	
			if(read(fd[0], mensagem, 100) < 0) //Leitura da mensagem do filho 1
				printf("\nErro na leitura do pipe\n");
			else
				printf("\nFILHO 2 leu: %s\n", mensagem);
			sleep(2);
	
			if (write(fd[1], mensagem_2, sizeof(mensagem_2)) < 0) //Escrita de filho 2 p/ 1
				printf("\nErro na escrita do pipe\n");
			sleep(2);
	
			if (write(fd[1], mensagem_2, sizeof(mensagem_2)) < 0) //Escrita de filho 2 p/ pai
				printf("\nErro na escrita do pipe\n");
			sleep(4);
	
			if(read(fd[0], mensagem, 100) < 0) // Leitura da mensagem do pai
				printf("\nErro na leitura do pipe\n");
			else
				printf("\nFILHO 2 leu: %s\n", mensagem);
			sleep(2);
			}
			
			// Codigo do pai
			else
			{
			sleep(3);
	
			if(read(fd[0], mensagem, 100) < 0) //Leitura da mensagem do filho 1
				printf("\nErro na leitura do pipe\n");
			else
				printf("\nPAI leu: %s\n", mensagem);
	
			sleep(3);
	
			if(read(fd[0], mensagem, 100) < 0) //Leitura da mensagem do filho 2
				printf("\nErro na leitura do pipe\n");
			else
				printf("\nPAI leu: %s\n", mensagem);
			}
	
			if (write(fd[1], mensagem_3, sizeof(mensagem_3)) < 0) //Escrita de pai p/ filho 1
				printf("\nErro na escrita do pipe\n");
			sleep(2);
			
			if (write(fd[1], mensagem_3, sizeof(mensagem_3)) < 0) //Escrita de pai p/ filho 2
				printf("\nErro na escrita do pipe\n");
			
		sleep(2);
		}
		return 0;
	}	
	
4. Crie um programa em C que cria um processo-filho e um pipe de comunicação. O processo-filho deverá pedir o nome do usuário, envia-lo para o pai via pipe, e o pai deverá escrever o nome do usuário no terminal.

	#include <stdio.h>
	#include <stdlib.h>
	#include <unistd.h>
	#include <signal.h>
	#include <sys/signal.h>
	#include <errno.h>
	
	int main() {
		int pid, pid_pai;	// PIDs dos processos
		int fd[2];		// Descritores do pipe
		char nome_usuario[100];
		// Cria o pipe
		pipe(fd);
		// Cria o processo
		pid = fork();
		// Codigo do filho
		if(pid == 0)
		{
			//Pergunta o nome
			printf("\nDiga seu nome: ");
			scanf("%s", nome_usuario);
			if (write(fd[1], nome_usuario, sizeof(nome_usuario)) < 0)
				printf("Erro na escrita do pipe\n");
			sleep(2);
		}
		//Codigo do pai	
		else
		{
			sleep(2);
		
			//Leitura da mensagem do filho 
			if(read(fd[0], nome_usuario, 100) < 0) 
				printf("Erro na leitura do pipe\n");
			else
				printf("Nome lido: %s\n", nome_usuario);
			sleep(2);
		}
		return 0;
	}

5. Utilize o sinal de alarme para chamar a cada segundo o comando `ps` ordenando todos os processos de acordo com o uso da CPU. Ou seja, seu código atualizará a lista de processos a cada segundo. Além disso, o código deverá tratar o sinal do CTRL-C, escrevendo "Processo terminado!" na tela antes de terminar a execução do processo.

	#include <signal.h>
	#include <unistd.h>
	#include <stdio.h>
	#include <stdlib.h>
	
	void tratamento_alarme(int sig)
	{
		system("ps aux k-pcpu");
		// system("date +%s%3N");
		alarm(1);
	}
	
	void sig_handler(int signo)
	{
		if (signo == SIGINT)
		printf("Processo terminado!\n");
		exit(0);
	}
	
	int main()
	{
		signal(SIGALRM, tratamento_alarme);
		alarm(1);
		printf("Aperte CTRL+C para acabar:\n");
		signal(SIGINT, sig_handler);	
		while(1);
		return 0;
	}

