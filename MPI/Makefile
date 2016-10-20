OMPFLAGS = -fopenmp
OMPLIBS = -lgomp

CC = gcc
CPPFLAGS = -g -Wall
LDFLAGS = -g -Wall
LDLIBS = $(OMPLIBS)

MPICC = mpicc
#MPICH = /usr/lib/openmpi
MPICH = /opt/openmpi-1.4.3-gcc44
CFLAGS = -I$(MPICH)/include

all: tree_barrier dissem_barrier

dissem_barrier: dissem_barrier.o
	$(MPICC) -o $@ $(LDFLAGS) $^ $(LDLIBS)

tree_barrier: tree_barrier.o
	$(MPICC) -o $@ $(LDFLAGS) $^ $(LDLIBS)

#hello_world: hello_world.o
	#$(MPICC) -o $@ $(LDFLAGS) $^ $(LDLIBS)

#communicate_world: communicate_world.o
	#$(MPICC) -o $@ $(LDFLAGS) $^ $(LDLIBS)

#openmp_mpi: openmp_mpi.o
	#$(MPICC) -o $@ $(LDFLAGS) $^ $(LDLIBS) $(OMPLIBS)

#openmp_mpi.o: openmp_mpi.c
	#$(CC) -c -o $@ $(CPPFLAGS) $(CFLAGS) $(OMPFLAGS) $^

clean:
	rm -f *.o hello_world communicate_world openmp_mpi tree_barrier dissem_barrier
