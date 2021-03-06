/*
 * mctsolverilpcallbackuser.cpp
 *
 *  Created on: 20-jan-2019
 *      Author: M. El-Kebir
 */

#include "mctsolverilpcallbackuser.h"

MctSolverIlpCallbackUser::MctSolverIlpCallbackUser(IloEnv env,
                                                   int k,
                                                   const BoolMatrix& b,
                                                   const StringVector& indexToMutation,
                                                   IloNumVar3Matrix y,
                                                   IloNumVarMatrix z)
  : MctSolverIlpCallback(k, b, indexToMutation, y, z)
  , IloCplex::UserCutCallbackI(env)
{
  init(getEnv());
}

MctSolverIlpCallbackUser::MctSolverIlpCallbackUser(const MctSolverIlpCallbackUser& other)
  : MctSolverIlpCallback(other)
  , IloCplex::UserCutCallbackI(other)
{
  init(getEnv());
}

void MctSolverIlpCallbackUser::updateVarsVals()
{
  getValues(_values, _variables);
}

void MctSolverIlpCallbackUser::addConstraint(IloExpr expr)
{
  add(1 <= expr, IloCplex::UseCutPurge).end();
}
