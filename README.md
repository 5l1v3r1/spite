SPITE: Synthetic Proxy Infrastructure for Task Evaluation
=======

Description
-------
The Synthetic Proxy Infrastructure for Task Evaluation is a proxy application designed to support application developers in gauging the performance of various task granularities when determining how best to utilize task based programming models.  The infrastructure is designed to provide examples of common communication patterns with a synthetic workload intended to provide performance data to evaluate programming model and platform overheads for the purpose of determining task granularity for task decomposition purposes.

This is presented as a reference implementation of a proxy application with run-time configurable input and output task dependencies ranging from an embarrassingly parallel scenario to patterns with stencil-like dependencies upon their nearest neighbors. Once all, if any, inputs are satisfied each task will execute a synthetic workload (a simple DGEMM of  in this case) of varying size and output all, if any, outputs to the next tasks. The intent is for this reference implementation to be implemented as a proxy app in different programming models so as to provide the same infrastructure and to allow for application developers to simulate their own communication needs to assist in task decomposition under various models on a given platform.


Installation
-----
TODO

Use
-----
TODO

Copyright and License
------
Los Alamos National Security, LLC (LANS) owns the copyright to SPITE, which it identifies internally as LA-CC-C17058.  The license is BSD-ish with a "modifications must be indicated" clause.  See [LICENSE.md](https://github.com/lanl/spite/blob/master/LICENSE.md) for the full text.

Contact
-----
Robert Pavel,  [_rspavel@lanl.gov_](mailto:rspavel@lanl.gov)
