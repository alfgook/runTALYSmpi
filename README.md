# runTALYSmpi

A simple program to start talys calculations using an MPI interface in R. As shown in the "example.R" script, using the .C foreign language interface in R "start_mpi_worker" can be used to spawn a given number of mpi threads to execute a C program. The C program that is launched starts talys jobs in specified directories, the jobs being performed in paralell.

The function run_talys(directories) in "example.R" script tries to start 10 workers by default. The number of workers acctually started depends on the mpi interface, it will never be more than n-1 where n is the number of availible workers.

To run the example, do

mpirun -np 1 Rscript --vanilla example.R

observe that you need to start it with only 1 worker (-np 1), "start_mpi_worker" takes care of spawning the workers. Setting -np to a larger number will result in an error.

TO DO:

1) Want to implement that the workers not only start a job in a given directory but also

	1.1) definetely: parses the output given a certain needed output, passed from the calling R script, and gives it back to the master thread. In turn, the master thread will collect all the outputs and pass it back to the calling R script.

	1.2) maybe: takes care of preparing the talys input files given some directions from the calling R script. This may however be better left to the user for a larger amount of flexibility in usage.
