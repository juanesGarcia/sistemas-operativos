#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <wait.h>
#include <signal.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include<string.h>

void *oldhandler;
void sighandler(int sig){
printf("[%d]sig %d capturada \n",getpid(),sig);
}


int main()
{
int i=0,j=0;
int k,n=0;
pid_t pid[5];
pid_t h[3];
pid_t h211;
char *ptr=NULL;
int shm_id;
int shm_size = sizeof(char)*14;

shm_id=shmget(IPC_PRIVATE, shm_size, IPC_CREAT | S_IRUSR | S_IWUSR);
oldhandler=signal(SIGUSR1,sighandler);

if(oldhandler==SIG_ERR){perror("signal:\n");exit(EXIT_FAILURE);}

for(i=0;i<5;i++){
	pid[i]=fork();
	if(!pid[i]){
	printf("el hijo [%d] con el pid:%d\n",i,getpid());
	if(i==0){
		j=0;
		h[j]=fork();
		if(!h[j]){
		printf("hijo h[%d]  con el pid %d\n",j,getpid());
		}
	}
	if(i==2){
		j=1;
		h[j]=fork();
		if(!h[j]){
		printf("hijo h[%d] con el pid %d\n",j,getpid());
		h211=fork();
		if(!h211){
		printf("hijo 211 con el pid %d\n",getpid());
		}
		}
		
	}
	if(i==4){
		j=2;
		h[j]=fork();
		if(!h[j]){
		printf("hijo h[%d] con el pid %d\n",j,getpid());
		}
	}
	
	break;
	}
}


if(i==5){
	ptr = (char *) shmat(shm_id, 0, 0);
	usleep(1000);
	printf("padre con el pid [%d] inicio\n",getpid());
	strcpy(ptr,"A");
	printf("la letra [%s]\n",ptr);
	kill(pid[4],SIGUSR1); 
	pause();
	strcpy(ptr,"A");
	printf("la letra [%s]\n",ptr);
	printf("padre con el pid [%d] termino\n",getpid());
 
	for(int k=0;k<5;k++){wait(NULL);}	
		

}  else{
	ptr = (char *) shmat(shm_id, 0, 0);
	
        pause();
        if (i!=0)
            
        {
            if(i==4){
                
                if(h[j]!=0){
                		
                	strcpy(ptr,"B");
	                printf("la letra [%s]\n",ptr);
                    printf("hijo [%d] iniciado\n",i);
                    usleep(10000);
                    kill(h[j],SIGUSR1);

                    pause();
              		strcpy(ptr,"B");
	                 printf("la letra [%s]\n",ptr);
                    printf("hijo [%d] la segunda vez terminado\n",i);
                     usleep(10000);
                    kill(pid[i-1],SIGUSR1);
                }else{
                 
                 	strcpy(ptr,"C");
	                printf("la letra [%s]\n",ptr);    
                    printf("hijo h[%d] iniciando\n",j);
                    usleep(10000);
                    kill(getppid(),SIGUSR1);
                }
            }

    	          if(i==2){

            	 if(h[j]!=0){

                	strcpy(ptr,"E");
	                printf("la letra [%s]\n",ptr);    
                    printf("hijo [%d] iniciado\n",i);
                    usleep(10000);
                    kill(h[j],SIGUSR1);
                    pause();
                    strcpy(ptr,"E");
	                printf("la letra [%s]\n",ptr);
                    printf("hijo [%d] la segunda vez terminado\n",i);
                     usleep(10000);
                    kill(pid[i-1],SIGUSR1);
                }else{
                	if(h211!=0){
                    strcpy(ptr,"F");
	                printf("la letra [%s]\n",ptr);
					printf("hijo h[%d] iniciado\n",j);
                    usleep(10000);
                    kill(h211,SIGUSR1);
                    pause();
                    strcpy(ptr,"F");
	                printf("la letra [%s]\n",ptr);
                    printf("hijo h[%d] la segunda vez terminado\n",j);
                     usleep(10000);
                    kill(getppid(),SIGUSR1);

                	}else{
                 	strcpy(ptr,"G");
	                printf("la letra [%s]\n",ptr);
                	printf("hijo [h211] iniciado\n");
                    usleep(10000);
                    kill(getppid(),SIGUSR1);



                	}
                    
                }

            	
            }

            if(i==1 || i==3){
            	if(i==1){
                		strcpy(ptr,"H");
	                    printf("la letra [%s]\n",ptr);

                	}else{

                		strcpy(ptr,"D");
	                 printf("la letra [%s]\n",ptr);
                	}

            	 printf("hijo [%d] terminado\n",i);
                 usleep(10000);
                kill(pid[i-1],SIGUSR1);
             }    
            
        }else{

        	if(h[j]!=0){
        			strcpy(ptr,"I");
	                 printf("la letra [%s]\n",ptr);
            printf("hijo [%d] iniciando\n",i);
            usleep(10000);
            kill(h[j],SIGUSR1);
            pause();
            strcpy(ptr,"I");
	                 printf("la letra [%s]\n",ptr);
             printf("hijo [%d] segunda vez\n",i);
             kill(getppid(),SIGUSR1);
        	}else{
        		strcpy(ptr,"J");
	                 printf("la letra [%s]\n",ptr);
        		  printf("hijo h[%d] iniciando\n",j);
                  usleep(10000);
                  kill(getppid(),SIGUSR1);

        	}
        
            
        
        }
        
    }

    signal( SIGUSR1, oldhandler); 
	
return EXIT_SUCCESS;
}







