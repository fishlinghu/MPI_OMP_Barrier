OMPFLAGS = -fopenmp
OMPLIBS = -lgomp

CC = g++
CPPFLAGS = -g -Wall
LDFLAGS = -g -Wall
LDLIBS = $(OMPLIBS)

MPICC = mpic++
#MPICH = /usr/lib/openmpi
MPICH = /opt/openmpi-1.4.3-gcc44
CFLAGS = -I$(MPICH)/include

all: harness harness_2 wordcount

#dissem_barrier: dissem_barrier.o
#	$(MPICC) -o $@ $(LDFLAGS) $^ $(LDLIBS)

#tree_barrier: tree_barrier.o
	#$(MPICC) -o $@ $(LDFLAGS) $^ $(LDLIBS)

wordcount: mpi_barrier.cpp omp_barrier.cpp wordcount.cpp
	$(MPICC) -o $@ $(LDFLAGS) $^ $(LDLIBS) $(OMPLIBS) $(OMPFLAGS)

harness: mpi_barrier.cpp omp_barrier.cpp harness.cpp
	$(MPICC) -o $@ $(LDFLAGS) $^ $(LDLIBS) $(OMPLIBS) $(OMPFLAGS)

harness_2: mpi_barrier.cpp omp_barrier.cpp harness_2.cpp
	$(MPICC) -o $@ $(LDFLAGS) $^ $(LDLIBS) $(OMPLIBS) $(OMPFLAGS)

mpi_barrier.o: mpi_barrier.cpp
	$(MPICC) -c -o $@ $(LDFLAGS) $^ $(LDLIBS) $(OMPLIBS)

omp_barrier.o: omp_barrier.cpp
	$(CC) -c -o $@ $(CPPFLAGS) $(CFLAGS) $(OMPFLAGS) $^

harness.o: harness.cpp
	$(MPICC) -o $@ $(LDFLAGS) $^ $(LDLIBS) $(OMPLIBS)


clean:
	rm -f *.o hello_world communicate_world openmp_mpi tree_barrier dissem_barrier harness harness_2 wordcount
