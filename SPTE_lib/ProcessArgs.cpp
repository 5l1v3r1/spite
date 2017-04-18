#include "ProcessArgs.hpp"

#include <cstdlib>
#include <iostream>
#include <cstring>

namespace SPTE_Proxy
{

SPTE_Proxy::RunConfig processArgs(int argc, char ** &argv)
{
	RunConfig retVal;
	//Set default values
	retVal.dim = 1;
	retVal.depSize = 16;
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
