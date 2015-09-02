#ifndef __SCHED_NOMIS__
#define __SCHED_NOMIS__

#include <vector>
#include <queue>
#include <list>
#include "basesched.h"

using namespace std;

struct tareaMyst{
	int pid;
	int prioridad;
	int quantum;
};

class SchedNoMistery : public SchedBase {
	public:
		SchedNoMistery(std::vector<int> argn);
		~SchedNoMistery();
		virtual void load(int pid);
		virtual void unblock(int pid);
		virtual int tick(int cpu, const enum Motivo m);

	private:
		int cant_prioridades;
		queue<tareaMyst>* tareas_x_prioridad;
		int* quantum_x_prioridad;
		list<tareaMyst> bloqueados;
		tareaMyst tarea_actual;

		int switcheando();

};

#endif
