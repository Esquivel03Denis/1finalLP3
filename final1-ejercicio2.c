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
static datosSegmento * vectorGlobal;
static int vectorMax[MAX];
static int numeroEncontrado=0;   //si se encuentra el valor, entonces la variable toma el valor 1
static int variableGlobalIndice=-1;
static int indicePosicion=-1;  //contendra la posicion en el caso de que la variable sea encontrada




int main (int argc, char *argv[]){
	int segmento;
	//int vectorAux[MAX];
	int argumento2;
	datosSegmento pos;
	srand (time(NULL));
	for (int i = 0; i < MAX; ++i)
	{

		vectorMax[i] = rand() % 10;
		printf("%d\n",vectorMax[i]);
	}
	sem_init(&sem1,0,1);
	argumento2 = atoi(argv[2]);
	pthread_t hilos[argumento2];
	segmento = MAX / argumento2;
	datosSegmento vectorValores[segmento];
	vectorGlobal = vectorValores;
	int distribucion[argumento2];
	for (int i = 0; i < argumento2; ++i)
	{
			distribucion[i] = 0;
	}
	int a;
	int suma=0;
		while(suma != MAX){
			for (int i = 0; i < argumento2; ++i)
			{
				distribucion[i] ++;
					for (int i = 0; i < argumento2; ++i)
					{	

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
			printf("VALORES %d %d\n",vectorGlobal[a].inicio ,vectorGlobal[a].fin);
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
			printf("El indice es %d\n", indicePosicion);
		else
			printf("VALOR NO ENCONTRADO\n");


return 0;
}

void *busqueda(){
	int aux=0;

	sem_wait(&sem1);
	variableGlobalIndice++;
	sem_post(&sem1);
	printf("DESPUES%d\n",vectorGlobal[variableGlobalIndice].inicio );

	for (int i = vectorGlobal[variableGlobalIndice].inicio; i <=vectorGlobal[variableGlobalIndice].fin; ++i)
	{
		if(numeroEncontrado ==0){
			printf("MASVALORES %d  %d\n",vectorMax[i],vectorGlobal[variableGlobalIndice].valorBuscado);
				if (vectorMax[i] == vectorGlobal[variableGlobalIndice].valorBuscado){
					printf("SE\n");
					numeroEncontrado = 1;
					indicePosicion = i;
					break;
				}

		}else{

			break;
		}
	}

	
}

