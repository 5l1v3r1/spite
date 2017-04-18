#ifndef TASKGRANULARITYPROXY_HPP
#define TASKGRANULARITYPROXY_HPP

namespace SPTE_Proxy
{
	const int GENERIC_MESSAGE_TAG = 71;

	struct RunConfig
	{
		unsigned int depSize;
		unsigned int taskSize;
		unsigned int numIters;
		unsigned int dim;
	};

}

#endif

