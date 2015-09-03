#ifndef __SCHED_RR2__
#define __SCHED_RR2__

#include <vector>
#include <queue>
#include <list>
#include "basesched.h"
#include "tareas_structs.h"

using namespace std;

class SchedRR2 : public SchedBase {
	public:
		SchedRR2(std::vector<int> argn);
        ~SchedRR2();
		virtual void load(int pid);
		virtual void unblock(int pid);
		virtual int tick(int cpu, const enum Motivo m);

	private:
		int cant_cpu;				// cantidad de cores
		int* quantum_x_cpu;			// quantum para cada core
		int* cant_tareas_x_cpu;		// cant tareas corriendo en cada core
		tarea* actual_x_cpu;		// tarea actual en cada core
		list<tarea>* tareas_x_cpu;	// tareas corriendo en cada core
		
		int switcheando(int cpu);
};

#endif
