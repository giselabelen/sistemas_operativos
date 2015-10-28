#include "RWLock.h"
#include <cstdio>

#define CANT_ESCRITORES	1
#define CANT_LECTORES	50
#define CANT_THREADS	101

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

int main(int argc, char* argv[]) 
{
    pthread_t thread[CANT_THREADS];
    int nros[CANT_THREADS];
    int tid;
    
	for (tid = 0; tid < CANT_THREADS; tid++){
		nros[tid]=tid;
		if (tid == CANT_LECTORES){
			pthread_create(&thread[tid], NULL, escritor, &nros[tid]);
		}else{
			pthread_create(&thread[tid], NULL, lector, &nros[tid]);
		}
	}
	
	for (tid = 0; tid < CANT_THREADS; tid++){
		pthread_join(thread[tid], NULL);
	}

    return 0;
}
