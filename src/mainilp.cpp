/*
 * mainvisualize.cpp
 *
 *  Created on: 19-jan-2019
 *      Author: M. El-Kebir
 */

#include "utils.h"
#include <lemon/arg_parser.h>
#include "clonetree.h"
#include "parentchildgraph.h"
#include "mctsolverilp.h"

int main(int argc, char** argv)
{
  int k = 0;
  std::string resultspath;
  std::string clusteringStr;
  
  int timeLimit = -1;
  int memoryLimit = -1;
  int nrThreads = 1;
  bool verbose = false;
  
  lemon::ArgParser ap (argc, argv);
  ap.other("trees", "Input trees");
  ap.refOption("k", "Number of clusters", k, true);
  ap.refOption("p", "Path to results (make it unique)", resultspath, true);
  ap.refOption("c", "Clustering", clusteringStr);
  ap.refOption("v", "Verbose output (default: False)", verbose);
  ap.refOption("T", "Number of threads (default: 1)", nrThreads);
  ap.refOption("t", "Time limit in seconds (default: -1, no time limit)", timeLimit);
  ap.refOption("m", "Memory limit in MB (default: -1, no memory limit)", memoryLimit);
  ap.parse();
  
  if (ap.files().size() != 1){
    std::cerr << "Error <trees> must be specified" << std::endl;
    return 1;
  }
  
  std::string filenameTrees = ap.files()[0];
  
  CloneTreeVector ctv;
  try{
    if (filenameTrees != "-"){
      std::ifstream inTrees(filenameTrees.c_str());
      if (!inTrees.good()){
        std::cerr << "Error: could not open '" << filenameTrees << "' for reading" << std::endl;
        return 1;
      }
      inTrees >> ctv;
      inTrees.close();
    }
    else {
      std::cin >> ctv;
    }
  }
  catch (std::runtime_error& e){
    std::cerr << "Error: " << e.what() << std::endl;
    return 1;
  }
  
  MCTSolverILP solver(ctv, k, timeLimit, memoryLimit, nrThreads, verbose);
  
  if (clusteringStr.empty()){
    MCTSolver::run(solver, resultspath);
  }
  else{
    StringVector s;
    boost::split(s, clusteringStr, boost::is_any_of(","));
    
    if (s.size() != ctv.size()){
      std::cerr << "Error: expected " << ctv.size()
                << " clusters, got " << s.size() << std::endl;
      return 1;
    }
    IntVector clustering;
    for (const std::string& str : s){
      int j = boost::lexical_cast<int>(str);
      if (0 <= j && j < k){
        clustering.push_back(j);
      }
      else{
        std::cerr << "Error: expected integer between 0 and " << k-1 << ", got " << str << std::endl;
        return 1;
      }
    }
    
    solver.setClustering(clustering);
    
    char buf[1024];
    for (int j = 0; j < k; ++j)
    {
      snprintf(buf, 1024, "%s_cluster%d.dot", resultspath.c_str(), j);
      std::ofstream outFile(buf);
      solver.getConsensus(j)->writeDOT(outFile,
                                       solver.getClusterNrTrees(j),
                                       solver.getClusterCost(j));
      outFile.close();
    }
  }
}

