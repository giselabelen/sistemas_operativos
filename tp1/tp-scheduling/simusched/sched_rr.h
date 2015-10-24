#ifndef __SCHED_RR__
#define __SCHED_RR__

#include <vector>
#include <queue>
#include <list>
#include "basesched.h"
#include "tareas_structs.h"

using namespace std;

class SchedRR : public SchedBase {
	public:
		SchedRR(vector<int> argn);
        ~SchedRR();
		virtual void load(int pid);
		virtual void unblock(int pid);
		virtual int tick(int cpu, const enum Motivo m);
		
	private:
		int* quantum_x_cpu;			// quantum de cada cpu
		tarea* actual_x_cpu;		// tarea que corre cada cpu
		list<tarea> proc_esperando;	// cola de procesos esperando

		int switcheando(int cpu);
};

#endif
