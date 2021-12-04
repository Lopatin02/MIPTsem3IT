//dup2(oldfd, newfd) -- в дескриптор newfd (первый свободный) мы вставляем копию файла oldfd

#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

void proc_info(const char * procname) 
{
	printf("%s: PID %d, PPID %d, PGID %d, SID %d\n", procname, getpid(), getppid(), getpgid(0), getsid(0));
}

int main(void) 
{
	pid_t child_id = fork();
	
	if (child_id < 0) 
	{
		perror("fork"); //Выход с ошибкой при неверно созданном дочернем процессе
		return 1;
	}
	 
	if (child_id == 0) //выполняется в дочернем процессе 
	{
		//dup2 дублирует файловый дискриптор
		if (dup2(fileno(stderr), fileno(stdout)) < 0) 
		{
			perror("dup2");
		}

		proc_info("child"); //Используем ранее написанную функцию
		return 0;
	}

	//выполняется в родительском процессе
	proc_info("parent");
	wait(NULL);

	return 0;
}

