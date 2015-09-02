#include <iostream>
#include <vector>
#include <queue>
#include "sched_no_mistery.h"
#include "basesched.h"

using namespace std;

SchedNoMistery::SchedNoMistery(vector<int> argn) {  
	
	cant_prioridades = argn.size();

	tareas_x_prioridad = new queue<tareaMyst>[cant_prioridades];
	quantum_x_prioridad = new int[cant_prioridades];

	for (int i = 0; i < cant_prioridades; i++)
	{
		quantum_x_prioridad[i] = argn[i];
	}

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

	tareaMyst nueva;
	nueva.pid = pid;
	nueva.prioridad= 0;
	nueva.quantum = 0;

	tareas_x_prioridad[0].push(nueva);
}

void SchedNoMistery::unblock(int pid) {  

	for (list<tareaMyst>::iterator it = bloqueados.begin(); it != bloqueados.end(); it++)
	{
		if (it->pid == pid)
		{
			tareaMyst copia;
			copia.pid = pid;
			copia.quantum = 0;
			if (it->prioridad > 0)
			{
				copia.prioridad = (it->prioridad) - 1;
			}else{
				copia.prioridad = 0;
			}

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
			tarea_actual.quantum--;
			if(tarea_actual.quantum > 0)
			{
				// TODAVIA NO TERMINE EL QUANTUM ASI QUE SIGO
				proximo = tarea_actual.pid;
			}else{

				// SI TERMINE EL QUANTUM O ESTABA EN LA IDLE

				// COPIO LA TAREA ACTUAL CON LA NUEVA PRIORIDAD SOLO SI NO ES LA IDLE/INVALIDA Y LA PUSHEO DONDE VA
				if (tarea_actual.pid >= 0)
				{
					tareaMyst copia;
					copia.pid = tarea_actual.pid;
					copia.quantum = 0;
					copia.prioridad = tarea_actual.prioridad;

					if (copia.prioridad < cant_prioridades)
					{
						copia.prioridad++;
					}

					tareas_x_prioridad[copia.prioridad].push(copia);
				}

				proximo = switcheando();

			}
			break;
		
		case BLOCK:

			tareaMyst copia;
			copia.pid = tarea_actual.pid;
			copia.quantum = 0;
			copia.prioridad = tarea_actual.prioridad;

			bloqueados.push_back(copia);

			proximo = switcheando();

			break;
		
		case EXIT:
			
			proximo = switcheando();
	}
	
	return proximo;
}

int SchedNoMistery::switcheando()
{
	int i;
	int proximo;

	for (i = 0; i < cant_prioridades; i++)
	{
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

	if (i == cant_prioridades)
	{
		tarea_actual.pid = -1;
		tarea_actual.quantum = 0;
		proximo = IDLE_TASK;
	}

	return proximo;
}