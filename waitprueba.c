#include<stdio.h>
#include<unistd.h>
#include<sys/types.h>
#include<stdlib.h>
#include<sys/wait.h>


int main(void){

	pid_t root = getpid();


	for (int i = 0; i < 3; ++i)
	{
		if(!fork())
			break;
	}

	if(root == getpid()){

		for (int j = 0; j< 3; ++j)
		
			wait(NULL);
			printf(" el padre de los procesos con su pid:%d\n", getpid());
	}else{

		printf(" el proceso hijo con el pid:%d\n",getpid());
	}

	return EXIT_SUCCESS;

}