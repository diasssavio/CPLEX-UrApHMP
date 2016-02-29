/*
 * model.h
 *
 *  Created on: Aug 21, 2015
 *      Author: savio
 */

#ifndef SRC_MODEL2_H_
#define SRC_MODEL2_H_

#include <sstream>
#include <ilcplex/ilocplex.h>

#include "solution.h"
#include "UrApHMP.h"
#include "typedef.hpp"

class model2 : public IloModel {
private:
	void init(); // Initialize all variables for problem
	void add_const(); // Add constraints to model
	void add_obj(); // Add objective function to model

public:
	model2(IloEnv, uraphmp&, solution&);
	virtual ~model2();

	// Instance & Solution
	uraphmp& instance;
	solution& sol;

	// Variables
	// traffic proportion
	IloNumVarArray2 w;
	IloNumVarArray3 x;
	IloNumVarArray3 y;

	// Assigned Hubs
	IloNumVarArray2 z;
};

#endif /* SRC_MODEL2_H_ */
