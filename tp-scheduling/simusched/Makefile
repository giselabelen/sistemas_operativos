export LC_ALL=C
CXXFLAGS= -Wall -pedantic -g -ggdb
LDFLAGS= -lpthread

OBJS=main.o simu.o basesched.o basetask.o tasks.o sched_rr.o sched_fcfs.o sched_rr2.o sched_no_mistery.o
MAIN=simusched


.PHONY: all clean new ejercicio1 ejercicio2 ejercicio3 ejercicio4 ejercicio5 ejercicio6 ejercicio7 ejercicio8
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
	rm *.png

new: clean all

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

ejercicio7: $(MAIN)

ejercicio8: $(MAIN)



