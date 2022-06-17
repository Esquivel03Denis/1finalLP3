# include <stdio.h>
# include <pthread.h>
# include <string.h>
# include <stdlib.h>
# include <unistd.h>
# include <semaphore.h>
# include <time.h>
#define MAX 20


typedef struct datosSegmento{
	int inicio;
	int fin;
	int valorBuscado;
}datosSegmento;



void *busqueda();
sem_t sem1;
static datosSegmento vectorGlobal[MAX];
static int vectorMax[MAX];
static int numeroEncontrado=0;   				//si se encuentra el valor, entonces la variable toma el valor 1
static int variableGlobalIndice=-1;
static int indicePosicion=-1;  					//contendra la posicion en el caso de que la variable sea encontrada


int main (int argc, char *argv[]){
	if (atoi(argv[2]) <= MAX){
		int argumento2;
		srand (time(NULL));
		printf("Vector MAX:\n");
		argumento2 = atoi(argv[2]);
		pthread_t hilos[argumento2];
		int distribucion[argumento2];

		//Añade números aleatorios al vector
		for (int i = 0; i < MAX; ++i)
		{
			vectorMax[i] = rand() % 10;
			printf("%d  ",vectorMax[i]);
		}
		printf("\n\n");
		sem_init(&sem1,0,1);

		//Calcula los segmentos para cada hilo
		for (int i = 0; i < argumento2; ++i)
		{
			distribucion[i] = 0;
		}
		int a=0;
		int suma=0;
		while(suma != MAX){

			for (int i = 0; i < argumento2; ++i){

				distribucion[i] ++;
					for (int i = 0; i < argumento2; ++i){	

						suma = suma + distribucion[i];
					}

					if(suma == MAX){
						break;
					}
				suma=0;

			}
		}

		//GUARDAR VALORES
		a=0;
		vectorGlobal[a].inicio = 0;
		while(a<argumento2){
			vectorGlobal[a].valorBuscado = atoi(argv[1]);
			vectorGlobal[a].fin = vectorGlobal[a].inicio + distribucion[a] -1;
			printf("Hilo: %d\nRealiza la busqueda en el segmento: [%d,%d]\n\n",(a+1), vectorGlobal[a].inicio, vectorGlobal[a].fin);
			a++;
			vectorGlobal[a].inicio= vectorGlobal[a-1].fin + 1;

		}

		//CREAR HILOS
		a=0;
		while(a<argumento2){
			pthread_create(&hilos[a], NULL, busqueda,NULL);
			a++;	
		}

		//UNIR HILOS
		a =0;
		while(a<argumento2){
			pthread_join(hilos[a], NULL);
			a++;
		}
		if(indicePosicion>=0)
			printf("El número %d se encontró en el índice %d del vector\n",atoi(argv[1]), indicePosicion);
		else
			printf("El número buscado no se encuentra en el vector\n");
	}else
		printf("ERROR!!! el número de hilos excede la longitud del vector\n");

	return 0;
}

void *busqueda(){
	int aux=0;

	sem_wait(&sem1);
	variableGlobalIndice++;
	sem_post(&sem1);

	for (int i = vectorGlobal[variableGlobalIndice].inicio; i <=vectorGlobal[variableGlobalIndice].fin; ++i)
	{
		if(numeroEncontrado ==0){
				if (vectorMax[i] == vectorGlobal[variableGlobalIndice].valorBuscado){
					numeroEncontrado = 1;
					indicePosicion = i;
					break;
				}

		}else{

			break;
		}
	}
}
