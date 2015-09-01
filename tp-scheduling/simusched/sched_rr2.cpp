#include <vector>
#include <queue>
#include "sched_rr2.h"
#include "basesched.h"
#include <iostream>

using namespace std;

SchedRR2::SchedRR2(vector<int> argn) {
	// Round robin recibe la cantidad de cores y sus cpu_quantum por parámetro
	cant_cpu = argn[0];
	// cout << cant_cpu << endl;
	quantum_x_cpu = new int[cant_cpu];
	cant_tareas_x_cpu = new int[cant_cpu];
	actual_x_cpu = new tarea2[cant_cpu];
	tareas_x_cpu = new list<tarea2>[cant_cpu];
	
	// tarea invalida que indica que el cpu no se esta usando
	tarea2 invalida;
	invalida.pid = -2;
	invalida.bloqueado = false;
	invalida.quantum = 0;
	

	for (int i = 0; i < cant_cpu; i++){
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
	tarea2 nueva;
	nueva.pid = pid;
	nueva.bloqueado = false;
	nueva.quantum = 0;
	// hasta ahi inicie el elemento
	
	int i;
	int tareas = cant_tareas_x_cpu[0];
	int cpu = 0;
	// cout << "cpu " << cpu << endl;
	// busco el cpu para correr esta tarea
	for(i = 1; i < cant_cpu; i++)
	{//	cout << "no dberia entrar aca" << endl;
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

	// buscar la tarea correspondiente en las que están esperando y desbloquearla
	for(int i = 0; i < cant_cpu; i++)
	{
		for(list<tarea2>::iterator it = tareas_x_cpu[i].begin(); it != tareas_x_cpu[i].end(); it++)
		{
			if(it->pid == pid)
			{
				it->bloqueado = false;
				i = cant_cpu;
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
			actual_x_cpu[cpu].quantum--;
			if(actual_x_cpu[cpu].quantum > 0)
			{
				proximo = actual_x_cpu[cpu].pid;
			}else{
				if(tareas_x_cpu[cpu].empty())
				{
					actual_x_cpu[cpu].quantum = quantum_x_cpu[cpu];
					proximo = actual_x_cpu[cpu].pid;

				}else{
					// copio la tarea actual del cpu
					tarea2 copia;
					copia.pid = actual_x_cpu[cpu].pid;
					copia.bloqueado = false;
					copia.quantum = 0;

					// la pongo en espera
					if (copia.pid >= 0)
					{
						tareas_x_cpu[cpu].push_back(copia);
					}

					proximo = switcheando(cpu);
				}
			}

			break;
		
		case BLOCK:
			// copio la tarea actual del cpu
			tarea2 copia;
			copia.pid = actual_x_cpu[cpu].pid;
			copia.bloqueado = true;
			copia.quantum = 0;

			// la pongo en espera
			tareas_x_cpu[cpu].push_back(copia);

			if(tareas_x_cpu[cpu].empty())
			{
				actual_x_cpu[cpu].pid = -1;							//Si no tengo tareas en espera, mando la idle
				proximo = IDLE_TASK;
			}else{
				proximo = switcheando(cpu);
			}	

			break;
		
		case EXIT:
			cant_tareas_x_cpu[cpu]--;

			if(tareas_x_cpu[cpu].empty())
			{
				actual_x_cpu[cpu].pid = -1;							//Si no tengo tareas en espera, mando la idle
				proximo = IDLE_TASK;
			}else{
				proximo = switcheando(cpu);
			}	
	}
	// cout << proximo << endl;
	return proximo;
}


int SchedRR2::switcheando(int cpu)
{
	tarea2 copia;										//Si tengo tareas en espera...
	int tam = tareas_x_cpu[cpu].size();
	list<tarea2>::iterator it = tareas_x_cpu[cpu].begin();
	int i;
	
	for(i = 0; i < tam; i++)							//Aca busco la primera tarea en espera que no
	{													//este bloqueada, reacomodando la "cola" de
		copia.pid = it->pid;							//manera adecuada, y se la asigno al CPU.
		copia.bloqueado = it->bloqueado;				//ver mas abajo un caso borde, cuando no 
		copia.quantum = it->quantum;					//hay tareas "nobloqueadas".
		it = tareas_x_cpu[cpu].erase(it);
		
		if(copia.bloqueado)
		{
			tareas_x_cpu[cpu].push_back(copia);		// OJO QUE ACA PUEDE ROMPER EL ITERADOR
		}else{
			copia.quantum = quantum_x_cpu[cpu];
			actual_x_cpu[cpu] = copia;
			break;
		}
	}
	
	if(i == tam)										//Pero, si todas estaban bloqueadas, dejo
	{													//el mismo orden y mando la idle
		actual_x_cpu[cpu].pid = -1;
		return IDLE_TASK;
	}else{
		return copia.pid;								//si hay nobloqueadas, mando la primera de estas
	}
}
