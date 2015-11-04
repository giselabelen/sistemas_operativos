#include "RWLock.h"
#include <cassert>

#define LIMITE 1	// CANTIDAD DE ESCRITURAS CONSECUTIVAS PERMITIDAS EN CASO DE HABER LECTORES ESPERANDO

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
	lectores++;		// SOY UN LECTOR NUEVO
	while((escritores > 0 && contador < LIMITE) || escribiendo > 0){
		// O BIEN ALGUIEN ESTA ESCRIBIENDO, O BIEN HAY ESCRITORES Y NO SE SUPERO EL LIMITE
		pthread_cond_wait(&cond_esperoleer,&mutex);
	}
	leyendo++;	// EMPIEZO A LEER
	cont2--;
	if(cont2 == 0){ contador = 0; }		// YA ES HORA DE QUE PUEDAN VOLVER A PASAR ESCRITORES
	pthread_mutex_unlock(&mutex);
}

void RWLock :: wlock() {
    /* Cambiar por su implementación */
    //~ pthread_rwlock_wrlock(&(this->rwlock));
    pthread_mutex_lock(&mutex);
	escritores++;	// SOY UN ESCRITOR NUEVO
	while(leyendo > 0 || escribiendo > 0 || (lectores > 0 && contador >= LIMITE)){
		// O BIEN ALGUIEN ESTA LEYENDO/ESCRIBIENDO, O BIEN HAY LECTORES Y YA PASE EL LIMITE
		pthread_cond_wait(&cond_esperoescribir,&mutex);
	}
	escribiendo++;	// EMPIEZO A ESCRIBIR
	assert(escribiendo == 1);
	pthread_mutex_unlock(&mutex);
}

void RWLock :: runlock() {
    /* Cambiar por su implementación */
    //~ pthread_rwlock_unlock(&(this->rwlock));
    pthread_mutex_lock(&mutex);
    lectores--;		// YA NO VOY A EXISTIR COMO LECTOR
	leyendo--;		// YA NO ESTOY LEYENDO
	assert(lectores >= 0 && leyendo >= 0);
	if(leyendo == 0){
		// SI NADIE MAS ESTA LEYENDO, LE AVISO A UN ESCRITOR
		pthread_cond_signal(&cond_esperoescribir);
	}
    pthread_mutex_unlock(&mutex);
}

void RWLock :: wunlock() {
    /* Cambiar por su implementación */
    //~ pthread_rwlock_unlock(&(this->rwlock));
    pthread_mutex_lock(&mutex);
    escritores--;	// YA NO VOY A EXISTIR COMO ESCRITOR
	escribiendo--;	// YA NO ESTOY ESCRIBIENDO	
	contador++;
	assert(escritores >= 0 && escribiendo == 0);
	if((escritores == 0 || contador >= LIMITE) && lectores > 0){
		// SI HAY LECTORES ESPERANDO Y, O BIEN NO HAY ESCRITORES O BIEN YA LLEGUE AL LIMITE ...
		cont2 = lectores;	// GUARDO LA CANTIDAD MINIMA DE LECTORES QUE VAN A LEER HASTA LA PROXIMA ESCRITURA
		pthread_cond_broadcast(&cond_esperoleer);	// AVISO A TODOS LOS LECTORES
	}else{
		// ... SI NO
		pthread_cond_signal(&cond_esperoescribir);	// AVISO AL PROXIMO ESCRITOR
    }
    pthread_mutex_unlock(&mutex);
}
