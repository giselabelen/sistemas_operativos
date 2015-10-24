CC = g++
CFLAGS = -g -Wall -Werror -fno-stack-protector
LDFLAGS = -pthread

BINARIES = backend rwlocktest

all: $(BINARIES)

backend: Backend_multi.cpp Backend_multi.h Enviar_recibir.o Encabezado.h Casillero.h RWLock.o
	$(CC) $(LDFLAGS) $(CFLAGS) Backend_multi.cpp Enviar_recibir.o RWLock.o -o backend

Enviar_recibir.o: Enviar_recibir.cpp Enviar_recibir.h Encabezado.h
	$(CC) $(CFLAGS) -c Enviar_recibir.cpp

RWLock.o: RWLock.cpp RWLock.h
	$(CC) $(LDFLAGS) $(CFLAGS) -c RWLock.cpp

rwlocktest: RWLockTest.cpp RWLock.cpp RWLock.h
	$(CC) $(LDFLAGS) $(CFLAGS) RWLock.cpp RWLockTest.cpp -o rwlocktest

clean:
	rm -f *.o
	rm -f $(BINARIES)
