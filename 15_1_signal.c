//kill -9  3114 - завершает программу
//надо написать программу, которая будет сопротивляться своему завершению
#include <unistd.h>
#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

volatile int g_last_signal;//информирует компилятор, что значение переменной может меняться извне. Это может произойти под управлением операционной системы, аппаратных средств или другого потока. Поскольку значение может измениться, компилятор каждый раз загружает его из памяти

void proc_info(void) 
{//информацию о процессе выдаем
    printf("PID %d, PPID %d, PGID %d, SID %d, UID %d, GID %d\n", getpid(), getppid(), getpgrp(), getsid(0), getuid(), getgid());
}

void sig_handler(int s_number) 
{
    g_last_signal = s_number;
}

int main(void) 
{

    proc_info();
    
    for(int i = 0; i < NSIG; i++) 
    {
        //signal(i, sig_handler);
	struct sigaction s_act;//Системный вызов sigaction используется для изменения действий процесса при получении соответствующего сигнала.
        memset(&s_act, 0, sizeof(s_act));
        sigfillset(&s_act.sa_mask);
        s_act.sa_flags = SA_RESTART;
        s_act.sa_handler = sig_handler;
        if (sigaction(i, &s_act, NULL)){
           perror("sigaction");
        }
    }

    while(1) 
    {
        pause();
        printf("last_signal = %d (%s)\n", g_last_signal, strsignal(g_last_signal));
    }

    return 0;
}
