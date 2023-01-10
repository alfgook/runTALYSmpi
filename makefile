EXECS=runTALYSmpi
SHLIBS=start_mpi_workers.o start_mpi_workers.so
TALYS_LIB=talys/source/*.o

MPICC?=mpic++

runTALYSmpi: runTALYSmpi.cpp
	${MPICC} -c runTALYSmpi.cpp
	${MPICC} -o runTALYSmpi runTALYSmpi.o ${TALYS_LIB} -lgfortran

start_mpi_workers: export MPI_CPPFLAGS=$(shell mpicc -showme:compile)
start_mpi_workers: export MPI_LDFLAGS=$(shell mpicc -showme:link)
start_mpi_workers: start_mpi_workers.c
	R CMD SHLIB start_mpi_workers.c

all: talys ${EXECS} start_mpi_workers

clean:
	rm -f ${EXECS} *.o

install:
	cp runTALYSmpi /usr/local/bin
	