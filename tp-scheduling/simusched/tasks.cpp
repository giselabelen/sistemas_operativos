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

//esta funcion es para entender a sched mystery.. es una task que corre un cierto totalcpu ciclos, se bloquea
//una vez en el ciclo numero lugardebloqueo y durante longbloqueo ciclos. totalcpu no contepla los ciclos que estuvo 
//bloqueado
void TaskExpMyst(int pid, vector<int> params) { // params: totalcpu, lugardebloqueo, longbloqueo, cant_bloqueos
	
	int totalcpu = params[0];
	int lugardebloqueo = params[1];
	int longbloqueo = params[2];
	int cant_bloqueos = params[3];
	
	uso_CPU(pid, lugardebloqueo - 1);
	
	for (int j = 0; j < cant_bloqueos; j++)
	{
		uso_IO(pid,longbloqueo);
	}
		
	uso_CPU(pid, totalcpu - (lugardebloqueo + (cant_bloqueos * longbloqueo)));

	return;
}



void TaskBatch(int pid, vector<int> params) { // params: totalcpu, cantbloqueos
	
	int totalcpu = params[0];
	int cantbloqueos = params[1];
	int cuenta = totalcpu - cantbloqueos;
	int bloquear;
	
	for(int i = 0; i < cuenta-1; i++)
	{
		bloquear = rand() % 2;
		


		if(bloquear && (cantbloqueos > 0))
		{
			uso_IO(pid,1);
			cantbloqueos--;
		}
		
		uso_CPU(pid,1);
	}
	
	while(cantbloqueos > 0)
	{
		uso_IO(pid,1);
		cantbloqueos--;
	}
	
	return;
}

void tasks_init(void) {
	/* Todos los tipos de tareas se deben registrar acá para poder ser usadas.
	 * El segundo parámetro indica la cantidad de parámetros que recibe la tarea
	 * como un vector de enteros, o -1 para una cantidad de parámetros variable. */
	
	srand(time(NULL));
	 
	register_task(TaskCPU, 1);
	register_task(TaskIO, 2);
	register_task(TaskAlterno, -1);
	register_task(TaskConsola, 3);
	register_task(TaskBatch, 2);
	register_task(TaskExpMyst, 4);

}
