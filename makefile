EXECS=runTALYSmpi
SHLIBS=start_mpi_workers.o start_mpi_workers.so

MPICC?=mpic++

all: ${EXECS} start_mpi_workers

runTALYSmpi: runTALYSmpi.c
	${MPICC} -o runTALYSmpi runTALYSmpi.c

start_mpi_workers: export MPI_CPPFLAGS=$(shell mpicc -showme:compile)
start_mpi_workers: export MPI_LDFLAGS=$(shell mpicc -showme:link)
start_mpi_workers: start_mpi_workers.c
	R CMD SHLIB start_mpi_workers.c

clean:
	rm -f ${EXECS} ${SHLIBS}
