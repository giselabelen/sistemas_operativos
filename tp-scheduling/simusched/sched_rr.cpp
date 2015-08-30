#include <vector>
#include <queue>
#include "sched_rr.h"
#include "basesched.h"
#include <iostream>

using namespace std;

SchedRR::SchedRR(vector<int> argn) {
	// Round robin recibe la cantidad de cores y sus cpu_quantum por parámetro
	cant_cpu = argn[0];
	
	// cout << "argumentos " << argn[0] << "y" << argn[1] << endl;


	// int arreglo_q[cant_cpu];
	// tarea arreglo_t[cant_cpu];
	
	// quantum_x_cpu = arreglo_q;
	// actual_x_cpu = arreglo_t;

	quantum_x_cpu = new int[cant_cpu];
	actual_x_cpu = new tarea[cant_cpu];
	
	// tarea invalida que indica que el cpu no se esta usando
	tarea invalida;
	invalida.pid = -2;
	invalida.cpu = -1;
	invalida.bloqueado = false;
	invalida.quantum = 0;
	

	for (int i = 0; i < cant_cpu; i++){
		quantum_x_cpu[i] = argn[i+1];
		actual_x_cpu[i] = invalida;
	}
	
}

SchedRR::~SchedRR() {
	delete[] quantum_x_cpu;
	delete[] actual_x_cpu;
}


void SchedRR::load(int pid) {
	
	// cargo los datos de la tarea que ingresa
	tarea nueva;
	nueva.pid = pid;
	nueva.cpu = -1;
	nueva.bloqueado = false;
	nueva.quantum = 0;
	// hasta ahi inicie el elemento
	
	// int i;
	
	// busco si hay un cpu libre para correr esta tarea
	// for(i = 0; i < cant_cpu; i++)
	// {
	// 	// si lo encuentro, lo uso
	// 	if(actual_x_cpu[i].pid == -1)
	// 	{
	// 		nueva.cpu = i;
	// 		nueva.quantum = quantum_x_cpu[i];
	// 		actual_x_cpu[i] = nueva;
	// 		break;
	// 	}
	// }
	
	// // si no, la pongo en espera
	// if(i == cant_cpu)
	// {
		proc_esperando.push_back(nueva);
	// }
	
}

void SchedRR::unblock(int pid) {
	
	// buscar la tarea correspondiente en las que están esperando y desbloquearla
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
	
	int proximo;

	switch(m)
	{
		case TICK:
			actual_x_cpu[cpu].quantum--;
			if(actual_x_cpu[cpu].quantum > 0)
			{
				proximo = actual_x_cpu[cpu].pid;
			}else{
				if(proc_esperando.empty())
				{
					actual_x_cpu[cpu].quantum = quantum_x_cpu[cpu];
					proximo = actual_x_cpu[cpu].pid;

				}else{
					// copio la tarea actual del cpu
					tarea copia;
					copia.pid = actual_x_cpu[cpu].pid;
					copia.cpu = -1;
					copia.bloqueado = false;
					copia.quantum = 0;

					// la pongo en espera
					if (copia.pid >= 0)
					{
						proc_esperando.push_back(copia);
					}

					proximo = switcheando(cpu);
				}
			}

			break;
		
		case BLOCK:
			// copio la tarea actual del cpu
			tarea copia;
			copia.pid = actual_x_cpu[cpu].pid;
			copia.cpu = -1;
			copia.bloqueado = true;
			copia.quantum = 0;

			// la pongo en espera
			proc_esperando.push_back(copia);

			if(proc_esperando.empty())
			{
				actual_x_cpu[cpu].pid = -1;							//Si no tengo tareas en espera, mando la idle
				proximo = IDLE_TASK;
			}else{
				proximo = switcheando(cpu);
			}	

			break;
		
		case EXIT:
			if(proc_esperando.empty())
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

int SchedRR::switcheando(int cpu)
{
	tarea copia;										//Si tengo tareas en espera...
	int tam = proc_esperando.size();
	list<tarea>::iterator it = proc_esperando.begin();
	int i;
	
	for(i = 0; i < tam; i++)							//Aca busco la primera tarea en espera que no
	{													//este bloqueada, reacomodando la "cola" de
		copia.pid = it->pid;							//manera adecuada, y se la asigno al CPU.
		copia.bloqueado = it->bloqueado;				//ver mas abajo un caso borde, cuando no 
		copia.cpu = it->cpu;							//hay tareas "nobloqueadas".
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
	
	if(i == tam)										//Pero, si todas estaban bloqueadas, dejo
	{													//el mismo orden y mando la idle
		actual_x_cpu[cpu].pid = -1;
		return IDLE_TASK;
	}else{
		return copia.pid;								//si hay nobloqueadas, mando la primera de estas
	}
}



		// OTRA OPCION CON LOS ITERADORES DE ALE

		// // ASUMO QUE EXISTE UN IT A PROC_ESPERANDO GLOBAL/ATRIBUTO (VEMOS) QUE SE LLAMA IT_SIG/IT_GLOBAL
		// case EXIT:
		// 	if(proc_esperando.empty())
		// 	{
		// 		actual_x_cpu[cpu] = NULL;							//Si no tengo tareas en espera, mando la idle
		// 		return IDLE_TASK;
		// 	}else{
		// 		tarea copia;										//Si tengo tareas en espera...
		// 		int tam = proc_esperando.size();
		// 		list<tarea>::iterator it = proc_esperando.begin();
		// 		int i;
				
		// 		for(i = 0; i < tam; i++)
		// 		{	
		// 			if(it_sig == proc_esperando.end())
		// 			{
		// 				it_sig = proc_esperando.begin();
		// 			}

		// 			if(it_sig->bloqueado)
		// 			{
		// 				it_sig++;
		// 			}else{
		// 				copia.pid = it_sig->pid;							//manera adecuada, y se la asigno al CPU.
		// 				copia.bloqueado = it_sig->bloqueado;				//ver mas abajo un caso borde, cuando no 
		// 				copia.cpu = cpu;									//hay tareas "nobloqueadas".
		// 				copia.quantum = quantum_x_cpu[cpu];
		// 				it_sig = proc_esperando.erase(it_sig);	// VER QUE PASA CUANDO SE BORRA EL DEL FINAL
		// 				actual_x_cpu[cpu] = copia;
		// 				break;
		// 			}

		// 		if(i == tam)										//Pero, si todas estaban bloqueadas, dejo
		// 		{													//el mismo orden y mando la idle
		// 			actual_x_cpu[cpu] = NULL;
		// 			return IDLE_TASK;
		// 		}else{
		// 			return copia.pid;								//si hay nobloqueadas, mando la primera de estas
		// 		}
		// 	}