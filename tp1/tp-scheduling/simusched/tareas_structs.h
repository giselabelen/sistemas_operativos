#ifndef __TAREAS_STRUCTS__
#define __TAREAS_STRUCTS__

/* ESTRUCTURAS UTILIZADAS PARA REPRESENTAR LAS TAREAS EN LOS SCHEDULERS */

struct tarea{
	int pid;
	int quantum;	// ciclos que restan por correr en el quantum actual
	bool bloqueado;
};

struct tareaMyst{
	int pid;
	int prioridad;
	int quantum;	// ciclos que restan por correr en el quantum actual
};

#endif
