#ifndef OUTPUTRESULTS_HPP
#define OUTPUTRESULTS_HPP
#include "TaskGranularityProxy.hpp"

namespace SPTE_Proxy
{
void writeSPTEResults(double taskTime[], double computeTime[], SPTE_Proxy::RunConfig & config, int nContributors);
}
#endif
