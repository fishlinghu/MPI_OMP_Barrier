#!/bin/sh
#PBS -q class_short16
#PBS -l nodes=12:sixcore
#PBS -l walltime=00:30:00 
#PBS -N diss_6_16
OMPI_MCA_mpi_yield_when_idle=0 /opt/openmpi-1.4.3-gcc44/bin/mpirun --hostfile $PBS_NODEFILE -np 6 ~/lab2/MPI/dissem_barrier 1000000 16
