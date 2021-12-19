#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>

void proc_info(const char * procname) {//Parent Process ID(PPID), возвращает идентификатор группы процессов, к которой принадлежит процесс(getpgid)
    printf("%s: PID %d, PPID %d, PGID %d, SID %d\n", procname, getpid(), getppid(), getpgid(0), getsid(0));
}

int main(void) {
    pid_t child_id = fork();//fork - создает новый процесс, а pid_t - тип данных для ID проесса
    if (child_id < 0) {//провекра, что процесс создался, иначе выводятся ошибка
        perror("fork");
        return 1;
    }
    //это выполняется лишь для процесса child
    if (child_id == 0) {//поток вывода называется "стандартным выводом (standard output)" - stdout, stderr - поток сообщений об ошибках называется от standard error, fileno пределяет дескриптор файла открытого потока данных по указател
        //if (dup2(fileno(stderr), fileno(stdout)) < 0) {//dup2 делает newfd копией oldfd, закрывая newfd, если требуется
        int fd = open("a.txt",O_WRONLY | O_CREAT | O_TRUNC, 0600);
        
        if (fd < 0)
        {
                perror("File cannot be opened for writing"); //вывод в поток ошибок
                return 2;
        }

        if (dup2(fd, fileno(stdout)) < 0) {  
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
