#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#define PM 6
#define CM 10

typedef struct {
	char *na;
	int jo;
}my_args;

int *buffer;//buffer
sem_t mutex;//buffer mutex
sem_t prdn;// production
sem_t empty;//judge buffer  

void show(){
	for(int i = 0;i < PM;i++)
		printf("%d ",buffer[i]);
	printf("\n");
}

void producer(void* args){
	int t = 1;
	while(t <= PM-1){
		
		sem_wait(&empty);
		sem_wait(&mutex);
		printf("%sid:%u\n",((my_args*)args) ->na,(unsigned int)pthread_self());
		printf("%s %d begin:",((my_args*)args) ->na,t);
		show();
		for(int a = 0;a < PM;a++){
			if(buffer[a] == 0){
				buffer[a] = rand()%10+1;
				sem_post(&prdn);
				break;
			}
		}
		printf("%send:     ",((my_args*)args) ->na);
		show();
		sleep(1);
		if(t == PM-1){
			printf("%s EXIT\n",((my_args*)args) ->na);
		}
		printf("\n");
		sem_post(&mutex);
		//sem_post(&prdn);
		t++;
	}
	pthread_exit(NULL);
}




void consumer(void* args){
	int t = 1,a;
	while(t <= CM){
		sem_wait(&prdn);
		sem_wait(&mutex);

		printf("%sid:%u\n",((my_args*)args) ->na,(unsigned int)pthread_self());
		printf("%s %d begin:",((my_args*)args) ->na,t);
		show();

		for(a = 0;a < PM;a++){
			if((buffer[a]%2 == ((my_args*)args) ->jo) && (buffer[a] != 0)){
				buffer[a] = 0;
				sem_post(&empty);
				break;
			}
		}
		if(a == PM)
			sem_post(&prdn);
		printf("%s end:    ",((my_args*)args) ->na);
		show();
		sleep(1);
		if(t == CM)
			printf("%s EXIT\n",((my_args*)args) ->na);
		printf("\n");
		//sem_post(&mutex);
		//if(a < PM)
		//	sem_post(&empty);
		sem_post(&mutex);
		t++;
	}
	pthread_exit(NULL);

}

int main()
{
	pthread_t pa,pb,pc; //producer
	pthread_t ca,cb;    //consumer
	srand((int)time(NULL));
	
	buffer = (int*)malloc(sizeof(int)*PM);
	memset(buffer,0,PM);
	if(sem_init(&mutex,0,1) != 0){
		printf("mutex_init error\n");
		return 0;
	}

	if(sem_init(&empty,0,PM) != 0){
		printf("empty_init error\n");
		return 0;
	}

	if(sem_init(&prdn,0,0) != 0){
		printf("prdn_init error\n");
		return 0;
	}
	
	my_args thpa = {"PA",0};	
	my_args thpb = {"PB",0};
	my_args thpc = {"PC",0};
	my_args thca = {"CA",1};
	my_args thcb = {"CB",0};

	pthread_create(&pa,NULL,(void*)&producer,&thpa);
	pthread_create(&pb,NULL,(void*)&producer,&thpb);
	pthread_create(&pc,NULL,(void*)&producer,&thpc);
	pthread_create(&ca,NULL,(void*)&consumer,&thca);
	pthread_create(&cb,NULL,(void*)&consumer,&thcb);

	pthread_join(pa,NULL);
	pthread_join(pb,NULL);
	pthread_join(pc,NULL);
	pthread_join(ca,NULL);
	pthread_join(cb,NULL);

	sem_destroy(&mutex);
	sem_destroy(&empty);
	sem_destroy(&prdn);

	return 0;
}
