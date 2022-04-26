/* manager */ 
#include "mpi.h"
#include "unistd.h"
#include <R.h>

#define MIN(x,y) ((x<y)?x:y)

int initalize_mpi() {
   MPI_Init(NULL, NULL);
   printf("mpi session initalized\n");
   return 0;
}

int finalize_mpi() {
   int init_flag;
   MPI_Initialized(&init_flag);
   if(init_flag) {
      MPI_Finalize();
      printf("the mpi session has been finalized\n");
   }
   return 0;
}

int start_mpi_workers(const char **worker_program, char *job_list[], const int *number_of_jobs, const int *number_of_workers) 
{ 

   //MPI_Init(NULL, NULL);  

   int world_size, universe_size, *universe_sizep, flag; 
   MPI_Comm everyone;           /* intercommunicator */ 

   int nbr_of_workers = (*number_of_workers);
   int nbr_of_jobs = (*number_of_jobs);

   MPI_Comm_size(MPI_COMM_WORLD, &world_size);

   if (world_size != 1) {
      printf("Top heavy with management");
      return 1;
   }

   //MPI_Attr_get(MPI_COMM_WORLD, MPI_UNIVERSE_SIZE, &universe_sizep, &flag);
   MPI_Comm_get_attr(MPI_COMM_WORLD, MPI_UNIVERSE_SIZE, &universe_sizep, &flag);

   if (!flag) { 
      printf("This MPI does not support UNIVERSE_SIZE."); 
      return 1;
   } else {
      universe_size = *universe_sizep;
   }

   //printf("universe_size = %d\n",universe_size);
   if (universe_size == 1) {
      printf("No room to start workers");
      return 1;
   }

   nbr_of_workers = MIN(nbr_of_workers,universe_size-1);
   printf("nbr_of_workers = %d\n",nbr_of_workers);
   //printf("job_list[0] = %s\n",job_list[0]);

   // copy the list of jobs from the calling R-script and add a NULL pointer at the end
   // this is required by MPI_Comm_spawn to calculate the number of arguments
   char **argv = (char **) malloc((nbr_of_jobs+1) * sizeof(char *));
   for(int i=0;i<nbr_of_jobs;i++) {
      argv[i] = job_list[i];
   }
   argv[nbr_of_jobs] = NULL;

/*
   // create info object
   MPI_Info info;
   MPI_Info_create(&info);
   // add /usr/local/bin to the info object
   MPI_Info_set(info,"wdir","/usr/local/bin");
   // apparently I can only have one value for wdir, so it needs to be known at compile time

   MPI_Comm_spawn(worker_program[0],
                     argv,
                     nbr_of_workers,
                     info, 
                     0,
                     MPI_COMM_SELF,
                     &everyone,
                     MPI_ERRCODES_IGNORE);
*/

   MPI_Comm_spawn(worker_program[0],
                     argv,
                     nbr_of_workers,
                     MPI_INFO_NULL, 
                     0,
                     MPI_COMM_SELF,
                     &everyone,
                     MPI_ERRCODES_IGNORE);
   
   // Could add parallel code here. The communicator "everyone" can be used to communicate with
   // the spawned processes, which have ranks 0,.. nbr_of_workers-1 in the remote group of 
   // the intercommunicator "everyone".
   // I need to add a wait for the workers to finish thier tasks here!!!
   //MPI_Barrier(everyone);

   printf("workers spawned... waiting...");
   for(int worker=0;worker<nbr_of_workers;worker++) {
      int rank;
      MPI_Recv(&rank, 1, MPI_INT, worker, 0, everyone, MPI_STATUS_IGNORE);
      //printf("master received %d from worker %d\n",rank,worker);
   }
   

   free(argv);
   //MPI_Finalize();
   return 0; 
} 