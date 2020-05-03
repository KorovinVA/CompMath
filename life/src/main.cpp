#include <mpi.h>
#include <iostream>
#include "Slave.h"
#include "Master.h"

int main(int argc, char* argv[])
{
	int Prank = 0;
	int Psize = 0;
	
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &Prank);
	MPI_Comm_size(MPI_COMM_WORLD, &Psize);

	if (Prank != 0)
	{
		Slave proc(Prank, Psize);
		proc.run();
	}
	else
	{
		Master proc(Psize);
		proc.run();
	}
	std::cout << "Process number " << Prank << " has exited succesfully." << std::endl;
	MPI_Finalize();
	return 0;
}