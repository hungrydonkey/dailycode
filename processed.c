#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#define PM 6
#define CM 10

int *buffer;//buffer
sem_t mutex;//buffer mutex
sem_t prdn;// production
sem_t empty;//judge buffer  

void show(){
	for(int i = 0;i < PM;i++)
		printf("%d ",buffer[i]);
	printf("\n");
}

void Aproducer(void* args){
	int t = 1;
	while(t <= PM-1){
		
		sem_wait(&empty);
		sem_wait(&mutex);
		printf("PAid:%u\n",(unsigned int)pthread_self());
		printf("PA %d begin:",t);
		show();
		for(int a = 0;a < PM;a++){
			if(buffer[a] == 0){
				buffer[a] = rand()%10+1;
				sem_post(&prdn);
				break;
			}
		}
		printf("PAend:     ");
		show();
		sleep(1);
		if(t == PM-1){
			printf("PA EXIT\n");
		}
		printf("\n");
		sem_post(&mutex);
		//sem_post(&prdn);
		t++;
	}
	pthread_exit(NULL);
}


void Bproducer(void* args){
	int t = 1;
	while(t <= PM-1){
		
		sem_wait(&empty);
		sem_wait(&mutex);
		printf("PBid:%u\n",(unsigned int)pthread_self());
		printf("PB %d begin:",t);
		show();
		for(int a = 0;a < PM;a++){
			if(buffer[a] == 0){
				buffer[a] = rand()%10+1;
				sem_post(&prdn);
				break;
			}
		}
		printf("PBend:     ");
		show();
		sleep(1);
		if(t == PM-1){
			printf("PB EXIT\n");
		}
		printf("\n");
		sem_post(&mutex);
		//sem_post(&prdn);
		t++;
	}
	pthread_exit(NULL);

}

void Cproducer(void* args){
	int t = 1;
	while(t <= PM-1){	
		sem_wait(&empty);
		sem_wait(&mutex);
		printf("PCid:%u\n",(unsigned int)pthread_self());
		printf("PC %d begin:",t);
		show();
		for(int a = 0;a < PM;a++){
			if(buffer[a] == 0){
				buffer[a] = rand()%10+1;
				sem_post(&prdn);
				break;
			}
		}
		printf("PCend:     ");
		show();
		sleep(1);
		if(t == PM-1){
			printf("PC EXIT\n");
		}
		printf("\n");
		sem_post(&mutex);
		//sem_post(&prdn);
		t++;
	}
	pthread_exit(NULL);
}

void consumerA(void* args){
	int t = 1,a;
	while(t <= CM){
		sem_wait(&prdn);
		sem_wait(&mutex);

		printf("CAid:%u\n",(unsigned int)pthread_self());
		printf("Ca %d begin:",t);
		show();

		for(a = 0;a < PM;a++){
			if(buffer[a]%2 == 1){
				buffer[a] = 0;
				sem_post(&empty);
				break;
			}
		}
		if(a == PM)
			sem_post(&prdn);
		printf("Ca end:    ");
		show();
		sleep(1);
		if(t == CM)
			printf("Ca EXIT\n");
		printf("\n");
		//sem_post(&mutex);
		//if(a < PM)
		//	sem_post(&empty);
		sem_post(&mutex);
		t++;
	}
	pthread_exit(NULL);

}

void consumerB(void* args){
	int t = 1,a;
	while(t <= CM){
		sem_wait(&prdn);
		sem_wait(&mutex);

		printf("CBid:%u\n",(unsigned int)pthread_self());
		printf("Cb %d begin:",t);
		show();

		for(a = 0;a < PM;a++){
			if((buffer[a]%2 == 0) && (buffer[a] != 0)){
				buffer[a] = 0;
				sem_post(&empty);
				break;
			}
		}
		if(a == PM)
			sem_post(&prdn);
		printf("Cb end:    ");
		show();
		sleep(1);
		if(t == CM)
			printf("Cb EXIT\n");
		printf("\n");
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

	pthread_create(&pa,NULL,(void*)&Aproducer,NULL);
	pthread_create(&pb,NULL,(void*)&Bproducer,NULL);
	pthread_create(&pc,NULL,(void*)&Cproducer,NULL);
	pthread_create(&ca,NULL,(void*)&consumerA,NULL);
	pthread_create(&cb,NULL,(void*)&consumerB,NULL);

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


