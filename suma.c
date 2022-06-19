#include<stdio.h>
#include<unistd.h>
#include<sys/types.h>
#include<stdlib.h>
#include<sys/wait.h>

int leerNumeros(char *,float **);
void escribir_hijo(char *, float);

int main(int argc, char const *argv[]){
	float *datos = NULL;
	int totalnumeros=0;
	int totalhijos=2,i;
	char *filesout[2]={"result_hijo1.txt","result_hijo2.txt"};
	totalnumeros=leerNumeros("datos1.txt",&datos);
	for (i = 0; i < totalhijos; ++i) if (!fork()) break;
	
	if(i<totalhijos){
		FILE *fileout=NULL;
		int delta, pos_ini,pos_fin,hasta;
		float suma_parcial=0.0;
		delta =totalnumeros/totalhijos;
		pos_ini = delta*i;
		hasta=delta-1;
		pos_fin = pos_ini+hasta;
		for (int j= 0; j <=pos_fin ; ++i)
		{
			suma_parcial=datos[j];
		}
		escribir_hijo(filesout[i],suma_parcial);

		printf("p [%d][%d-%d]=%f\n",i,pos_ini,pos_fin,suma_parcial);

	}else{
		for (int i = 0; i < totalhijos; ++i)wait(NULL);
	
	}

	return 0;

}

void escribir_hijo(char *filename, float total){
	FILE *file = fopen(filename,"w");
	if(file==NULL){perror("abrir archivo"); exit(1);}
	fwrite(&total,sizeof(float),1,file);
	fprintf(file, "%f",total);
}

int leerNumeros(char *filename,float **datos){
	int totalnumeros;
	FILE *file;
	file = fopen(filename,"r");
	if(file==NULL){perror("abrir archivo"); exit(1);}
	fscanf(file,"%d",&totalnumeros);
	*datos = (float*)calloc(totalnumeros,sizeof(float));
	for (int i = 0; i < totalnumeros; ++i)
	{
		fscanf(file,"%f",&(*(datos)[i]));
	}
	fclose(file);
	return totalnumeros;
}

