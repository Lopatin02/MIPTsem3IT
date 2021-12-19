//пишем программу клиент(mq_send):
//Функции:
//-открывает очередь
//-посылает в очередь сообщение
//-закрывает очередь
//В сервере надо использовать pipe(пайп) - потоковая передача, в каком порядке быди записаны, в таком и прочитаются
//
//Также нужно написать сервер(вторая программа)(mq_receive)
//Функции:
//-создается очередь сообщений. Читаем (mq_receive) в бесконечном цикле каждое сообщение(не забыть что вреся, когда пришло, тоже запоминать)
//-при посылке сигнала, очередь удаляется

#include <stdio.h>
#include <fcntl.h>           /* For O_* constants */
#include <sys/stat.h>        /* For mode constants */
#include <mqueue.h>
#include <time.h>
#include <string.h>

int main(int argc, char const *argv[]) {

    if(argc != 3) {//проверка на корректность входа, если он неверный, тогда выводится ошибка
        printf("Usage: path /queue_name message\n");
        return 1;
    }

    int res = 0;

    //Создаем очередь
    mqd_t our_queue = mq_open(argv[1], O_WRONLY);//mq_open() создает новую очередь сообщений POSIX() или открывает существующую очередь
    //Очереди сообщений создаются и открываются с помощью mq_open. Эта
    //функция возвращает дескриптор очереди сообщений (mqd_t), который
    //используется для ссылки на открытую очередь сообщений в последующих вызовах.
    //очередь распознается по имени(argv[1])
    //O_WRONLY Открываем очередь только для отправки сообщений
    if(our_queue == (mqd_t)-1) 
    {
        perror("mq_open");//если возникает ошибка, тогда mq_open() будет выдавать (mqd_t)-1
        return 2;
    }

    //Выводим сообщение
    if(mq_send(our_queue, argv[2], strlen(argv[2]), 0) < 0)//Сообщение добавляется, на которое msg_ptr(message pointer, в нашем случае это argv[2]) указывает, к сообщению
    //strlen(argv[2]) - длина сообщения
    //Это сообщение добавляется в очередь сообщений, на которую ссылается дескриптор mqdes(в нашем случае это our_queue) 
    //The msg_prio(0) argument is a nonnegative(неотрицательный) integer that specifies the
    //priority of this message.  Messages are placed on the queue in
    //decreasing order of priority, with newer messages of the same
    //priority being placed after older messages with the same
    //priority
    {
        perror("failed to send message");//ошибка поссылки сообщения
        res = 3;
    }

    //mq_close() закрывает дескриптор очереди сообщений mqdes(our_queue)
    if(mq_close(our_queue) == -1)
    {
        perror("mq_close");//при ошибки с закрытием
        res = 4;
    }

    return res;
}
