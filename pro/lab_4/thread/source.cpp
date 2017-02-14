#include <unistd.h>
#include <semaphore.h>
#include <pthread.h>
#include <iostream>

#define MAX_BUFFER_SIZE 10

using namespace std;

//ofstream cout("out.log");

pthread_t thread_1;
pthread_t thread_2;
pthread_t thread_3;
pthread_t thread_4;
pthread_t thread_5;

sem_t SCR1, sem_1, sem_2;
pthread_mutex_t MCR1 =  PTHREAD_MUTEX_INITIALIZER;

int data = 0;
int common_resource[MAX_BUFFER_SIZE];
int next_in = 0, next_out = 0;
int passageway = 0;


void* producer(void *_id){
    int id = *(int*)_id;
    int sem_value;

    while(true){

        if(id == 5){
            sem_post(&sem_2);
            sem_wait(&sem_1);
        }

        sem_getvalue(&SCR1, &sem_value);
        //if (sem_value == 0) break;
        if(passageway == 0 && sem_value == MAX_BUFFER_SIZE) passageway++;
        else if(passageway == 1 && sem_value == 0) passageway++;
        else if(passageway == 2 && sem_value == MAX_BUFFER_SIZE) passageway++;
        else if(passageway == 3 && sem_value == 0) break;

        if(sem_value < MAX_BUFFER_SIZE)
        if (pthread_mutex_trylock(&MCR1) == 0){
            common_resource[next_in] = data++;
            next_in = (next_in + 1) % MAX_BUFFER_SIZE;
            cout << "Producer thread " << id << ": semaphore = " << sem_value+1 << "; element " << data - 1 << " CREATED;\n";
            pthread_mutex_unlock(&MCR1);
            sem_post(&SCR1);
        }
        else {
            //cout << "Producer thread " << id << " does some useful work\n";
        }

    }
    cout << "Producer thread " << id << " stopped!!!\n";

    pthread_cancel(thread_2);
    pthread_cancel(thread_3);
    pthread_cancel(thread_4);
    if (id == 1)
        pthread_cancel(thread_5);

    return NULL;
}


void* consumer(void *_id){
    int id = *(int*)_id;
    int sem_value;

    while(true){
        if(id == 4){
            sem_post(&sem_1);
            sem_wait(&sem_2);
        }

        sem_getvalue(&SCR1, &sem_value);
        if (passageway == 3 && sem_value == 0) break;

        if(sem_trywait(&SCR1) == 0)
            if( pthread_mutex_trylock(&MCR1) == 0) {
                sem_getvalue(&SCR1, &sem_value);
                int value = common_resource[next_out];
                next_out = (next_out + 1) % MAX_BUFFER_SIZE;
                cout << "Consumer thread " << id << ": semaphore = " << sem_value << "; element " << value << " DELETED;\n";
                pthread_mutex_unlock(&MCR1);
            }
            else {
                sem_post(&SCR1);
                //cout << "Consumer thread " << id << " does some useful work\n";
            }
        else{
            //cout << "Consumer thread " << id << " does some useful work\n";
        }
        usleep(2);
    }

    cout << "Consumer thread " << id << " stopped!!!\n";
    return NULL;
}


int main(){
    sem_init(&SCR1, 0, 0);
    sem_init(&sem_1, 0, 0);
    sem_init(&sem_2, 0, 0);

    int id_thread_1 = 1;
    int id_thread_2 = 2;
    int id_thread_3 = 3;
    int id_thread_4 = 4;
    int id_thread_5 = 5;

    for(; data < 5; ++data) {
        common_resource[data] = data;
        sem_post(&SCR1);
        ++next_in;
    }
    cout << "Queue with elements from 0-th to " << 9 << "-th has been created!!!\n";
    int sem_value;
    sem_getvalue(&SCR1,&sem_value);
    cout << "semaphore = " << sem_value << endl;


    pthread_create(&thread_1,NULL,&producer,(void*)&id_thread_1);
    pthread_create(&thread_2,NULL,&consumer,(void*)&id_thread_2);
    pthread_create(&thread_3,NULL,&consumer,(void*)&id_thread_3);
    pthread_create(&thread_4,NULL,&consumer,(void*)&id_thread_4);
    pthread_create(&thread_5,NULL,&producer,(void*)&id_thread_5);

    pthread_join(thread_1, NULL);
    pthread_join(thread_2, NULL);
    pthread_join(thread_3, NULL);
    pthread_join(thread_4, NULL);
    pthread_join(thread_5, NULL);

    return 0;
}
