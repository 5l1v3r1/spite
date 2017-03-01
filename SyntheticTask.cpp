#include "SyntheticTask.hpp"


void SPTE_Proxy::performSyntheticWorkload(SPTE_Proxy::RunConfig &runConfig)
{
	///TODO: Investigate to ensure this is not getting optimized away...
	double A[runConfig.taskSize*runConfig.taskSize];
	double B[runConfig.taskSize*runConfig.taskSize];
	double C[runConfig.taskSize*runConfig.taskSize];

	unsigned int dimX = runConfig.taskSize;

	for(volatile int i = 0; i < dimX; i++)
	{
		for(volatile int j = 0; j < dimX; j++)
		{
			for(volatile int k = 0; k < dimX; k++)
			{
				C[i + j*dimX] += A[i + k*dimX] + B[k+j*dimX];
			}
		}
	}
}


