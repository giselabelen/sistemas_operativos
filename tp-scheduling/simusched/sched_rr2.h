#ifndef __SCHED_RR2__
#define __SCHED_RR2__

#include <list>
#include <vector>
#include <queue>
#include "basesched.h"


using namespace std;

struct tarea2{
	int pid;
	bool bloqueado;
	int quantum;
};


class SchedRR2 : public SchedBase {
	public:
		SchedRR2(std::vector<int> argn);
        ~SchedRR2();
		virtual void load(int pid);
		virtual void unblock(int pid);
		virtual int tick(int cpu, const enum Motivo m);

	private:
		int switcheando(int cpu);
		int cant_cpu;
		int* quantum_x_cpu;
		int* cant_tareas_x_cpu;
		tarea2* actual_x_cpu;
		list<tarea2>* tareas_x_cpu;
};

#endif
