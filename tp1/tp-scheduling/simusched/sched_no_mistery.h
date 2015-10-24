#ifndef __SCHED_NOMIS__
#define __SCHED_NOMIS__

#include <vector>
#include <queue>
#include <list>
#include "basesched.h"
#include "tareas_structs.h"

using namespace std;

class SchedNoMistery : public SchedBase {
	public:
		SchedNoMistery(std::vector<int> argn);
		~SchedNoMistery();
		virtual void load(int pid);
		virtual void unblock(int pid);
		virtual int tick(int cpu, const enum Motivo m);

	private:
		int cant_prioridades;					// cant de prioridades establecidas
		int* quantum_x_prioridad;				// quantum en cada prioridad
		tareaMyst tarea_actual;					// tarea corriendo ahora
		list<tareaMyst> bloqueados;				// procesos bloqueados
		queue<tareaMyst>* tareas_x_prioridad;	// tareas en cada prioridad
		
		int switcheando();
};

#endif
