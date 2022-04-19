#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>

// --------------------------------------------------------------------------------------
//
//      A very simplistic program to start talys calculations using an MPI interface.
//      provide a list of directories that each include a talys input file named input,
//      the paths provided should be absolute paths. This program will execute the jobs
//      in parallel.
//
// --------------------------------------------------------------------------------------

bool PathExists(const std::string &s)
{
  struct stat buffer;
  return (stat (s.c_str(), &buffer) == 0);
}

int main(int argc, char** argv) //int argc; char *argv[];
{
    if(argc == 1) {
        std::cout << "no input files provided \n" <<
         "example useage:\n" <<
         "mpirun -np 3 /dev/shm/run1 /dev/shm/run2 /dev/shm/run3 /dev/shm/run4: \n" <<
         "This command would run the 4 calculations on 3 processors.\n" <<
         "Note the use of a different directory for each calculation. \n" <<
         "Also note the use of the memory resident /dev/shm to perform calculations, \n" <<
         "you may place the directories anywhere, but the use of /dev/shm greatly  \n" <<
         "improves performance. " << std::endl;
    }
    int nbr_of_ranks, rank;  
    MPI_Status Stat;

    MPI_Init(&argc,&argv);
    MPI_Comm_size(MPI_COMM_WORLD, &nbr_of_ranks);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    // first check that all the input files exists and that
    // all paths were provided as absolute paths
    int nbr_of_jobs = argc - 1;
    int job_to_do = rank + 1;
    while(job_to_do <= nbr_of_jobs) {
        if(argv[job_to_do][0]!='/') {
            std::cerr << "all paths must be provided as absolute paths" << std::endl;
            std::cerr << argv[job_to_do] << " is not an absolute path" << std::endl;
            exit(1);
        }
        std::string input_file(argv[job_to_do]);
        input_file += "/input";
        if(!PathExists(input_file)) {
            std::cerr << "no input file: " << input_file << std::endl;
            exit(1);
        }
        job_to_do += nbr_of_ranks;
    }

    MPI_Barrier(MPI_COMM_WORLD);
    // perform the calculations
    job_to_do = rank + 1;
    while(job_to_do <= nbr_of_jobs) {
        // move into the directory of the job to do
        chdir(argv[job_to_do]);

        // perform the talys calculation
        system("talys < input > output");

        // get the next job
        job_to_do += nbr_of_ranks;
    }
    
    MPI_Finalize();

    exit(0);
}