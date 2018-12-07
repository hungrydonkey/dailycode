#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include<stdlib.h>
#include<time.h>
#define BUFFER_SIZE 6
int *buffer;
sem_t mutex,empty,full;
int x,y;
void output()
{
        int i;
        for(i=0;i<BUFFER_SIZE;i++)
        {
                printf("%d",buffer[i]);
                printf(" ");
        }
        printf("\n");
}
void *produce()
{
        int j;
	srand((unsigned)time(0));
	int x=-1;
//int a=1;
        j=0;
        do
        {
	       int a=rand()%10;
                sem_wait(&empty);
                sem_wait(&mutex);
                printf("id:%lu%s%d%s\n",pthread_self(),"生产者",j,"^^^^^ ");
                printf("原缓冲区");
		output();
		x=-1;
		int i=0;
                for(i;i<6;i++)
                {
			x++;
			if(buffer[(x)%BUFFER_SIZE]==0)
			{buffer[(x)%BUFFER_SIZE]=a;
				break;
			}
                }
		printf("现缓冲区");
                output();
                j++;
                sem_post(&mutex);
                sem_post(&full);
        }while(j!=5);
}
void *consumeA()
{                                                                                    int j;
	int y=-1;
	j=0;                                                                         do                                                                           {
                sem_wait(&full);
                sem_wait(&mutex);                                                            printf("id:%lu%s%d%s\n",pthread_self(),"消费者A",j,"***** ");                printf("原缓冲区");   
		output();
		y=-1;
		for(int i=0;i<6;i++)
		{
			y++;
			if(buffer[(y)%BUFFER_SIZE]%2==1)
			{
				buffer[(y)%BUFFER_SIZE]=0;
				break;}
		}
		printf("现缓冲区");
                output();
                j++;
                sem_post(&mutex);
                sem_post(&empty);
        }while (j!=10);
}

void *consumeB()
{
	int y=-1;
        int j;
        j=0;
	do
        {
                sem_wait(&full);
                sem_wait(&mutex);
                printf("id:%lu%s%d%s\n",pthread_self(),"消费者B",j,"***** ");
                printf("原缓冲区");
		output();
		y=-1;
		for(int i=0;i<6;i++)
		{
			y++;
                      if(buffer[(y)%BUFFER_SIZE]%2==0)
		      {       buffer[(y)%BUFFER_SIZE]=0;
			      break;
		      }
		}
		printf("现缓冲区");
                output();                                                                    j++;
                sem_post(&mutex);
                sem_post(&empty);
                }while (j!=10);
}
int main()
{
        int i;
//      x=0;
//      y=0;
        buffer=(int*)malloc(BUFFER_SIZE*sizeof(int*));
        for(i=0;i<BUFFER_SIZE;i++)                                                   {
                buffer[i]=0;
        }
        //semaphore
        sem_init(&mutex,1,1);                                                        sem_init(&empty,0,BUFFER_SIZE);                                              sem_init(&full,0,0);                                                         //multipthread      
	pthread_t tid[5];                                                            pthread_attr_t attr;
        pthread_attr_init(&attr);
//      for(i=0;i<5;i++)                                                     //      {                                                                   
//

	        pthread_create(&tid[3],&attr,consumeA,NULL);                                 pthread_create(&tid[4],&attr,consumeB,NULL);
                pthread_create(&tid[0],&attr,produce,NULL);
                pthread_create(&tid[1],&attr,produce,NULL);
                pthread_create(&tid[2],&attr,produce,NULL);
//      }
        for(i=0;i<5;i++)
        {
                pthread_join(tid[i],NULL);
        }
        return 0;
}
