#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>

void* multiescalar(void* arg);
int num,size,cantidad,sum=0;
int *vector,*vector2,*resul,*dato=NULL;;
pthread_mutex_t mt = PTHREAD_MUTEX_INITIALIZER;

int main(){
	


	FILE *file=fopen("input2.txt","r");

	if(!file){

		perror("error al abrir");
		exit(-1);
	}

	fscanf(file,"%d",&size);

	vector=malloc(sizeof(int)*size);
	vector2=malloc(sizeof(int)*size);
    resul=malloc(sizeof(int)*size);
    printf("vector 1 =\n");
    printf("[");
	for (int i = 0; i < size; ++i)
	{
	fscanf(file,"%d",&vector[i]);
	printf("%d ",vector[i]);
	}
	 printf("] \n\n");
	 printf("vector 2 =\n");
     printf("[");
	for (int i = 0; i < size; ++i)
	{
	fscanf(file,"%d",&vector2[i]);
	printf("%d ",vector2[i]);
	}
    printf("]\n\n");

	printf("Digite la cantidad de hilo\n");
	scanf("%d",&num);
	printf("\n");

	cantidad=size/num;
   
	if(size%num==0 && num<=size){
	pthread_t tid[num];
	
	for(int i=0; i<num; i++){	
	    dato=(int *)malloc(sizeof(int));
	    *dato=i;
	    if(i==0){
           multiescalar(dato);
	    }else{
	    	pthread_create(&tid[i], NULL,multiescalar,(void*)dato);		
	    }
		
	 }
	 
	 for(int i=1; i<num; i++)
	    pthread_join(tid[i], NULL);
	}else{
		perror("pasar un numero de hilos diferente");
	}

	printf("Hilo principal[%lu]\n", pthread_self());
	printf("[");
	for (int i = 0; i < size; ++i)
	 {
	 	pthread_mutex_lock(&mt);
	 	sum+=resul[i];
	 	printf("%d ",resul[i]);
	 	pthread_mutex_unlock(&mt);

	 } 

    printf("]\n\n");
    printf(" la suma total es: %d\n\n",sum);
    pthread_exit(0);

	
	return EXIT_SUCCESS;
}

void* multiescalar(void* arg){
	int pos=*((int*)arg);
	int ini=pos*cantidad,fin=ini+cantidad,mul;
	printf("Hilo [%lu]\n", pthread_self()); 
    printf("[");
	for (int i = ini; i <fin; ++i)
	{
		pthread_mutex_lock(&mt);
	     mul=vector[i]*vector2[i];
	     resul[i]=mul;
	     printf("%d ",resul[i]);
	     pthread_mutex_unlock(&mt);

	}
	
 printf("]\n\n");
	
}