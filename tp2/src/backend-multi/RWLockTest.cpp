#include "RWLock.h"
#include <cstdio>
#include <ctime>
#include <stdlib.h>
#include <unistd.h>


/*-----------------------------------*/
// DEFINO LAS FUNCIONES
void *escritor(void *p_minumero);
void *lector(void *p_minumero);
void test1();
void test2();
void test3();
/*-----------------------------------*/


RWLock lock;
int compartida = 0;


void *escritor(void *p_minumero)
{
	lock.wlock();
	int minumero = *((int *) p_minumero);
	printf("Voy a escribir. Soy el thread nro. %d.\n", minumero);
	compartida++;
	usleep(100);
	printf("Escribi %d. Soy el thread nro. %d.\n", compartida, minumero);
	lock.wunlock();
	return NULL;
}

void *lector(void *p_minumero)
{
	lock.rlock();
	int minumero = *((int *) p_minumero);
	printf("Voy a leer. Soy el thread nro. %d.\n", minumero);
	usleep(100);
	printf("Lei %d. Soy el thread nro. %d.\n", compartida, minumero);
	lock.runlock();
	return NULL;
}

/* -------------------------- TEST 1 ---------------------------*/
/* 		MANDA MUCHOS LECTORES, UN ESCRITOR, Y MÁS LECTORES	    */
void test1()
{
	int cant_escritores = 1;
	int cant_lectores = 20;
	int cant_threads = cant_escritores+(2*cant_lectores);
	
	pthread_t thread[cant_threads];
    int nros[cant_threads];
    int tid;
    
	for (tid = 0; tid < cant_threads; tid++){
		nros[tid]=tid;
		if (tid == cant_lectores){
			pthread_create(&thread[tid], NULL, escritor, &nros[tid]);
		}else{
			pthread_create(&thread[tid], NULL, lector, &nros[tid]);
		}
	}
	
	for (tid = 0; tid < cant_threads; tid++){
		pthread_join(thread[tid], NULL);
	}
}

/* -------------------------- TEST 2 ---------------------------*/
/* 		MANDA MUCHOS ESCRITORES, UN LECTOR, Y MÁS ESCRITORES    */
void test2()
{
	int cant_escritores = 20;
	int cant_lectores = 1;
	int cant_threads = (2*cant_escritores)+cant_lectores;
	
	pthread_t thread[cant_threads];
    int nros[cant_threads];
    int tid;
    
	for (tid = 0; tid < cant_threads; tid++){
		nros[tid]=tid;
		if (tid == cant_escritores){
			pthread_create(&thread[tid], NULL, lector, &nros[tid]);
		}else{
			pthread_create(&thread[tid], NULL, escritor, &nros[tid]);
		}
	}
	
	for (tid = 0; tid < cant_threads; tid++){
		pthread_join(thread[tid], NULL);
	}
}

/* -------------------------- TEST 3 ---------------------------*/
/* 	CANTIDAD DE LECTORES Y ESCRITORES RANDOM, ORDEN RANDOM 		*/
void test3()
{
	int cant_threads = 41;
	
	pthread_t thread[cant_threads];
    int nros[cant_threads];
    int roles[cant_threads];
    int tid;
    
    // tiro los roles random: 1 para escritor, 0 para lector
    for (int i = 0; i < cant_threads; i++){
		roles[i] = rand() % 2;
	}
    
	for (tid = 0; tid < cant_threads; tid++){
		nros[tid]=tid;
		if (roles[tid] == 1){
			pthread_create(&thread[tid], NULL, escritor, &nros[tid]);
		}else{
			pthread_create(&thread[tid], NULL, lector, &nros[tid]);
		}
	}
	
	for (tid = 0; tid < cant_threads; tid++){
		pthread_join(thread[tid], NULL);
	}
	
}


int main(int argc, char* argv[]) 
{
	int var = atoi(argv[1]);
	srand(time(0));
	
	switch(var){
	
		case 1:
			test1();
			break;
		case 2:
			test2();
			break;
		case 3:
			test3();
			break;
		default:
			std::cout << "Le pifiaste al numerito" << std::endl;
	}
	
    return 0;
}
