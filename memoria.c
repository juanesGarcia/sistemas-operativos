#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/shm.h>

int main(){
	double *p=NULL;
	int shmid = shmget((key_t)20221,sizeof(double),IPC_CREAT|0600);
	if (shmid!=-1)printf("ok {%d}\n",shmid);
	else{perror("shmget");exit(1);}
	p=(double*)shmat(shmid,NULL,0);
	*p=5.0;
	shmdt(p);
	

	return EXIT_SUCCESS;
}