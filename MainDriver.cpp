#include "TaskGranularityProxy.hpp"

#include <mpi.h>
#include <cstring>
#include <cstdlib>
#include <chrono>

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
	retVal.numIters = 16;

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
			std::cout << "-iters=[number of iterations of test to run for averaged time]" << std::endl;
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
			else if(strcmp(argname, "-iters") == 0)
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

	//Set up 2D Cartesian Topology if needed (p2p4)
	int dims[2];
	int periods[2] = {1, 1};
	int cartRank;
	int coords[2];
	MPI_Comm cartComm;
	if(runConfig.depType == SPTE_Proxy::P2P4)
	{
		MPI_Dims_create(nProcs, 2, dims);
		MPI_Cart_create(MPI_COMM_WORLD, 2, dims, periods, 1, &cartComm);
		MPI_Comm_rank(cartComm, &cartRank);
		MPI_Cart_coords(cartComm, cartRank, 2, coords);
	}

	//Initialize timers
	double taskTimer = 0.0;
	double computeTimer = 0.0;

	//Pre-seed first round of inputs
	///TODO: pre-seed first round of inputs

	//Add an initial barrier before all work to maximize contention
	MPI_Barrier(MPI_COMM_WORLD);

	//Run numIters times
	for(int t = 0; t < runConfig.numIters; t++)
	{
		//Start outer (full task) timer
		std::chrono::time_point<std::chrono::system_clock> oStart = std::chrono::system_clock::now();

		//Get inputs if needed
		if(runConfig.depType == SPTE_Proxy::P2P2)
		{
			//Recv from rank -1 and +1
			///TODO
		}
		else if(runConfig.depType == SPTE_Proxy::P2P4)
		{
			//Use cart coords to recv +/-1x and +/-1y
			///TODO
		}

		//Start inner (compute) timer
		std::chrono::time_point<std::chrono::system_clock> iStart = std::chrono::system_clock::now();

		//Execute pseudo-task
		///TODO

		//End inner (compute) timer
		std::chrono::time_point<std::chrono::system_clock> iEnd = std::chrono::system_clock::now();
		std::chrono::duration<double> iDiff = iEnd-iStart;
		computeTimer += iDiff.count();

		//Send outputs to be inputs for next round
		if(runConfig.depType == SPTE_Proxy::P2P2)
		{
			//Send to rank -1 and +1
			///TODO
		}
		else if(runConfig.depType == SPTE_Proxy::P2P4)
		{
			//Use cart coords to send to +/-1x and +/-1y
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
	double globalTaskTime, globalComputeTime;

	//Reduce on timers
	MPI_Reduce(&taskTimer, &globalTaskTime, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);
	MPI_Reduce(&computeTimer, &globalComputeTime, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);

	//If rank 0, output times
	if(rank == 0)
	{
		std::cout << "Max Avg Task Time\t" << globalTaskTime << std::endl;
		std::cout << "Max Avg Compute Time\t" << globalComputeTime << std::endl;
	}

	//Add a superfluous barrier to ensure that all work is done
	MPI_Barrier(MPI_COMM_WORLD);
	//Finalize MPI
	MPI_Finalize();
	return 0;
}

