#include "RWLock.h"
#include <cstdio>
#include <ctime>
#include <stdlib.h>

//~ #define CANT_ESCRITORES	1
//~ #define CANT_LECTORES	50
//~ #define CANT_THREADS	101

/*-----------------------------------*/
// DEFINO LAS FUNCIONES
void *escritor(void *p_minumero);
void *lector(void *p_minumero);
void test1();
void test2();
void test3();
/*-----------------------------------*/


RWLock lock;

void *escritor(void *p_minumero)
{
	lock.wlock();
	int minumero = *((int *) p_minumero);
	printf("Escribiendo. Soy el thread nro. %d.\n", minumero);
	lock.wunlock();
	return NULL;
}

void *lector(void *p_minumero)
{
	lock.rlock();
	int minumero = *((int *) p_minumero);
	printf("Leyendo. Soy el thread nro. %d.\n", minumero);
	lock.runlock();
	return NULL;
}

/* -------------------------- TEST 1 ---------------------------*/
/* 		MANDA MUCHOS LECTORES, UN ESCRITOR, Y MÁS LECTORES	    */
void test1()
{
	int cant_escritores = 1;
	int cant_lectores = 50;
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
	int cant_escritores = 10;
	int cant_lectores = 1;
	int cant_threads = (2*cant_escritores)+cant_lectores;
	
	pthread_t thread[cant_threads];
    int nros[cant_threads];
    int tid;
    
	for (tid = 0; tid < cant_threads; tid++){
		nros[tid]=tid;
		if (tid == cant_escritores){
			//~ std::cerr << "sale lector " << std::endl;
			pthread_create(&thread[tid], NULL, lector, &nros[tid]);
		}else{
			//~ std::cerr << "sale escritor " << std::endl;
			pthread_create(&thread[tid], NULL, escritor, &nros[tid]);
		}
		//~ std::cerr << "sale thread " << tid << std::endl;
	}
	//~ std::cerr << "ya saque todos" << std::endl;
	
	for (tid = 0; tid < cant_threads; tid++){
		pthread_join(thread[tid], NULL);
		//~ std::cerr << "listo thread " << tid << std::endl;
	}
}

/* -------------------------- TEST 3 ---------------------------*/
/* 	CANTIDAD DE LECTORES Y ESCRITORES RANDOM, ORDEN RANDOM 		*/
void test3()
{
	int cant_threads = 5;
	
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
			//~ std::cerr << "sale escritor " << std::endl;
			pthread_create(&thread[tid], NULL, escritor, &nros[tid]);
		}else{
			//~ std::cerr << "sale lector " << std::endl;
			pthread_create(&thread[tid], NULL, lector, &nros[tid]);
		}
	}
	
	for (tid = 0; tid < cant_threads; tid++){
		pthread_join(thread[tid], NULL);
	}
	
}


int main(int argc, char* argv[]) 
{
	srand(time(0));
    //~ test1();
    //~ test2();
    test3();
    //~ lock.wlock();
    //~ lock.wunlock();

    return 0;
}
