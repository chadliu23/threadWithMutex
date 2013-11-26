#include <iostream>
#include <pthread.h>
#include <unistd.h>
using namespace std;

pthread_mutex_t count_mutex;
pthread_cond_t count_threshold_30;
pthread_cond_t count_threshold_10;
int i =0;
void* threadA(void *t) 
{
	while(true)
	{

		if(i <= 50)
		{

		pthread_mutex_lock(&count_mutex);
			++i;
			cout <<"threadA i+1:" <<i <<endl;

		pthread_mutex_unlock(&count_mutex);
		}
		if(i >10)
		{
			pthread_cond_signal(&count_threshold_10);
		}
		if(i >30)
		{
			pthread_cond_signal(&count_threshold_30);
		}
		usleep(900 * 1000);
	}
}
void* threadB(void *t)
{
	while(true)
	{
		if(i <= 50)
		{
			pthread_mutex_lock(&count_mutex);
			i+=2;
			cout <<"threadB i+2:" <<i <<endl;
			pthread_mutex_unlock(&count_mutex);
		}
		usleep(900 * 1000);
	}
}

void* threadC(void *t)
{
	while(true)
	{
		pthread_mutex_lock(&count_mutex);
		pthread_cond_wait(&count_threshold_10, &count_mutex);
		--i;
		cout <<"threadC i-1:" <<i <<endl;
		pthread_mutex_unlock(&count_mutex);
	}
}


void* threadD(void *t)
{
	while(true)
	{
		pthread_mutex_lock(&count_mutex);
		pthread_cond_wait(&count_threshold_30, &count_mutex);
		i-=3;
		cout <<"threadD i-3:" <<i <<endl;
		pthread_mutex_unlock(&count_mutex);
	}
}

int main() {
	pthread_t threads[4];

    pthread_mutex_init(&count_mutex, NULL);

  	pthread_cond_init (&count_threshold_10, NULL);
  	pthread_cond_init (&count_threshold_30, NULL);
    
    pthread_create(&threads[0], NULL, &threadA, NULL);
    pthread_create(&threads[1], NULL, &threadB, NULL);
    pthread_create(&threads[2], NULL, &threadC, NULL);
    pthread_create(&threads[3], NULL, &threadD, NULL);

    for (i=0; i<4; i++) {
    	pthread_join(threads[i], NULL);
  	}
    // :~

    return 0;
}