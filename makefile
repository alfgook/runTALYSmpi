EXECS=runTALYSmpi
MPICC?=mpic++

all: ${EXECS}

runTALYSmpi: runTALYSmpi.c
	${MPICC} -o runTALYSmpi runTALYSmpi.c

clean:
	rm -f ${EXECS}
