#include <stdio.h>
#include <fcntl.h>           
#include <sys/stat.h>        
#include <mqueue.h>
#include <time.h>
#include <string.h>

int main(void) 
{
    const char * queue_name = "/test-mqueue";
    
    mqd_t queue_fd = mq_open(queue_name, O_RDWR | O_CREAT, 0600, NULL);
    //mq_open() создает новую очередь сообщений POSIX() или открывает существующую очередь
    //Очереди сообщений создаются и открываются с помощью mq_open. Эта
    //функция возвращает дескриптор очереди сообщений (mqd_t), который
    //используется для ссылки на открытую очередь сообщений в последующих вызовах.
    //Oflag аргумент определяет флаги , которые управляют работой вызова. (Определение значений флагов может быть получено в том числе <fcntl.h>)
    //O_RDWR открыть очередь как для отправки, так и для приема сообщений
    //O_CREAT создать очередь сообщений, если она не существует
    if(queue_fd == (mqd_t)-1) 
    {
        perror("mq_open");//если возникает ошибка, тогда mq_open() будет выдавать (mqd_t)-1
        return 1;
    }
    
    int res = 0;
 
    //send test message
    if(mq_send(queue_fd, "hello", strlen("hello"), 0) < 0) 
    //Сообщение добавляется, на которое msg_pt указывает, к сообщению
    //strlen("hello") - длина сообщения
    //Это сообщение добавляется в очередь сообщений, на которую ссылается дескриптор mqdes 
    //The msg_prio(0) argument is a nonnegative(неотрицательный) integer that specifies the
    //priority of this message.  Messages are placed on the queue in
    //decreasing order of priority, with newer messages of the same
    //priority being placed after older messages with the same
    //priority
    {
        perror("failed to send message");
        return 2;
    }

    //получаем и показываем информацию об очереди
    struct mq_attr m_info;
    mq_getattr(queue_fd, &m_info);//соответственно получают и изменяют атрибуты очереди сообщений, на которую ссылается очередь сообщений дескриптор mqdes
    printf("Flags : %ld\n", m_info.mq_flags);
    printf("Max. # of messages on queue : %ld\n", m_info.mq_maxmsg);
    printf("Max. message size (bytes) : %ld\n", m_info.mq_msgsize);
    printf("# of messages currently in queue : %ld\n", m_info.mq_curmsgs);

    //mq_close() закрывает дескриптор очереди сообщений mqdes
    if(mq_close(queue_fd) == -1)
    {
        perror("mq_close");//при ошибки с закрытием
        res = 3;
    }
  
    //mq_unlink() removes the specified message queue name. The
    //message queue name is removed immediately. The queue itself is
    //destroyed once any other processes that have the queue open close
    //their descriptors referring to the queue.
    mq_unlink(queue_name);

    return res;
}
