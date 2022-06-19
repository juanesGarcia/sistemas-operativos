#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <wait.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <signal.h>


#define N 3//####################### TAMAÑO DE LA MATRIZ #######################

void * oldhandler;
void sighandler( int sig ){
    //printf("[%d]sig %d capturada\n", getpid(), sig);
}

unsigned int sizeof_dm(int rows, int cols, size_t sizeElement) {
    size_t size;
    size = rows * sizeof(void*);
    size += (cols * rows * sizeElement);
    return size;
}

void create_index(void** m, int rows, int cols, size_t sizeElement) {
    int i;
    size_t sizeRow = cols * sizeElement;
    m[0] = m + rows;
    for (i = 1; i < rows; i++) {
        m[i] = (m[i - 1] + sizeRow);
    }
}

int main() {
    pid_t pidhijo;
    double iter;
    int i, j;
    oldhandler = signal(SIGUSR1, sighandler);
    if(oldhandler == SIG_ERR){perror("signal:");exit(EXIT_FAILURE);}

    double** matrix = NULL;
    int Rows = N, Cols = N;
    size_t sizeMatrix = sizeof_dm(Rows, Cols, sizeof(double));
    int shmId = shmget(IPC_PRIVATE, sizeMatrix, IPC_CREAT | 0600);
    matrix = shmat(shmId, NULL, 0);
    create_index((void*)matrix, Rows, Cols, sizeof(double));

    pidhijo = fork();
    switch(pidhijo){
        case -1:
            perror(" Error en fork");
            exit(EXIT_FAILURE);
        
        case 0: // HIJO
            pause();
            printf("\nHIJO [%d]\n", getpid());
            printf("Matriz recibida:\n");
            for (i = 0; i < N; i++){
                printf("[");
                for(j = 0; j < N; j++){
                    printf(" %.1f |", matrix[i][j]);
                }
                printf("\b]\n");
            }

            break;
        
        default: // PADRE
            usleep(10000); // Espera que el hijo se ponga en pausa
            printf("\nPADRE [%d]\n", getpid());
            iter = 0.0;
            printf("Matriz enviada:\n");
            for (i = 0; i < N; i++){
                printf("[");
                for(j = 0; j < N; j++){
                    matrix[i][j] = iter;
                    printf(" %.1f |", iter);
                    iter++;
                }
                printf("\b]\n");
            }
            
            kill(pidhijo, SIGUSR1);
            wait(NULL);
    }
    

    return EXIT_SUCCESS;
}