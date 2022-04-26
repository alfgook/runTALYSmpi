#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <iostream>
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
    int nbr_of_ranks, rank;  
    MPI_Status Stat;

    MPI_Comm parent;
    int parent_size;

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

    //MPI_Init(&argc,&argv);
    MPI_Init(NULL,NULL);
    MPI_Comm_get_parent(&parent);
    if( parent == MPI_COMM_NULL) {
        std::cerr << "No parent" << std::endl;
        exit(1);
    }

    MPI_Comm_remote_size(parent, &parent_size); 
    if (parent_size != 1) {
        std::cerr << "Something's wrong with the parent" << std::endl;
        exit(1);
    } 

    MPI_Comm_size(MPI_COMM_WORLD, &nbr_of_ranks);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    std::cout << "worker " << rank << "/" << nbr_of_ranks << " : checking wd " << std::endl;
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
    std::cout << "worker " << rank << "/" << nbr_of_ranks << " : wd ok " << std::endl;

    MPI_Barrier(MPI_COMM_WORLD);
    // perform the calculations
    job_to_do = rank + 1;
    while(job_to_do <= nbr_of_jobs) {
        // move into the directory of the job to do
        chdir(argv[job_to_do]);

        // perform the talys calculation
        system("talys < input > output");
        char wd[512];
        getcwd(wd,512);
        std::cout << "worker " << rank << "/" << nbr_of_ranks << " : " << wd << std::endl;

        // get the next job
        job_to_do += nbr_of_ranks;
    }
    
    //send a message to the parent
    MPI_Send(&rank, 1, MPI_INT, 0, 0, parent);

    MPI_Finalize();

    exit(0);
}