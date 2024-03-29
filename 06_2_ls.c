#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <stdlib.h>
#include <unistd.h>

//тип файл определяем
char stattype(unsigned mode) {
    switch (mode & S_IFMT) {
        case S_IFBLK:  return 'b';
        case S_IFCHR:  return 'c';
        case S_IFDIR:  return 'd';
        case S_IFIFO:  return 'p';
        case S_IFLNK:  return 'l';   
        case S_IFREG:  return '-';
        case S_IFSOCK: return 's';
        default:       return '?';
    }
}



int main(int argc, char const *argv[]) {
    //проверка на кол-во аргументов на входе
    if(argc > 2) {
        printf("Too many arguments\n");
        printf("Usage: %s [path]\n", argv[0]);
        return 1;
    }

    DIR * dir_fd;

    //comments
    if(argc == 2) {
        if(chdir(argv[1]) == -1) {//Вывод имени текущего каталога или переход в другую папку, изменяет текущий каталог каталог на path - chdir
            perror("chdir");
            return 2;
        }
    } 

    dir_fd = opendir(".");

    if (dir_fd == NULL) {//обработка ошибки при открытии директории     
        perror("opendir");
        return 2;
    }

    struct dirent * entry;
    while((entry = readdir(dir_fd)) != NULL) {//в цикле каждый файл директории открываем
        struct stat sb;
        if(lstat(entry->d_name, &sb) == -1) {
            perror ("lstat");
            printf("failed: entry->d_name = %s\n", entry->d_name);
            closedir(dir_fd);
            return 3;

        }                 
        char type = stattype(sb.st_mode); 
        printf("%c %s\n", type, entry->d_name);	
    }

    if(closedir(dir_fd) == -1) {
        perror("closedir");
        return 4;
   }    	
	return 0;
}

