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

void tasks_init(void) {
	/* Todos los tipos de tareas se deben registrar acá para poder ser usadas.
	 * El segundo parámetro indica la cantidad de parámetros que recibe la tarea
	 * como un vector de enteros, o -1 para una cantidad de parámetros variable. */
	
	srand(time(NULL));
	 
	register_task(TaskCPU, 1);
	register_task(TaskIO, 2);
	register_task(TaskAlterno, -1);
	register_task(TaskConsola, 3);
}
