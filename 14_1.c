#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

void proc_info(const char * procname) {//Parent Process ID(PPID), возвращает идентификатор группы процессов, к которой принадлежит процесс(getpgid)
    printf("%s: PID %d, PPID %d, PGID %d, SID %d\n", procname, getpid(), getppid(), getpgid(0), getsid(0));
}

int main(void) {
    pid_t child_id = fork();//fork - создает новый процесс, а pid_t - тип данных для ID проесса
    if (child_id < 0) {//провекра, что процесс создался правильно, без ошибок
        perror("fork");
        return 1;
    }
    //это выполняется лишь для процесса child
    if (child_id == 0) {//поток вывода называется "стандартным выводом (standard output)" - stdout, stderr - поток сообщений об ошибках называется от standard error, fileno пределяет дескриптор файла открытого потока данных по указателю
        if (dup2(fileno(stderr), fileno(stdout)) < 0) {//dup2 делает newfd копией oldfd, закрывая newfd, если требуется
            perror("dup2");//ошибка, dup2 не сработал, так как dup2 возвращают новый дескриптор или -1, если произошла ошибка
        }
        proc_info("child");//выводим информацию о процессе
        return 0;
    }
    
    //выполняется лишь для процесса родителя
    proc_info("parent");
    wait(NULL);

    return 0;
}
