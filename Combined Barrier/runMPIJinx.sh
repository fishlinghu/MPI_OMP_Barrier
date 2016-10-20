#!/bin/sh
#PBS -q class_short16
#PBS -l nodes=8:sixcore
#PBS -l walltime=00:30:00 
#PBS -N model2_8_12
OMPI_MCA_mpi_yield_when_idle=0 /opt/openmpi-1.4.3-gcc44/bin/mpirun --hostfile $PBS_NODEFILE -np 8 ~/lab2/combine/harness_2 12 1000000 
