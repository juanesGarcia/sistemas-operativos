#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <wait.h>
#include <signal.h>


void * oldhandler;
void sighandler( int sig ){
    printf("[%d]sig %d capturada\n", getpid(), sig);    
}

int i=0;

int main(){
   int j, n_hijos, k;
    pid_t pidhijo[3];
    pid_t h12;

    oldhandler = signal( SIGUSR1, sighandler); 
    if(oldhandler == SIG_ERR){perror("signal:");exit(EXIT_FAILURE);  }

    int root = getpid();
    for(i=0; i<3; i++){
        pidhijo[i] = fork();
        if(!pidhijo[i]){
            printf("el pid %d con el pid:%d\n",i,getpid());
            if(i==1){
                h12=fork();
                if(!h12)
                printf("el pid de 12 es:%d\n",getpid());
            }
            break;
        } 
            
    }






    if(i==3){
            usleep(10000);
            printf("Padre [%d] inicio \n", getpid());  
            kill(pidhijo[2], SIGUSR1);
            pause();
            printf("Padre [%d] termino \n", getpid());
            for(int j=0; j<3; j++)wait(NULL);
        
    }
    else{
        pause();
        if (i!=0)
            
        {
            if(i==1){
                
                if(h12!=0){
                    
                
                    printf("hijo [%d] terminado\n",i);
                    usleep(10000);
                    kill(h12,SIGUSR1);
                    pause();
                    printf("hijo [%d] la segunda vez terminado\n",i);
                     usleep(10000);
                    kill(pidhijo[i-1],SIGUSR1);
                }else{
                    printf("hijo [h12] terminado\n");
                    usleep(10000);
                    kill(getppid(),SIGUSR1);
                }
            }else{
                
              
                printf("hijo [%d] terminado\n",i);
                 usleep(10000);
                kill(pidhijo[i-1],SIGUSR1);
            }
        }else{
        
            
            printf("hijo [%d] terminado\n",i);
            usleep(10000);
            kill(getppid(),SIGUSR1);
        }
        
    }

    signal( SIGUSR1, oldhandler); 
    
    return EXIT_SUCCESS;
}