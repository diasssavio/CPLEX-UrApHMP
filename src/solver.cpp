/*
 * solver.cpp
 *
 *  Created on: Aug 21, 2015
 *      Author: savio
 */

#include "../include/solver.h"

solver::solver(model* _mod1) : mod1(_mod1), IloCplex(*_mod1) {
  obj_value = 0.0;
  z = IloNumArray2(getEnv());
  f = IloNumArray4(getEnv());
  mod2 = NULL;
}

solver::solver(model2* _mod2) : mod2(_mod2), IloCplex(*_mod2) {
	obj_value = 0.0;
  z = IloNumArray2(getEnv());
  w = IloNumArray2(getEnv());
  x = IloNumArray3(getEnv());
  y = IloNumArray3(getEnv());
  mod1 = NULL;
}

solver::~solver() { }

void solver::run(double tl, double UB, bool first) {
	setParam(IloCplex::Threads, 1);
	setParam(IloCplex::Param::Preprocessing::Aggregator, 0);
	setParam(IloCplex::Param::Preprocessing::Presolve, 0);
	setParam(IloCplex::PreInd, IloFalse);
	setParam(IloCplex::Param::TimeLimit, tl);
	setParam(IloCplex::Param::MIP::Tolerances::UpperCutoff, UB);
	if(first)
		setParam(IloCplex::Param::MIP::Limits::Solutions, 1);
	// CPLEX_PARAM_MIPEMPHASIS to feasibility emphasis
	setParam(IloCplex::Param::Emphasis::MIP, 1);

	// cplex.exportModel("test.lp");

	solve();

	if(getStatus() == IloAlgorithm::Status::Optimal || getStatus() == IloAlgorithm::Status::Feasible){
		if(mod1 != NULL) run1();
		else run2();
	}
}

void solver::run1() {
	int n = mod1->instance.get_n();

	// Getting results
	for(IloInt i = 0; i < n; i++){
		IloNumArray aux1(getEnv());
		IloNumArray3 aux2(getEnv());
		for(IloInt j = 0; j < n; j++){
			aux1.add(getValue(mod1->z[i][j]));
			IloNumArray2 aux3(getEnv());
			for(IloInt k = 0; k < n; k++){
				IloNumArray aux4(getEnv());
				for(IloInt l = 0; l < n; l++)
					aux4.add(getValue(mod1->f[i][j][k][l]));
				aux3.add(aux4);
			}
			aux2.add(aux3);
		}
		z.add(aux1);
		f.add(aux2);
	}

	obj_value = getObjValue();
}

void solver::run2() {
	int n = mod2->instance.get_n();

	// Getting results
	for(IloInt i = 0; i < n; i++){
		IloNumArray aux1(getEnv());
		IloNumArray aux2(getEnv());
		IloNumArray2 aux3(getEnv());
		IloNumArray2 aux4(getEnv());
		for(IloInt j = 0; j < n; j++){
			aux1.add(getValue(mod2->z[i][j]));
			aux2.add(getValue(mod2->w[i][j]));
			IloNumArray aux5(getEnv());
			IloNumArray aux6(getEnv());
			for(IloInt k = 0; k < n; k++){
				aux5.add(getValue(mod2->x[i][j][k]));
				aux6.add(getValue(mod2->y[i][j][k]));
			}
			aux3.add(aux5);
			aux4.add(aux6);
		}
		z.add(aux1);
		w.add(aux2);
		x.add(aux3);
		y.add(aux4);
	}

	obj_value = getObjValue();
}