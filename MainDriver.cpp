#include "TaskGranularityProxy.hpp"

#include <mpi.h>
#include <cstring>
#include <cstdlib>

#include <iostream>

namespace SPTE_Proxy
{

SPTE_Proxy::RunConfig processArgs(int argc, char ** &argv)
{
	RunConfig retVal;
	//Set default values
	retVal.depType = EMBARASSING;
	retVal.depSize = 0;
	retVal.taskSize = 128;

	//For each argument
	for(int i = 1; i < argc; i++)
	{
		//Make a copy of the string that we can destroy
		char argb[256];
		strcpy(argb, argv[i]);
		//Split string to collect the name and value
		char * argname = std::strtok(argv[i], "=");
		char * argval = std::strtok(nullptr, "=");
		//Now for a simple switch to set values
		if(strcmp(argname, "help") == 0)
		{
			std::cout << "-dep=[EMBARASSING,P2P2,P2P4]" << std::endl;
			std::cout << "-arg=[integer size of arguments/dependencies in chars]" << std::endl;
			std::cout << "-task=[integer size of one dimension of task matrix]" << std::endl;
		}
		if(argval != nullptr)
		{
			if(strcmp(argname, "-dep") == 0)
			{
				if(strcmp(argval, "EMBARASSING") == 0)
				{
					retVal.depType = EMBARASSING;
				}
				else if(strcmp(argval, "PSP2") == 0)
				{
					retVal.depType = P2P2;
				}
				else if(strcmp(argval, "P2P4") == 0)
				{
					retVal.depType = P2P4;
				}
			}
			else if(strcmp(argname, "-arg") == 0)
			{
				retVal.depSize = atoi(argval);
			}
			else if(strcmp(argname, "-task") == 0)
			{
				retVal.taskSize = atoi(argval);
			}
		}

	}
	return retVal;
}

}

int main(int argc, char ** argv)
{
	using SPTE_Proxy::RunConfig;
	using SPTE_Proxy::processArgs;

	//Initialize MPI
	MPI_Init(&argc, &argv);

	//Get standard size and rank data
	int nProcs, rank;
	MPI_Comm_size(MPI_COMM_WORLD, &nProcs);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	//Parse arguments
	///TODO: Probably a better argument parser
	RunConfig runConfig = processArgs(argc, argv);
	

	//Add a superfluous barrier to ensure that all work is done
	MPI_Barrier(MPI_COMM_WORLD);
	//Finalize MPI
	MPI_Finalize();
	return 0;
}

