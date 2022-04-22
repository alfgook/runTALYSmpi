/* manager */ 
#include "mpi.h"
#include <R.h>

#define MIN(x,y) ((x<y)?x:y)

int start_mpi_workers(const char **worker_program, char *job_list[], const int *number_of_jobs, const int *number_of_workers) 
{ 

   MPI_Init(NULL, NULL);  

   int world_size, universe_size, *universe_sizep, flag; 
   MPI_Comm everyone;           /* intercommunicator */ 

   int nbr_of_workers = (*number_of_workers);
   int nbr_of_jobs = (*number_of_jobs);

   MPI_Comm_size(MPI_COMM_WORLD, &world_size);

   if (world_size != 1) {
      printf("Top heavy with management");
      return 1;
   }

   MPI_Attr_get(MPI_COMM_WORLD, MPI_UNIVERSE_SIZE, &universe_sizep, &flag);

   if (!flag) { 
      printf("This MPI does not support UNIVERSE_SIZE."); 
      return 1;
   } else {
      universe_size = *universe_sizep;
   }

   if (universe_size == 1) {
      printf("No room to start workers");
      return 1;
   }

   nbr_of_workers = MIN(nbr_of_workers,universe_size-1);

   // copy the list of jobs from the calling R-script and add a NULL pointer at the end
   // this is required by MPI_Comm_spawn to calculate the number of arguments
   char **argv = (char **) malloc((nbr_of_jobs+1) * sizeof(char *));
   for(int i=0;i<nbr_of_jobs;i++) {
      argv[i] = job_list[i];
      // printf("argv[%d] = %s\n",i,argv[i]);
   }
   argv[nbr_of_jobs] = NULL;
   MPI_Comm_spawn(worker_program[0],
                     argv,
                     nbr_of_workers,
                     MPI_INFO_NULL,
                     0,
                     MPI_COMM_SELF,
                     &everyone,
                     MPI_ERRCODES_IGNORE);
   
   // Could add parallel code here. The communicator "everyone" can be used to communicate with
   // the spawned processes, which have ranks 0,.. MPI_UNIVERSE_SIZE-1 in the remote group of 
   // the intercommunicator "everyone".

   free(argv);
   MPI_Finalize();
   return 0; 
} 