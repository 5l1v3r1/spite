#include "TaskGranularityProxy.hpp"
#include "SyntheticTask.hpp"

#include <mpi.h>
#include <cstring>
#include <cstdlib>
#include <chrono>

#include <iostream>
#include <fstream>

namespace SPTE_Proxy
{

SPTE_Proxy::RunConfig processArgs(int argc, char ** &argv)
{
	RunConfig retVal;
	//Set default values
	retVal.dim = 0;
	retVal.depSize = 0;
	retVal.taskSize = 128;
	retVal.numIters = 16;

	//For each argument
	for(int i = 1; i < argc; i++)
	{
		//Make a copy of the string that we can destroy
		char argb[256];
		strcpy(argb, argv[i]);
		//Split string to collect the name and value
		char * argname = std::strtok(argv[i], "=");
		if(std::strrchr(argname, '-') != nullptr)
		{
			argname = std::strrchr(argname, '-')+1;
		}
		char * argval = std::strtok(nullptr, "=");
		//Now for a simple switch to set values
		if(strcmp(argname, "help") == 0)
		{
			std::cout << "-dim=[integer number of dimensions in cartesian space]" << std::endl;
			std::cout << "-arg=[integer size of arguments/dependencies in chars]" << std::endl;
			std::cout << "-task=[integer size of one dimension of task matrix]" << std::endl;
			std::cout << "-iters=[number of iterations of test to run for averaged time]" << std::endl;
		}
		if(argval != nullptr)
		{
			if(strcmp(argname, "dim") == 0)
			{
				retVal.dim = atoi(argval);
			}
			else if(strcmp(argname, "arg") == 0)
			{
				retVal.depSize = atoi(argval);
			}
			else if(strcmp(argname, "task") == 0)
			{
				retVal.taskSize = atoi(argval);
			}
			else if(strcmp(argname, "iters") == 0)
			{
				retVal.numIters = atoi(argval);
			}
		}
	}
	return retVal;
}

}

int main(int argc, char ** argv)
{
	using SPTE_Proxy::processArgs;

	//Initialize MPI
	MPI_Init(&argc, &argv);

	//Get standard size and rank data
	int nProcs, rank;
	MPI_Comm_size(MPI_COMM_WORLD, &nProcs);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	//Parse arguments
	SPTE_Proxy::RunConfig runConfig = processArgs(argc, argv);

	//Set up Cart topo if not 0-D
	int dims[runConfig.dim];
	int periods[runConfig.dim];
	for(int i = 0; i < runConfig.dim; i++)
	{
		periods[i] = 1;
	}
	int cartRank;
	int coords[runConfig.dim];
	MPI_Comm cartComm;
	///TODO: Test cart create with 0 dimensions. Probably keep the if
	if(runConfig.dim != 0)
	{
		MPI_Dims_create(nProcs, runConfig.dim, dims);
		MPI_Cart_create(MPI_COMM_WORLD, runConfig.dim, dims, periods, 1, &cartComm);
		MPI_Comm_rank(cartComm, &cartRank);
		MPI_Cart_coords(cartComm, cartRank, runConfig.dim, coords);
	}

	//Initialize timers
	double taskTimer = 0.0;
	double computeTimer = 0.0;

	//Pre-seed first round of inputs
	///TODO: pre-seed first round of inputs
	if(runConfig.depSize != 0)
	{
		///TODO
	}



	//Add an initial barrier before all work to maximize contention
	MPI_Barrier(MPI_COMM_WORLD);

	//Run numIters times
	for(int t = 0; t < runConfig.numIters; t++)
	{
		//Start outer (full task) timer
		std::chrono::time_point<std::chrono::system_clock> oStart = std::chrono::system_clock::now();

		//Get inputs if needed
		if(runConfig.depSize != 0)
		{
			///TODO
		}

		//Start inner (compute) timer
		std::chrono::time_point<std::chrono::system_clock> iStart = std::chrono::system_clock::now();

		//Execute pseudo-task
		SPTE_Proxy::performSyntheticWorkload(runConfig);

		//End inner (compute) timer
		std::chrono::time_point<std::chrono::system_clock> iEnd = std::chrono::system_clock::now();
		std::chrono::duration<double> iDiff = iEnd-iStart;
		computeTimer += iDiff.count();

		//Send outputs to be inputs for next round
		if(runConfig.depSize !=0)
		{
			///TODO
		}

		//End outer (full task) timer
		std::chrono::time_point<std::chrono::system_clock> oEnd = std::chrono::system_clock::now();
		std::chrono::duration<double> oDiff = oEnd-oStart;
		taskTimer += oDiff.count();
	}

	//Average Timers
	taskTimer = taskTimer / runConfig.numIters;
	computeTimer = computeTimer / runConfig.numIters;

	//Reduce on timers
	double * globalTaskTime, * globalComputeTime;
	if(rank == 0)
	{
		globalTaskTime = new double[nProcs];
		globalComputeTime = new double[nProcs];
	}
	MPI_Gather(&taskTimer, 1, MPI_DOUBLE, globalTaskTime, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
	MPI_Gather(&computeTimer, 1, MPI_DOUBLE, globalComputeTime, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
	//If rank 0, output times
	if(rank == 0)
	{
		std::ofstream resFile("SPTE_Results.out");
		resFile << "#Task\tCompute" << std::endl;
		for(int i = 0; i < nProcs; i++)
		{
			resFile << globalTaskTime[i] << "\t" << globalComputeTime[i] << std::endl;
		}
		resFile.close();
	}

	//Add a superfluous barrier to ensure that all work is done
	MPI_Barrier(MPI_COMM_WORLD);
	//Finalize MPI
	MPI_Finalize();
	return 0;
}

