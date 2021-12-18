#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdint.h>
#include <time.h>
#include <pthread.h>

//Блокировки взаимного исключения (мьютексы) не позволяют нескольким потокам одновременно выполнять критические разделы кода, 
//которые обращаются к совместно используемым данным (то есть мьютексы используются для сериализации выполнения потоков).

typedef struct 
{
	pthread_mutex_t mutex; //создаем мьютекс
	unsigned long long counter;
	int iterations;
} 

work_area_t;

void * thr_body(void * arg) 
{
	work_area_t * w = arg;
	for (int i = 0; i < w -> iterations; i++) 
	{
		pthread_mutex_lock(&w -> mutex); //Объект мьютекса, на который ссылается мьютекс, должен быть заблокирован путем вызова pthread_mutex_lock ()
		w -> counter++;
		pthread_mutex_unlock(&w -> mutex);
	}

	puts("Done (secondary)");
	return NULL;
}

int main(int argc, char * argv[]) 
{
	work_area_t data =
	{
		.mutex = PTHREAD_MUTEX_INITIALIZER,//PTHREAD_MUTEX_INITIALIZER инициализирует мьютекс, установив его значение по умолчанию(то есть настройки - базовые)
		.counter = 0,
		.iterations = (argc > 1) ? atoi(argv[1]) : 1000,//конвертирует начальную часть строки, на которую указывает nptr, в целое число 
	};

	pthread_t secondary_thread_id; //возвращает положительное значение, если ошибка
	if (errno = pthread_create(&secondary_thread_id, NULL, thr_body, &data))//Функция pthread_create () используется для создания нового потока с атрибутами, указанными attr , внутри процесса. 
        //Если attr - ПУСТО ( NULL), используются атрибуты по умолчанию 
	//Функция pthread_create () запускает новый поток в вызывающем процессе. Новая ветка
        //запускает выполнение, вызывая thr_body; data передается как единственный аргумент thr_body
	{
		perror("pthread_create");//если ошибка при создании
		return 1;
	}

	for (int i = 0; i < 10; i++) 
	{
		pthread_mutex_lock(&data.mutex);//блокируем на то, что ссылается data.mutex
		data.counter++;
		pthread_mutex_unlock(&data.mutex);////Объект мьютекса, на который ссылается мьютекс, должен быть разблокирован
	}
	puts("Done (main)");
	//нить исполнения, которая вызвала эту функцию переходит в состояние ожидания до завершения thread
	pthread_join(secondary_thread_id, NULL);

	return 0;
}
