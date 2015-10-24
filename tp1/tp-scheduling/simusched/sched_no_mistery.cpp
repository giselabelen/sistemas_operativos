#include <vector>
#include <queue>
#include <iostream>
#include "sched_no_mistery.h"
#include "basesched.h"

using namespace std;

SchedNoMistery::SchedNoMistery(vector<int> argn) {  
	// recibe como parámetros los quantum de cada nivel de prioridad
	
	cant_prioridades = argn.size();
	quantum_x_prioridad = new int[cant_prioridades];
	tareas_x_prioridad = new queue<tareaMyst>[cant_prioridades];
	
	// inicializo con los quantum que me dieron
	for (int i = 0; i < cant_prioridades; i++)
	{
		quantum_x_prioridad[i] = argn[i];
	}

	// tarea inválida que indica que el cpu no se esta usando
	tareaMyst invalida;
	invalida.pid = -2;
	invalida.prioridad = -1;
	invalida.quantum = 0;

	tarea_actual = invalida;
}

SchedNoMistery::~SchedNoMistery(){
	delete[] tareas_x_prioridad;
	delete[] quantum_x_prioridad;
}

void SchedNoMistery::load(int pid) {  

	// cargo los datos de la tarea que ingresa
	tareaMyst nueva;
	nueva.pid = pid;
	nueva.prioridad= 0;
	nueva.quantum = 0;

	// pongo a la tarea en espera en la cola de mayor prioridad
	tareas_x_prioridad[0].push(nueva);
}

void SchedNoMistery::unblock(int pid) {  

	// busco la tarea con ese pid entre las que están esperando y la
	// agrego a la cola de prioridad que le corresponde
	for (list<tareaMyst>::iterator it = bloqueados.begin(); it != bloqueados.end(); it++)
	{
		if (it->pid == pid)
		{
			// copio para no hacer lío
			tareaMyst copia;
			copia.pid = pid;
			copia.quantum = 0;
			
			// veo si se le puede subir o no la prioridad
			if (it->prioridad > 0)
			{
				copia.prioridad = (it->prioridad) - 1;
			}
			else{
				copia.prioridad = 0;
			}
			
			// encolo y saco de los procesos bloqueados
			tareas_x_prioridad[copia.prioridad].push(copia);
			it = bloqueados.erase(it);
			break;
		}
	}
}

int SchedNoMistery::tick(int cpu, const enum Motivo m) {  

	int proximo;

	switch(m)
	{
		case TICK:
		
			// actualizo los ciclos a correr de la tarea actual
			tarea_actual.quantum--;
			
			// si todavía le queda quantum, sigo en la misma tarea
			if(tarea_actual.quantum > 0)
			{
				proximo = tarea_actual.pid;
			}
			// si se le terminó el quantum...
			else{
				// ...	la pongo en espera en la cola de prioridad correspondiente,
				// verificando que no sea la IDLE o la tarea INVÁLIDA inicial
				if (tarea_actual.pid >= 0)
				{
					// copio para no hacer lío
					tareaMyst copia;
					copia.pid = tarea_actual.pid;
					copia.quantum = 0;
					copia.prioridad = tarea_actual.prioridad;

					// veo si se le puede bajar o no la prioridad
					if (copia.prioridad < cant_prioridades-1)
					{
						copia.prioridad++;
					}
					
					// encolo
					tareas_x_prioridad[copia.prioridad].push(copia);
				}

				// decido qué tarea sigue
				proximo = switcheando();

			}
			break;
		
		case BLOCK:

			// copio la tarea actual
			tareaMyst copia;
			copia.pid = tarea_actual.pid;
			copia.quantum = 0;
			copia.prioridad = tarea_actual.prioridad;

			// la aparto con las otras bloqueadas
			bloqueados.push_back(copia);

			// decido qué tarea sigue
			proximo = switcheando();

			break;
		
		case EXIT:
			
			// decido qué tarea sigue
			proximo = switcheando();
	}
	
	// devuelvo el pid de la siguiente tarea a ejecutar
	return proximo;
}

int SchedNoMistery::switcheando()
{
	int i;
	int proximo;

	// veo de mayor a menor prioridad
	for (i = 0; i < cant_prioridades; i++)
	{
		// si hay tareas con esta prioridad, la pongo a correr
		if (!tareas_x_prioridad[i].empty())
		{
			tarea_actual.pid = (tareas_x_prioridad[i].front()).pid;
			tarea_actual.quantum = quantum_x_prioridad[i];
			tarea_actual.prioridad = i;
			tareas_x_prioridad[i].pop();
			proximo = tarea_actual.pid;
			break;
		}
	}

	// si no hay nadie para correr, mando la IDLE
	if (i == cant_prioridades)
	{
		tarea_actual.pid = -1;
		tarea_actual.quantum = 0;
		proximo = IDLE_TASK;
	}

	return proximo;
}
