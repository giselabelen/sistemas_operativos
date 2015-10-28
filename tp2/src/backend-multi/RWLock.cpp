#include "RWLock.h"
#include <cassert>

#define LIMITE 1

/* IMPORTANTE: Se brinda una implementación básica del Read-Write Locks
que hace uso de la implementación provista por pthreads. Está dada para
que puedan utilizarla durante la adaptación del backend de mono a multi
jugador de modo de poder concentrarse en la resolución de un problema
a la vez. Una vez que su adaptación esté andando DEBEN hacer su propia
implementación de Read-Write Locks utilizando únicamente Variables de
Condición. */

RWLock :: RWLock() {
    /* Cambiar por su implementación */
    //~ pthread_rwlock_init(&(this->rwlock),NULL);
    pthread_mutex_init(&mutex,NULL);
	lectores = 0;
	escritores = 0;
	leyendo = 0;
	escribiendo = 0;
	contador = 0;
	cont2 = 0;
	pthread_cond_init(&cond_esperoleer,NULL);
	pthread_cond_init(&cond_esperoescribir,NULL);
}

void RWLock :: rlock() {
    /* Cambiar por su implementación */
    //~ pthread_rwlock_rdlock(&(this->rwlock));
    pthread_mutex_lock(&mutex);
	lectores++;
	while(escritores > 0 && contador <= LIMITE){
		pthread_cond_wait(&cond_esperoleer,&mutex);
	}
	leyendo++;
	cont2--;
	if(cont2 == 0){ contador = 0; }
	pthread_mutex_unlock(&mutex);
}

void RWLock :: wlock() {
    /* Cambiar por su implementación */
    //~ pthread_rwlock_wrlock(&(this->rwlock));
    pthread_mutex_lock(&mutex);
	escritores++;
	while(leyendo > 0 || escribiendo > 0 || (lectores > 0 && contador >= LIMITE)){
		pthread_cond_wait(&cond_esperoescribir,&mutex);
	}
	escribiendo++;
	assert(escribiendo == 1);
	contador++;
    pthread_mutex_unlock(&mutex);
}

void RWLock :: runlock() {
    /* Cambiar por su implementación */
    //~ pthread_rwlock_unlock(&(this->rwlock));
    pthread_mutex_lock(&mutex);
    lectores--;
	leyendo--;
	assert(lectores >= 0 && leyendo >= 0);
	if(lectores == 0){
		pthread_cond_signal(&cond_esperoescribir);
	}
    pthread_mutex_unlock(&mutex);
}

void RWLock :: wunlock() {
    /* Cambiar por su implementación */
    //~ pthread_rwlock_unlock(&(this->rwlock));
    pthread_mutex_lock(&mutex);
    escritores--;
	escribiendo--;
	assert(escritores >= 0 && escribiendo == 0);
	if(escritores == 0 || contador >= LIMITE){
		cont2 = lectores;
		pthread_cond_broadcast(&cond_esperoleer);
	}else{
		pthread_cond_signal(&cond_esperoescribir);
    }
    pthread_mutex_unlock(&mutex);
}
