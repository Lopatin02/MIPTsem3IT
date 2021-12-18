//нам нужно в этой задаче вывести свободное и доступное(для использования) пространство для файловой системы, в которой будет лежать данный файл или каталог

#include <stdio.h>
#include <sys/param.h>
#include <sys/mount.h>
#include <sys/statvfs.h>

int main(int argc, char *argv[])
{
    if (argc != 2)//ввод неверного числа параметров
    {
        printf("Usage: %s <pathname> \n", argv[0]);
        return 1;
    }

    struct statvfs buf;
   //функция statvfs возвращает информацию о смонтированной файловой системе
    
	if (statvfs(argv[1], &buf) == -1)//В случае успеха возвращается ноль. В случае ошибки возвращается -1
    {
        perror("Failed to statfs");//ошибка работы функции statvfs
        return 1;
    }

    printf("Total: %lu\n", buf.f_frsize * buf.f_blocks);//Fragment size - f_frsize, также f_blocks;   /* Size of fs - fragment size in f_frsize units */  
    printf("Available: %lu\n", buf.f_frsize * buf.f_bavail);//Number of free blocks for unprivileged users
    printf("Used: %lu\n", buf.f_frsize * buf.f_blocks - buf.f_frsize * buf.f_bfree);//f_bfree - Number of free blocks
    return 0;
}
