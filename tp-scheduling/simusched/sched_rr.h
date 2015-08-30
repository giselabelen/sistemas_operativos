#ifndef __SCHED_RR__
#define __SCHED_RR__

#include <vector>
#include <queue>
#include <list>
#include "basesched.h"

using namespace std;

struct tarea{
	int pid;
	bool bloqueado;
	int cpu;
	int quantum;
};

class SchedRR : public SchedBase {
	public:
		SchedRR(vector<int> argn);
        ~SchedRR();
		virtual void load(int pid);
		virtual void unblock(int pid);
		virtual int tick(int cpu, const enum Motivo m);
		
	private:

		int switcheando(int cpu);
		int cant_cpu;
		int* quantum_x_cpu;
		tarea* actual_x_cpu;
		list<tarea> proc_esperando;


};

#endif
