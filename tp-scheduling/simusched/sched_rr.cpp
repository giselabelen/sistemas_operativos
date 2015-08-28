#include <vector>
#include <queue>
#include "sched_rr.h"
#include "basesched.h"
#include <iostream>

using namespace std;

SchedRR::SchedRR(vector<int> argn) {
	// Round robin recibe la cantidad de cores y sus cpu_quantum por parámetro
	cant_cpu = argn[0];
	
	int arreglo_q[cant_cpu];
	tarea arreglo_t[cant_cpu];
	
	quantum_x_cpu = arreglo_q;
	actual_x_cpu = arreglo_t;
	
	for (int i=0;i<cant_cpu;i++){
		quantum_x_cpu[i] = argn[i+1];
		actual_x_cpu[i] = NULL;
	}
}

SchedRR::~SchedRR() {
	
}


void SchedRR::load(int pid) {
	
	tarea nueva;
	nueva.pid = pid;
	nueva.cpu = -1;
	nueva.bloqueado = false;
	nueva.quantum = 0;
	// hasta ahi inicie el elemento
	
	int i;
	
	for(i = 0; i < cant_cpu; i++)
	{
		if(actual_x_cpu[i] == NULL)
		{
			nueva.cpu = i;
			nueva.quantum = quantum_x_cpu[i];
			actual_x_cpu[i] = nueva;
			break;
		}
	}
	
	if(i == cant_cpu)
	{
		proc_esperando.push_back(nueva);
	}
}

void SchedRR::unblock(int pid) {
	
	for(list<tarea>::iterator it = proc_esperando.begin(); it != proc_esperando.end(); it++)
	{
		if(it->pid == pid)
		{
			it->bloqueado = false;
			break;
		}
	}
}

int SchedRR::tick(int cpu, const enum Motivo m) {
	
	switch(m)
	{
		case TICK:
			
		
		case BLOCK:
			
		
		case EXIT:
			if(proc_esperando.empty())
			{
				actual_x_cpu[cpu] = NULL;
				return IDLE_TASK;
			}else{
				tarea copia;
				int tam = proc_esperando.size();
				list<tarea>::iterator it = proc_esperando.begin();
				int i;
				
				for(i = 0; i < tam; i++)
				{
					copia.pid = it->pid;
					copia.bloqueado = it->bloqueado;
					copia.cpu = it->cpu;
					copia.quantum = it->quantum;
					it = proc_esperando.erase(it);
					
					if(copia.bloqueado)
					{
						proc_esperando.push_back(copia);	// OJO QUE ACA PUEDE ROMPER EL ITERADOR
					}else{
						copia.cpu = cpu;
						copia.quantum = quantum_x_cpu[cpu];
						actual_x_cpu[cpu] = copia;
						break;
					}
				}
				
				if(i == tam)
				{
					actual_x_cpu[cpu] = NULL;
					return IDLE_TASK;
				}else{
					return copia.pid;
				}
			}
			
			actual_x_cpu[cpu] = ;
		
	}
	
	
}
