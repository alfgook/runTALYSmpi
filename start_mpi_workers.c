/* manager */ 
#include "mpi.h"
#include <R.h>

#define MIN(x,y) ((x<y)?x:y)

int start_mpi_workers(const int *number_of_workers, const char **worker_program, char **argv) 
{ 
   printf("argv = %s\n",argv[0]);
   int world_size, universe_size, *universe_sizep, flag; 
   MPI_Comm everyone;           /* intercommunicator */ 

   int nbr_of_workers = (*number_of_workers);

   int argc = 1;
   MPI_Init(&argc, &argv); 
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
   printf("nbr_of_workers = %d\n",nbr_of_workers);
   MPI_Comm_spawn(worker_program[0], argv, nbr_of_workers, MPI_INFO_NULL, 0, MPI_COMM_SELF, &everyone, MPI_ERRCODES_IGNORE); 

   // Could add parallel code here. The communicator "everyone" can be used to communicate with
   // the spawned processes, which have ranks 0,.. MPI_UNIVERSE_SIZE-1 in the remote group of 
   // the intercommunicator "everyone". 

   MPI_Finalize(); 
   return 0; 
} 