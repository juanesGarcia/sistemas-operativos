#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <wait.h>
#include <signal.h>


void * oldhandler;
void sighandler( int sig ){
    //printf("[%d]sig %d capturada\n", getpid(), sig);    
}


int main(){
   int i,j, n_hijos, k;
    printf("Introduce la canridad de hijos\n");
        scanf("%d",&n_hijos);
    pid_t pidhijo[n_hijos];
 


    oldhandler = signal( SIGUSR1, sighandler); 
    if(oldhandler == SIG_ERR){perror("signal:");exit(EXIT_FAILURE);  }

    int root = getpid();
    for(i=0; i<n_hijos; i++){
        pidhijo[i] = fork();
        if(!pidhijo[i]) break;
    }

    if(root == getpid()){
            usleep(10000);
            printf("Padre [%d]\n", getpid());        -    
            kill(pidhijo[n_hijos-1], SIGUSR1);
            pause();
            printf("Padre [%d]\n", getpid());
        
    }
    else{
        for(k=0; k<n_hijos; k++){
            pause();
            printf("hijo[%d]\n", getpid());
            if(i!=0)
                kill(pidhijo[i-1], SIGUSR1);
            else
                kill(getppid(), SIGUSR1);
        }
    }

    signal( SIGUSR1, oldhandler); 
    
    return EXIT_SUCCESS;
}