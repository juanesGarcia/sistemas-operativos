#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/ipc.h>


void crearIndex(void **m, int rows, int cols, size_t sizeElement);
void anillos(int p, int N);
void mostrarMatriz(int **m, int N, char *matriz);
unsigned int sizeof_dm(int rows, int cols, size_t sizeElement);
void llenarMatrices(int N);
int multiplicarMatrices(int **a, int **b, int N, int fil, int col);
void error(char *);
int totalProcesos(int N);
int *parte, **matriz1, **matriz2, **matriz3; //sector a parte


int main(){
    srand(time(NULL));//inicializar numeros aleatorios 
    int s, num_procesos, N;
    pid_t hijo = (pid_t)(0), padre = getpid();
    printf("\nDigite el tamaño de las matrices cuadradas: \n");
    scanf("%d", &N);
    printf("\n");

    if(N>0){
        int rows = N, cols = N;
        num_procesos = totalProcesos(N);
        printf("\n Se ejecutarán %d procesos\n", num_procesos);

        int parteSHMID, matricesSHMID[3] = {0};//sectorSHMID a parteSHMID
       
        size_t sizeMatriz = sizeof_dm(rows, cols, sizeof(int));

        parteSHMID = shmget(IPC_PRIVATE, sizeMatriz, IPC_CREAT | 0600);
        parte = shmat(parteSHMID, NULL, 0);

        for (int l = 0; l < 3; ++l){
            matricesSHMID[l] = shmget(IPC_PRIVATE, sizeMatriz, IPC_CREAT | 0600);
            if(matricesSHMID[l] == -1){
				perror("Error en la memoria compartida");
				return -1;
			}
        }

        matriz1 = shmat(matricesSHMID[0], NULL, 0);
        matriz2 = shmat(matricesSHMID[1], NULL, 0);
        matriz3 = shmat(matricesSHMID[2], NULL, 0);
        
        crearIndex((void*)matriz1, rows, cols, sizeof(int));
        crearIndex((void*)matriz2, rows, cols, sizeof(int));
        crearIndex((void*)matriz3, rows, cols, sizeof(int));


        *parte = num_procesos;

        for (s=0; s<num_procesos; s++){

            hijo = fork();
            if(hijo== -1) perror("ERROR creando el hijo");
            else if (!hijo)
             break;
        }    

        if (padre == getpid()){
        
            llenarMatrices(N);
            printf("\n proceso padre con el pid [%d] muestra las matrices\n", getpid());
            mostrarMatriz(matriz1, N, "Matriz 1");
            mostrarMatriz(matriz2, N, "Matriz 2");

            printf("\n-------------------procesos hijos-------------------------\n");

            *parte = 0;
            while (*parte != s);
            printf("\n padre con el pid[%d] muestra matriz final \n", getpid());
            mostrarMatriz(matriz3, N, "Matriz 3");
            printf("\n----------------------------------------------------------------\n");

            shmdt(parte); 
            shmctl(parteSHMID, IPC_RMID, 0);
            
            shmdt(matriz1); 
            shmdt(matriz2); 
            shmdt(matriz3); 
            for (int k = 0; k < 3; ++k) {
                shmctl(matricesSHMID[k], IPC_RMID, 0);
            }
            
            exit(EXIT_SUCCESS);
        }else{
            sleep(1);//espera que iniciar
            while (*parte != s) 
            sleep(2);//espera para que el padre inicie;

            printf("\n hijo %d con el pid [%d]\n\n", s+1,getpid());

            anillos(s,N);
            printf("\n----------------------------------------------------------------\n");
          
            mostrarMatriz(matriz3, N, "  Anillo");
            printf("\n----------------------------------------------------------------\n");

            if (s!= N-1) 
                *parte += 1;
            else 
                *parte = num_procesos;

            shmdt(parte);
            shmdt(matriz1);
            shmdt(matriz2);
            shmdt(matriz3);
        }
    }else{
        perror("Error Tamaño no invalido");
    }
    return 0;
}

unsigned int sizeof_dm(int rows, int cols, size_t sizeElement){
    size_t size = rows * sizeof (void *); // index size
    size += (cols * rows * sizeElement); // Data size
    return size;
}

void llenarMatrices(int N){
    for (int fila=0; fila<N; fila++){
        for (int col=0; col<N; col++){
            matriz1[fila][col] = 1 + rand() % 9;
            matriz2[fila][col] = 1 + rand() % 9;
        }
    }    
}

void crearIndex(void **m, int rows, int cols, size_t sizeElement){
    size_t sizeRow = cols * sizeElement;
    m[0] = m + rows;
    for (int i=1; i<rows; i++){
        m[i] = (m[i-1] + sizeRow);
    }
}

void mostrarMatriz(int **m, int N, char *matriz){
    
    for (int r = 0; r < N; r++){
        if(r == 0) printf("\n%s:\t", matriz);
        else printf("\t\t");
        for (int c = 0; c < N; c++){
            printf("%d\t", m[r][c]);
        }
        printf("\n");
    }
}

int multiplicarMatrices(int **a, int **b, int N, int fil, int col){
    int temp=0;
    for (int k = 0; k < N; k++){
        temp += a[fil][k] * b[k][col];
    }
    printf("\n en la posición [%d][%d] | con el valor : [%d]\n",fil, col, temp);
    return temp;
}

int totalProcesos(int N){
  if (N%2 == 0) 
    return N/2;
  else 
    return N/2+1;
}

void anillos(int s, int N){
    int fIni = s, colIni = s;
    int filaF=N-s, colF=N-s;
    while (fIni < filaF && colIni < colF ){
       
        for (int i = colIni; i < colF; ++i)
            matriz3[fIni][i] = multiplicarMatrices(matriz1, matriz2, N, fIni, i);
        fIni++;
       
        for (int i = fIni; i < filaF; ++i)
            matriz3[i][colF-1] = multiplicarMatrices(matriz1, matriz2, N, i, colF-1);
        colF--;
 
        if (fIni < filaF){
            for (int i = colF-1; i >= colIni; --i) 
                matriz3[filaF-1][i] = multiplicarMatrices(matriz1, matriz2, N, filaF-1, i);
            filaF--;
        }
 
        if (colIni < colF){
            for (int i = filaF-1; i >= fIni; --i)
                 matriz3[i][colIni] = multiplicarMatrices(matriz1, matriz2, N, i, colIni);
            colIni++;
        }
    }
   
}

