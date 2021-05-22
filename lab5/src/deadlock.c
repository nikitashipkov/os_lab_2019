#include <getopt.h>
#include <errno.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


pthread_mutex_t mutex1 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex2 = PTHREAD_MUTEX_INITIALIZER;


void *dosomework_1(void *param) {
   pthread_mutex_lock(&mutex1);
   pthread_mutex_lock(&mutex2);
   /**
   * Do some work */
   pthread_mutex_unlock(& mutex2);
   pthread_mutex_unlock(& mutex1);
   pthread_exit(0); } /*
   thread2 runs in this function
   */ void *dosomework_2(void *param) {
      pthread_mutex_lock(&mutex2);
      pthread_mutex_lock(&mutex1);
   /**
   * Do some work */
   pthread_mutex_unlock(&mutex1);
   pthread_mutex_unlock(&mutex2);
   pthread_exit(0);
}



int main()
{
   pthread_t thread1;
    pthread_t thread2;

    if (pthread_create(&thread1, NULL, (void *)dosomework_1,
                    NULL) != 0) {
        perror("pthread_create");
        exit(1);
    }
    if (pthread_create(&thread2, NULL, (void *)dosomework_2,
                    NULL) != 0) {
        perror("pthread_create");
        exit(1);
    }

    if (pthread_join(thread1, NULL) != 0) {
        perror("pthread_join");
        exit(1);
    }

    if (pthread_join(thread2, NULL) != 0) {
        perror("pthread_join");
        exit(1);
    }

    return 0;
}
