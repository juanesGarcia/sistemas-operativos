#include<stdio.h>
#include<unistd.h>
#include<sys/types.h>
#include<stdlib.h>
#include<sys/wait.h>

int main(void){
	
	pid_t a = fork();

	switch(a){

		case -1: perror("error fork");
		exit(EXIT_FAILURE);

		case 0:

		printf("fork hijo: pid=%d y ppid=%d\n ",getpid(),getppid());

		break;

		default:

		printf("fork padre: pid=%d y ppid=%d\n ",getpid(),getppid());
		wait(NULL);

	}
    
    printf("\n");

	return EXIT_SUCCESS;

}