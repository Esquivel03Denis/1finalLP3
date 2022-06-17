# include <stdio.h>
# include <pthread.h>
# include <string.h>
# include <stdlib.h>
# include <unistd.h>
# include <semaphore.h>



typedef struct datosFactorial{
	int indice;
	int valor;
	long int factorial;
}datosFactorial;

void *factorial();
sem_t sem1;
static datosFactorial * vectorGlobal;
static int variableGlobal=1;
static int variableGlobalIndice=1;



int main (int argc, char *argv[]){
	sem_init(&sem1,0,1);
	datosFactorial vectorValores[argc];
	vectorGlobal = vectorValores;
	pthread_t hilos[argc];
	int aux =1;
	int error = 0;
	int valor;

	//GUARDAR VALORES
	while(aux < argc){	
		vectorGlobal[aux].indice = aux;		
		vectorGlobal[aux].factorial = 1;		
		vectorGlobal[aux].valor = atoi(argv[aux]);
		if(vectorGlobal[aux].valor < 0){
			error =1;break;
		}		
		aux++;
	}
	
	if(error == 0){
		//CREAR HILOS
		aux =1;
		while(aux < argc){
			pthread_create(&hilos[aux], NULL, factorial,NULL);	
			aux++;
		}

		//JOIN
		aux =1;
		while(aux < argc){
			pthread_join(hilos[aux], NULL);
			aux++;
		}
	}else{
		printf("Parametros incorrectos\n");
	}

	return 0;
}


void *factorial(){
	int i, valor_numero,indiceAuxiliar;
	sem_wait(&sem1);
	valor_numero = variableGlobal;
	for(i=1;i<=vectorGlobal[valor_numero].valor;i++){
	
		vectorGlobal[valor_numero].factorial = vectorGlobal[valor_numero].factorial * i;
	
	}
	variableGlobal ++;
	sem_post(&sem1);
	while(1){
		sem_wait(&sem1);
		if(valor_numero == variableGlobalIndice){
			printf("Hilo:%d   Valor:%d\n",valor_numero,vectorGlobal[valor_numero].valor);
			printf("Factorial = %ld\n\n",vectorGlobal[valor_numero].factorial);
			variableGlobalIndice++;
			sem_post(&sem1);
			break;
		}else{
		sem_post(&sem1);

		}
		
	}


}

