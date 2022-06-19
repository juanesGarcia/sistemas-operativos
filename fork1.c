#include<stdio.h>
#include<unistd.h>
#include<sys/types.h>
#include<stdlib.h>
#include<sys/wait.h>


int main(void){

	for (int i = 0; i < 4; ++i)
	{
		if(!fork())
			if(i==0 | i==2){
				printf("los hijos de los hijos con el pid: %d\n",getpid());
			}
		break;

	}

	

	return EXIT_SUCCESS;
}

