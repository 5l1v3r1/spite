#ifndef TASKGRANULARITYPROXY_HPP
#define TASKGRANULARITYPROXY_HPP

namespace SPTE_Proxy
{
	enum DepType
	{
		EMBARASSING, P2P2, P2P4
	};

	struct RunConfig
	{
		DepType depType;
		unsigned int depSize;
		unsigned int taskSize;
		unsigned int numIters;
	};

}

#endif

