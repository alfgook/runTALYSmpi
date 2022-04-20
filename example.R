dyn.load(paste0("start_mpi_workers", .Platform$dynlib.ext));

run_talys = function(directories)
{
	.C("start_mpi_workers",as.integer(10),as.character("runTALYSmpi"),as.character(directories));
}

print("The master will now create some slave processes...")
dirs <- c("run1","run2","run3","run4","run5","run6","run7")

dirs <- paste0(paste0(getwd(),"/"),dirs)
#print(dirs)

run_talys(dirs)
