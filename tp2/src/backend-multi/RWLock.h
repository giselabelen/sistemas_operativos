#ifndef RWLock_h
#define RWLock_h
#include <iostream>

class RWLock {
    public:
        RWLock();
        void rlock();
        void wlock();
        void runlock();
        void wunlock();

    private:
        /* IMPORTANTE: Se brinda una implementación básica del Read-Write Locks
        que hace uso de la implementación provista por pthreads. Está dada para
        que puedan utilizarla durante la adaptación del backend de mono a multi
        jugador de modo de poder concentrarse en la resolución de un problema
        a la vez. Una vez que su adaptación esté andando DEBEN hacer su propia
        implementación de Read-Write Locks utilizando únicamente Variables de
        Condición. */

        pthread_rwlock_t rwlock;
};

#endif
