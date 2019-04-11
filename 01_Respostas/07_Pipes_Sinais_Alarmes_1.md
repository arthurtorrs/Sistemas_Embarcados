1. Quantos pipes serão criados após as linhas de código a seguir? Por quê?

(a)
```C
int pid;
int fd[2];
pipe(fd);
pid = fork();
```
	1 pipe criado, porque ele é criado antes do fork(), logo é compartilhado entre o pai e o filho.


(b)
```C
int pid;
int fd[2];
pid = fork();
pipe(fd);
```
	2 pipes criados, pois é criado um pipe para cada processo após o fork().


2. Apresente mais cinco sinais importantes do ambiente Unix, além do `SIGSEGV`, `SIGUSR1`, `SIGUSR2`, `SIGALRM` e `SIGINT`. Quais são suas características e utilidades?

	`SIGSEGV`: emitido em caso de violação da segmentação, ou seja, tentativa de acesso a um dado fora do domínio de endereçamento do processo.
	`SIGUSR1`: primeiro sinal disponível ao usuário (utilizado para comunicação entre processos)
	`SIGUSR2`: outro sinal disponível ao usuário (utilizado para comunicação interprocessual)
	`SIGALRM`: emitido quando o relógio de um processo pára. O relógio é colocda em funcionamento usando alarm().
	`SIGINT`: sinal emitido aos processos do terminal quando as teclas de interrupção do teclado são acionadas (ex.: CTRL+C).

	`SIGQUIT`: sinal emitido aos processos do terminal quando a tecla de abandono do teclado são acionadas.
	`SIGILL`: emitido quando uma instrução ilegal é detectada.
	`SIGIOT`: emitido em casos de problema de hardware
	`SISSYS`: argumento de uma chamada de sistema
	`SIGPWR`: reativação sobre pane elétrica 


3. Considere o código a seguir:

```C
#include <signal.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

void tratamento_alarme(int sig)
{
	system("date");
	alarm(1);
}

int main()
{
	signal(SIGALRM, tratamento_alarme);
	alarm(1);
	printf("Aperte CTRL+C para acabar:\n");
	while(1);
	return 0;
}
```

Sabendo que a função `alarm()` tem como entrada a quantidade de segundos para terminar a contagem, quão precisos são os alarmes criados neste código? De onde vem a imprecisão? Este é um método confiável para desenvolver aplicações em tempo real?

	Não se deve contar com o sinal chegando precisamente, pois em um ambiente de multiprocessamento há tipicamente uma certa quantidade de delay involvida.
