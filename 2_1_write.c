//записать в указанный файл указанное содержимое (строку) open, write, close

#include <sys/types.h> 
#include <sys/stat.h>
#include <fcntl.h> 
#include <stdio.h> 
#include <string.h> 
#include <stdint.h> 
#include <unistd.h> 

ssize_t write_all(int fd, const void *buf, size_t count) //функция, которая все записывает, тк write не все данные может записать
{
	size_t bytes_written = 0; 
	const uint8_t *buf_adder = buf; 
	
//пока сумма прочитанных байтов не станет равна длине строки, читаем
	while (bytes_written < (size_t) count)
	{
		ssize_t res = write(fd, buf_adder + bytes_written, count - bytes_written); 
		if(res < 0) 
			return res; 
		bytes_written += (size_t)res; 
			
	}
	return (ssize_t)bytes_written; 
}

int main(int argc, char * argv[]) 
{
	int res = 0;

	//Выход с ошибкой, если неправильный ввод данных
	if (argc != 3) 
	{
		fprintf(stderr, "Usage: %s path text", argv[0]); 
		return 1; 
	}

	//Создание переменной fd
	int fd = open(argv[1], O_WRONLY | O_CREAT | O_TRUNC, 0644); //Если файл не открылся для записи, тогда fd = -1; если файла нет, тогда мы создаём, если в файле есть содержимое, тогда стираем его

	//Выход с ошибкой, если файл не окрылся для чтения
	if (fd < 0) 
	{
		perror("File cannot be opened for writing"); //вывод в поток ошибок
		return 2;
	}

	//Выход с ошибкой, если  ошибка в методе write
	if (write_all(fd, argv[2], strlen(argv[2])) < 0)
	{
		perror("Failure during writing"); 
		res = 3;
	}

	//Выход с ошибкой, если файл не закрылся
	if (close(fd) < 0)
	{ 
		perror("Failure during close");
		res = 4; 
	}

	return res;
}
