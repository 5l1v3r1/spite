#include "OutputResults.hpp"

#include <fstream>


namespace SPTE_Proxy
{
void writeSPTEResults(double taskTime[], double computeTime[], SPTE_Proxy::RunConfig & config, int nContributors)
{
		std::ofstream resFile("SPTE_Results.out", std::ofstream::out | std::ofstream::app);
		//resFile << "#TaskTime\tCommDims\tDepSize\tnProcs\tComputeTime\tTaskSize\tNumIters" << std::endl;
		for(int i = 0; i < nContributors; i++)
		{
			resFile << taskTime[i] << "\t" << config.dim << "\t" <<  config.depSize << "\t" << nContributors <<  "\t";
			resFile << computeTime[i] << "\t" << config.taskSize << "\t";
			resFile << config.numIters << std::endl;
		}
		resFile.close();
}
}
