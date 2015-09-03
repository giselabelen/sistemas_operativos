#include <vector>
#include <queue>
#include <iostream>
#include "sched_rr2.h"
#include "basesched.h"

using namespace std;

SchedRR2::SchedRR2(vector<int> argn) {
	// Round robin recibe la cantidad de cores y sus cpu_quantum por parámetro
	
	cant_cpu = argn[0];
	quantum_x_cpu = new int[cant_cpu];
	cant_tareas_x_cpu = new int[cant_cpu];
	actual_x_cpu = new tarea[cant_cpu];
	tareas_x_cpu = new list<tarea>[cant_cpu];
	
	// tarea inválida que indica que el cpu no se esta usando
	tarea invalida;
	invalida.pid = -2;
	invalida.quantum = 0;
	invalida.bloqueado = false;
	
	// inicializo
	for (int i = 0; i < cant_cpu; i++)
	{
		quantum_x_cpu[i] = argn[i+1];
		cant_tareas_x_cpu[i] = 0;
		actual_x_cpu[i] = invalida;
	}
}

SchedRR2::~SchedRR2() {
	delete[] quantum_x_cpu;
	delete[] cant_tareas_x_cpu;
	delete[] actual_x_cpu;
	delete[] tareas_x_cpu;
}

void SchedRR2::load(int pid) {

	// cargo los datos de la tarea que ingresa
	tarea nueva;
	nueva.pid = pid;
	nueva.quantum = 0;
	nueva.bloqueado = false;
	
	int i;
	int tareas = cant_tareas_x_cpu[0];
	int cpu = 0;

	// elijo el cpu para asignarle esta tarea
	for(i = 1; i < cant_cpu; i++)
	{
		if (cant_tareas_x_cpu[i] < tareas)
		{
			tareas = cant_tareas_x_cpu[i];
			cpu = i;
		}
	}

	// uso el cpu encontrado
	nueva.quantum = quantum_x_cpu[cpu];
	tareas_x_cpu[cpu].push_back(nueva);
	cant_tareas_x_cpu[cpu]++;
}

void SchedRR2::unblock(int pid) {

	// busco la tarea correspondiente entre las que están esperando y la desbloqueo
	for(int i = 0; i < cant_cpu; i++)
	{
		for(list<tarea>::iterator it = tareas_x_cpu[i].begin(); it != tareas_x_cpu[i].end(); it++)
		{
			if(it->pid == pid)
			{
				it->bloqueado = false;
				i = cant_cpu;			// para salir del doble for
				break;
			}
		}
	}
}

int SchedRR2::tick(int cpu, const enum Motivo m) {

	int proximo;

	switch(m)
	{
		case TICK:
			
			// actualizo los ciclos a correr de la tarea actual
			actual_x_cpu[cpu].quantum--;
			
			// si todavía le queda quantum, sigo en la misma tarea
			if(actual_x_cpu[cpu].quantum > 0)
			{
				proximo = actual_x_cpu[cpu].pid;
			}
			// si se le terminó el quantum...
			else{
				// ... 	y no hay más nadie para correr, sigo en la misma 
				// 		tarea, reseteando su 'quantum' ...
				if(tareas_x_cpu[cpu].empty())
				{
					actual_x_cpu[cpu].quantum = quantum_x_cpu[cpu];
					proximo = actual_x_cpu[cpu].pid;
				}
				// ...	sino desalojo a la tarea actual y cargo a la siguiente
				else{
					// copio la tarea actual del cpu
					tarea copia;
					copia.pid = actual_x_cpu[cpu].pid;
					copia.quantum = 0;
					copia.bloqueado = false;

					// la pongo en espera, verificando que no sea la IDLE
					// o la tarea INVÁLIDA inicial
					if (copia.pid >= 0)
					{
						tareas_x_cpu[cpu].push_back(copia);
					}
					
					// decido qué tarea sigue
					proximo = switcheando(cpu);
				}
			}

			break;
		
		case BLOCK:
		
			// copio la tarea actual del cpu, CON EL BLOQUEO SETEADO
			tarea copia;
			copia.pid = actual_x_cpu[cpu].pid;
			copia.quantum = 0;
			copia.bloqueado = true;

			// la pongo en espera
			tareas_x_cpu[cpu].push_back(copia);

			// si no hay más nadie para correr, mando la IDLE
			if(tareas_x_cpu[cpu].empty())
			{
				actual_x_cpu[cpu].pid = -1;
				proximo = IDLE_TASK;
			}
			// caso contrario, decido qué tarea sigue
			else{
				proximo = switcheando(cpu);
			}	

			break;
		
		case EXIT:
		
			// actualizo la cantidad de tareas de este cpu
			cant_tareas_x_cpu[cpu]--;

			// si no hay más nadie para correr, mando la IDLE
			if(tareas_x_cpu[cpu].empty())
			{
				actual_x_cpu[cpu].pid = -1;
				proximo = IDLE_TASK;
			}
			// caso contrario, decido qué tarea sigue
			else{
				proximo = switcheando(cpu);
			}	
	}
	
	// devuelvo el pid de la siguiente tarea a ejecutar
	return proximo;
}

int SchedRR2::switcheando(int cpu)
{
	int tam = tareas_x_cpu[cpu].size();	// cantidad de procesos en espera para este cpu
	list<tarea>::iterator it = tareas_x_cpu[cpu].begin();
	
	tarea copia;
	int i;
	
	// recorro los procesos en espera para este cpu hasta el primero que no esté bloqueado
	for(i = 0; i < tam; i++)
	{
		// hago una copia para no perder nada en el camino
		copia.pid = it->pid;
		copia.quantum = it->quantum;
		copia.bloqueado = it->bloqueado;
		
		// 'desencolo' esta tarea
		it = tareas_x_cpu[cpu].erase(it);
		
		// si la tarea que estoy revisando está bloqueada, 
		// la pongo en espera otra vez y sigo mirando
		if(copia.bloqueado)
		{
			tareas_x_cpu[cpu].push_back(copia);
		}
		// si no, la pongo para correr y me voy
		else{
			copia.quantum = quantum_x_cpu[cpu];
			actual_x_cpu[cpu] = copia;
			break;
		}
	}
	
	// Si todas las tareas en espera estaban bloqueadas, mando la IDLE.
	// Los procesos en espera quedan como estaban al principio.
	if(i == tam)
	{
		actual_x_cpu[cpu].pid = -1;
		return IDLE_TASK;
	}
	// si no, mando la que encontré (que es la primera no bloqueada)
	else{
		return copia.pid;
	}
}
