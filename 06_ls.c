#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <stdlib.h>
#include <errno.h>

//в этой функции мы определяем тип файла, более конкретное описание
char stattype(unsigned mode) 
{
    switch (mode & S_IFMT) 
    {
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

//тоже описание типов файла
char dirent_type(unsigned d_type) 
{
    switch (d_type) 
    {
        case DT_BLK:  return 'b';
        case DT_CHR:  return 'c';
        case DT_DIR:  return 'd';
        case DT_FIFO: return 'p';
        case DT_LNK:  return 'l';
        case DT_REG:  return '-';
        case DT_SOCK: return 's';
        default:      return '?';
    }
}

int main(int argc, char const *argv[]) 
{

    if(argc > 1)//проверка на корректное число аргументов 
    {
        printf("Too many arguments\n");
        printf("Usage: %s\n", argv[0]);
        return 1;
    }

    DIR * dir_fd = opendir(".");//мы открываем директорию и сразу обрабатываем ошибку при открытии
    if (dir_fd == NULL) 
    {
        perror("opendir");
        return 2;
    }

    while(1)//тут мы открываем каждый файл из директории 
    {
	errno = 0;
	struct dirent *entry = readdir(dir_fd);
	if (entry == NULL) 
	{
	    if (errno == 0)
                break;
	    perror("readdir");
	    closedir(dir_fd);
	    return 3;
	}

	char entry_type = dirent_type(entry->d_type);//определяем тип директории, структура задана указателем, поэтому и ставится ->, если была бы переменная, то было бы entry.d_type
	
	if (entry_type == '?')//entry_type не подробный, поэтому lstat вызываем, чтобы получить более подробную информацию 
	{
	    struct stat st;
	    if (lstat(entry->d_name, &st) == 0) 
	    {
		entry_type = stattype(st.st_mode);
	    }
	    else 
	    {
	        perror("error in lstat");
		entry_type = '!';
	    }
	}
 
        printf("%c %s\n", entry_type, entry->d_name);     
    }

    if(closedir(dir_fd) == -1)//ошибка при закрытии директории
    {
        perror("closedir");
        return 4;
    }

    return 0;
}


