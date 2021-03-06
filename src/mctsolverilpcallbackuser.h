/*
 * mctsolverilpcallbackuser.h
 *
 *  Created on: 20-jan-2019
 *      Author: M. El-Kebir
 */

#ifndef MCTSOLVERILPCALLBACKUSER_H
#define MCTSOLVERILPCALLBACKUSER_H

#include "mctsolverilpcallback.h"

class MctSolverIlpCallbackUser : public MctSolverIlpCallback, public IloCplex::UserCutCallbackI
{
public:
  MctSolverIlpCallbackUser(IloEnv env,
                           int k,
                           const BoolMatrix& b,
                           const StringVector& indexToMutation,
                           IloNumVar3Matrix y,
                           IloNumVarMatrix z);
  
  MctSolverIlpCallbackUser(const MctSolverIlpCallbackUser& other);
  
  void main()
  {
//    std::cerr << "user" << std::endl;
    if (_nodeNumber != getNnodes())
    {
      _nodeNumber = getNnodes();
      _cutCount = 0;
      //      _makeAttempt = _backOff.makeAttempt();
    }
    if (_cutCount < 50)
    {
      separate(getEnv());
      ++_cutCount;
    }
  }
  
  IloCplex::CallbackI *duplicateCallback() const
  {
    return (new (getEnv()) MctSolverIlpCallbackUser(*this));
  }
  
protected:
  virtual void updateVarsVals();
  virtual void addConstraint(IloExpr expr);
};

#endif // MCTSOLVERILPCALLBACKUSER_H
