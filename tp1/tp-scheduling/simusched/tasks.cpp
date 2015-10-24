#include "tasks.h"

using namespace std;

void TaskCPU(int pid, vector<int> params) { // params: n
	uso_CPU(pid, params[0]); // Uso el CPU n ciclos.
}

void TaskIO(int pid, vector<int> params) { // params: ms_pid, ms_io,
	uso_CPU(pid, params[0]); // Uso el CPU ms_pid ciclos.
	uso_IO(pid, params[1]); // Uso IO ms_io ciclos.
}

void TaskAlterno(int pid, vector<int> params) { // params: ms_pid, ms_io, ms_pid, ...
	for(int i = 0; i < (int)params.size(); i++) {
		if (i % 2 == 0) uso_CPU(pid, params[i]);
		else uso_IO(pid, params[i]);
	}
}

void TaskConsola(int pid, vector<int> params) { // params: n, bmin, bmax
	
	int bloqueo;
	
	// pongo nombres para que sea más declarativos
	int n = params[0];
	int bmin = params[1];
	int bmax = params[2];
	
	// para cada bloqueo calculo el tiempo y ejecuto
	for(int i = 0; i < n; i++)
	{
		bloqueo = bmin + rand() % (bmax-bmin+1);
		uso_IO(pid,bloqueo);
	}
	
	return;
}

void TaskBatch(int pid, vector<int> params) { // params: totalcpu, cantbloqueos
	
	int bloquear;
	
	// pongo nombres para que sea más declarativos
	int totalcpu = params[0];
	int cantbloqueos = params[1];
	
	int cuenta = totalcpu - cantbloqueos;
	
	for(int i = 0; i < cuenta-1; i++)
	{
		// bloquear va a valer 0 ó 1
		bloquear = rand() % 2;
		
		// si es 1 y todavía tengo bloqueos por hacer, 
		// hago una llamada bloqueante.
		if(bloquear && (cantbloqueos > 0))
		{
			uso_IO(pid,1);
			cantbloqueos--;
		}
		
		// un uso de cpu
		uso_CPU(pid,1);
	}
	
	// si todavía me faltan bloqueos por hacer, los hago ahora
	while(cantbloqueos > 0)
	{
		uso_IO(pid,1);
		cantbloqueos--;
	}
	
	return;
}

void TaskExpMyst(int pid, vector<int> params) { // params: totalcpu, lugardebloqueo, longbloqueo, cant_bloqueos
	/* Task creada para experimentar con SchedMistery. Lo que hace es hacer 
	 * uso de cpu por 'totalcpu' ciclos, y ejecutar 'cant_bloqueos' llamadas 
	 * bloqueantes a partir del instante 'lugardebloqueo', durando cada una 
	 * 'longbloqueo' ciclos.'totalcpu' no contempla los ciclos que estuvo bloqueado. 
	 * 
	 * IMPORTANTE: ESTO NUNCA CHEQUEA QUE LAS CUENTAS DEN, ASÍ QUE HAY QUE
	 * SER CUIDADOSOS AL PASAR LOS PARÁMETROS.*/
	
	// pongo nombres para que sea más declarativo
	int totalcpu = params[0];
	int lugardebloqueo = params[1];
	int longbloqueo = params[2];
	int cant_bloqueos = params[3];
	
	// uso cpu hasta que tenga que empezar a bloquearse
	uso_CPU(pid, lugardebloqueo - 1);
	
	// hago todas las llamadas bloqueantes
	for (int j = 0; j < cant_bloqueos; j++)
	{
		uso_IO(pid,longbloqueo);
	}
	
	// hago el resto de uso de cpu	
	uso_CPU(pid, totalcpu - (lugardebloqueo + (cant_bloqueos * longbloqueo)));

	return;
}

void tasks_init(void) {
	/* Todos los tipos de tareas se deben registrar acá para poder ser usadas.
	 * El segundo parámetro indica la cantidad de parámetros que recibe la tarea
	 * como un vector de enteros, o -1 para una cantidad de parámetros variable. */
	
	srand(time(NULL));	// para el rand
	 
	register_task(TaskCPU, 1);
	register_task(TaskIO, 2);
	register_task(TaskAlterno, -1);
	register_task(TaskConsola, 3);
	register_task(TaskBatch, 2);
	register_task(TaskExpMyst, 4);

}
