//Цель задачи: скопировать данные из одного файла в другой
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <stdint.h>
#define BUF_SIZE 1048576

ssize_t write_all(int fd, const void *buf, size_t count) {//создаем эту функцию, так как у write partial success и не се может записаться
    /* давайте мы будет вызывать write, пока не возникнет ошибка, либо пока все не запишем*/
    size_t bytes_written = 0;
    while(bytes_written < count) {
        ssize_t res = write(fd, buf + bytes_written, count - bytes_written);
        if(res < 0) {
            return res;
        }
        bytes_written += (size_t)res;
    }

    return (ssize_t)bytes_written;
}


int copy_file(int src_fd, int dest_fd) {//src_fd - откуда копируем, dest_fd - куда копировать
    ssize_t bytes = 0;
    char* data = (char*)malloc(BUF_SIZE);//data - выдел память(читаем блоками)
    if(!data) {//malloc не ошибся	
	perror("Failed memory allocation");
        return 6;
    }
    
    int res = 0;

    while(1) {
        bytes = read(src_fd, data, sizeof(data));

        if(bytes == -1) {//проверка, что read не ошибся	                
	    perror("File reading error");
            res = 7;
	    break;
        }

        if(bytes == 0) {// выход из цикла
            break;
        }

        bytes = write_all(dest_fd, data, (size_t)bytes);//записываем в write_all, bytes(число байтов), data - указатель на начало блока, в который считали

        if(bytes < 0) {//если write ошибся
            perror("File writing error");  
            res = 8;
	    break;
        }
    }

    free(data);
    return res;
}


int main(int argc, char* argv[]) {
    int res = 0;

    if(argc != 3) {//проверка, что передали верное кол-во аргументов
        fprintf(stderr, "Usage: %s path text\n", argv[0]);
        return 1;
    }

    int in_fd = open(argv[1], O_RDONLY);
    if (in_fd == -1) {
        perror("File open error");
        return 2;
    }

    int out_fd = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (out_fd == -1) {
        perror("File open error");
        close(in_fd);
        return 3;
    }
    if(copy_file(in_fd, out_fd) != 0) {
        fprintf(stderr, "Failed to copy to %s from %s", argv[1], argv[2]);
        res = 4;
    }
    if (close(in_fd)) {
        perror("File close error");
        res = 5;
    }
    if (close(out_fd)) {
        perror("File close error");
        res = 6;
    }
    return res;
}
