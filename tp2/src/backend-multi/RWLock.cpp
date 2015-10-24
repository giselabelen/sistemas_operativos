#include "RWLock.h"

/* IMPORTANTE: Se brinda una implementación básica del Read-Write Locks
que hace uso de la implementación provista por pthreads. Está dada para
que puedan utilizarla durante la adaptación del backend de mono a multi
jugador de modo de poder concentrarse en la resolución de un problema
a la vez. Una vez que su adaptación esté andando DEBEN hacer su propia
implementación de Read-Write Locks utilizando únicamente Variables de
Condición. */

RWLock :: RWLock() {
    /* Cambiar por su implementación */
    pthread_rwlock_init(&(this->rwlock),NULL);
}

void RWLock :: rlock() {
    /* Cambiar por su implementación */
    pthread_rwlock_rdlock(&(this->rwlock));
}

void RWLock :: wlock() {
    /* Cambiar por su implementación */
    pthread_rwlock_wrlock(&(this->rwlock));
}

void RWLock :: runlock() {
    /* Cambiar por su implementación */
    pthread_rwlock_unlock(&(this->rwlock));
}

void RWLock :: wunlock() {
    /* Cambiar por su implementación */
    pthread_rwlock_unlock(&(this->rwlock));
}
