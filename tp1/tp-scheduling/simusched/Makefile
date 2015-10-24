export LC_ALL=C
CXXFLAGS= -Wall -pedantic -g -ggdb
LDFLAGS= -lpthread

OBJS=main.o simu.o basesched.o basetask.o tasks.o sched_rr.o sched_fcfs.o sched_rr2.o sched_no_mistery.o
MAIN=simusched


.PHONY: all clean new ejercicio1 ejercicio2 ejercicio3 ejercicio4 ejercicio5 ejercicio6 ejercicio7 ejercicio8 ejercicios
all: $(MAIN)

$(MAIN): $(OBJS) sched_mistery.o 
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

%.o: %.cpp %.h basesched.h
basesched.o: basesched.cpp basesched.h
basetask.o: basetask.cpp basetask.h
main.o: main.cpp simu.h basetask.h basesched.h tasks.h sched_fcfs.h \
 sched_rr.h sched_rr2.h tareas_structs.h
simu.o: simu.cpp simu.h basetask.h basesched.h
simusched.o: simusched.cpp basetask.h basesched.h tasks.h
tasks.o: tasks.cpp tasks.h basetask.h

clean:
	rm -f $(OBJS) $(MAIN)
	cd lotes; \
	rm -f *.png

new: clean all

# Para cada ejercicio, hago make all, me muevo a la carpeta 'lotes' y corro.
# Los graficos correspondientes apareceran en la carpeta 'lotes'

# ATENCION: LOS GRAFICOS QUE SALGAN NO NECESARIAMENTE SERAN LOS MISMOS QUE 
# ESTAN EN EL INFORME, YA QUE LOS LOTES QUE UTLICEN TASKCONSOLA Y TASKBATCH
# TOMAN DECISIONES USANDO RAND.

ejercicio1: $(MAIN)
	cd lotes; \
	.././simusched lote_EJ1.tsk 1 1 0 SchedFCFS | .././graphsched.py > ej1.png
	
ejercicio2: $(MAIN)
	cd lotes; \
	.././simusched lote_EJ2.tsk 1 4 0 SchedFCFS | .././graphsched.py > ej2_1core.png; \
	.././simusched lote_EJ2.tsk 2 4 0 SchedFCFS | .././graphsched.py > ej2_2core.png
	
ejercicio3: $(MAIN)
	cd lotes; \
	.././simusched lote_EJ3.tsk 1 1 1 SchedFCFS | .././graphsched.py > ej3.png
	
ejercicio4: $(MAIN)

ejercicio5: $(MAIN)
	cd lotes; \
	.././simusched lote_EJ5.tsk 1 2 0 SchedRR 2 | .././graphsched.py > ej5-q2.png; \
	.././simusched lote_EJ5.tsk 1 2 0 SchedRR 10 | .././graphsched.py > ej5-q10.png; \
	.././simusched lote_EJ5.tsk 1 2 0 SchedRR 50 | .././graphsched.py > ej5-q50.png
	
ejercicio6: $(MAIN)
	cd lotes; \
	.././simusched lote_EJ5.tsk 1 2 0 SchedFCFS | .././graphsched.py > ej6.png
	
ejercicio7: $(MAIN)
	cd lotes; \
	.././simusched lote_EJ7_1.tsk 1 1 1 SchedMistery 4 2 6 8 | .././graphsched.py > ej7-1.png; \
	.././simusched lote_EJ7_2.tsk 1 1 1 SchedMistery 2 4 6 | .././graphsched.py > ej7-2.png; \
	.././simusched lote_EJ7_3.tsk 1 1 1 SchedMistery 4 2 6 8 | .././graphsched.py > ej7-3.png

ejercicio8: $(MAIN)
	cd lotes; \
	.././simusched lote_EJ8_c.tsk 2 1 4 SchedRR 2 2 | .././graphsched.py > ej8-rr_conviene.png; \
	.././simusched lote_EJ8_c.tsk 2 1 4 SchedRR2 2 2 | .././graphsched.py > ej8-rr2_conviene.png; \
	.././simusched lote_EJ8_nc.tsk 2 1 4 SchedRR 2 2 | .././graphsched.py > ej8-rr_noconviene.png; \
	.././simusched lote_EJ8_nc.tsk 2 1 4 SchedRR2 2 2 | .././graphsched.py > ej8-rr2_noconviene.png

ejercicios: ejercicio1 ejercicio2 ejercicio3 ejercicio4 ejercicio5 ejercicio6 ejercicio7 ejercicio8
