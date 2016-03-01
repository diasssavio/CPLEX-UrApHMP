#include <cstdio>
#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <ilcplex/ilocplex.h>

#include "../include/FWChrono.h"
#include "../include/UrApHMP.h"
#include "../include/solution.h"
#include "../include/model.h"
#include "../include/model2.h"
#include "../include/typedef.hpp"

using namespace std;

template<typename T>
T string_to(const string& s){
	istringstream i(s);
	T x;
	if (!(i >> x)) return 0;
	return x;
}

template<typename T>
string to_string2(const T& t){
  stringstream ss;
  if(!(ss << t)) return "";
  return ss.str();
}

void drawing(ofstream&, double**, solution&);

ILOSTLBEGIN

// Is it the first integer feasible solution?
static bool first_sol = true;

// Time against the first integer feasible solution
static double first_time = 0.0;
// FWChrono timer;

// ILOUSERCUTCALLBACK2(CounterCall, IloNumVarArray2, z, FWChrono&, timer){
// 	if(first_sol && (getSolnPoolNsolns() == 1)){
// 		first_time = ((double) timer.getMilliSpan() / 1000);
// 		first_sol = false;
// 	}
// }

//ILOINCUMBENTCALLBACK2(CounterCall, IloNumVarArray2&, z, FWChrono&, timer){
/*ILOHEURISTICCALLBACK2(CounterCall, IloNumVarArray2&, z, FWChrono&, timer){
	IloEnv env = getEnv();
//	if(getStatus() == IloAlgorithm::Status::Feasible){
	if((getCplexStatus() == CPX_STAT_FEASIBLE) && first_time){
		first_time = ((double) timer.getMilliSpan() / 1000);
		first_sol = false;
		env.out() << "----------MIP Feasible solution----------" << endl;
		env.out() << "Nodes              : " << getNnodes()  << endl;
		env.out() << "Objective value    : " << getObjValue()  << endl;
		env.out() << "-----------------------------------------" << endl;
	}
}*/
ILOLAZYCONSTRAINTCALLBACK2(CounterCall, IloNumVarArray2&, z, FWChrono&, timer){
//ILOUSERCUTCALLBACK2(CounterCall, IloNumVarArray2&, z, FWChrono&, timer){
	if(first_sol) {
		IloEnv env = getEnv();
		IloInt n = z.getSize();

		try {
		  IloInt numberOfIInf = 0;
		  for (IloInt i = 0; i < n; i++)
		  	for (IloInt j = 0; j < n; j++)
				if (getFeasibility(z[i][j]) == Infeasible)
				  	numberOfIInf++;
		  cout << "Number of infeasibilities: " << numberOfIInf << endl;
		  
		  if (numberOfIInf == 0) {
			  first_time = ((double) timer.getMilliSpan() / 1000);
			  first_sol = false;
				env.out() << "----------MIP Feasible solution----------" << endl;
				env.out() << "Nodes              : " << getNnodes()  << endl;
				env.out() << "Objective value    : " << getObjValue()  << endl;
				env.out() << "-----------------------------------------" << endl;
			}
		} catch (IloException& e) {
		  env.out() << e << endl;
		  throw -1;
		} catch (...) {
		  throw -1;
		}
	}
}

int main(int argc, char* args[]){
	// Reading input file
	int p = string_to<int>(args[1]);
	int r = string_to<int>(args[2]);
	double X = string_to<double>(args[3]), alpha_1 = string_to<double>(args[4]), delta = string_to<double>(args[5]);
	
	int n;
	scanf("%d", &n);
	
	uraphmp instance(n, X, alpha_1, delta);

	vector< vector< double> > aux;
	for(int i = 0; i < n; i++){
		vector< double > aux2;
		for(int j = 0; j < n; j++){
			double temp;
			scanf("%lf", &temp);
			aux2.push_back(temp);
		}
		aux.push_back(aux2);
	}
	instance.set_traffics(aux);

	aux.clear();
	for(int i = 0; i < n; i++){
		vector< double > aux2;
		for(int j = 0; j < n; j++){
			double temp;
			scanf("%lf", &temp);
			aux2.push_back(temp);
		}
		aux.push_back(aux2);
	}
	instance.set_distances(aux);
	aux.clear();

	// Creating a solution object
	solution sol(instance, p, r);

	// Initializing cplex environment
	IloEnv env;

	try{
		model2 mod(env, instance, sol);
		IloCplex cplex(mod);
		cplex.setParam(IloCplex::Threads, 1);
		// cplex.setParam(IloCplex::NodeLim, 0);
		cplex.setParam(IloCplex::Param::Preprocessing::Aggregator, 0);
		cplex.setParam(IloCplex::Param::Preprocessing::Presolve, 0);
		cplex.setParam(IloCplex::PreInd, IloFalse);
		// cplex.exportModel("test.lp");
		
		FWChrono timer;
		timer.start();
		cplex.use(CounterCall(env, mod.z, timer));
		cplex.solve();
		timer.stop();

		// Getting the allocation optima from cplex solver
		double **z = (double**) malloc(sizeof(double*) * n);
		for(unsigned i = 0; i < n; i++){
			z[i] = (double*) malloc(sizeof(double) * n);
			for(unsigned j = 0; j < n; j++)
				z[i][j] = cplex.getValue(mod.z[i][j]);
		}

		// Drawing the allocation graph
		// drawer to_draw(z, sol);
		// ofstream dot_file("out.dot");
		// to_draw.draw(dot_file);

		// Drawing the allocation graph
		ofstream dot_file("out.dot");
		drawing(dot_file, z, sol);

		// Printing solution
		vector<IloNum> result;
		for(int i = 0; i < n; i++)
			result.push_back(cplex.getValue(mod.z[i][i]));
		cout << "Objective value = " << fixed << cplex.getObjValue() << endl << "Hubs: ";
		for(int i = 0; i < n; i++)
			if(result[i] != 0.0)
				cout << i + 1 << " ";
		cout << endl;
		printf("Total execution time: %.2lf\n", timer.getStopTime());

		// Calculating the number of used edges
		int count = 0;
		for(int i = 0; i < n; i++)
			for(int j = 0; j < n; j++)
				if((cplex.getValue(mod.z[i][j]) != 0.0) && (i != j))
					count++;
		printf("Number of used edges: %d\n", count + (p * (p-1) / 2));

		printf("Time of the first integer solution: %.2lf\n", first_time);
		printf("Number of integer solutions: %d\n", cplex.getSolnPoolNsolns());
		// for(int i = 0; i < n; i++){
		// 	for(int j = 0; j < n; j++)
		// 		printf("%.2lf  ", cplex.getValue(mod.z[i][j]));
		// 		// cout << cplex.getValue(mod.z[i][j]) << endl;
		// 	cout << endl;
		// }
	}catch(IloException& e){
		cerr << "Concert Exception" << e << endl;
	}
	// Closing the environment
	env.end();

	return 0;
}

void drawing(ofstream& out, double** z, solution& sol){
	out << "strict graph G {" << endl;
	
	// Drawing the nodes & hubs
	for(int i = 0; i < sol.get_instance().get_n(); i++){
		out << i << "[label=" << i+1 << "]";
		if(z[i][i] != 0.0)
			out << "[style=filled]";
		out << ";" << endl;
	}

	// Drawing the edges
	for(int i = 0; i < sol.get_instance().get_n(); i++)
		for(int j = 0; j < sol.get_instance().get_n(); j++){
			if(i == j) continue;
			char aux[10];
			sprintf(aux, "%.2lf", z[i][j]);
			if(z[i][j] == 0.5)
				out << i << "--" << j << "[style=dotted];" << endl;
			if((z[i][j] != 0.0) && (z[i][j] < 0.5))
				out << i << "--" << j << "[color=red, label=" << aux << "];" << endl;
			if((z[i][j] > 0.5) && (z[i][j] != 1.0))
				out << i << "--" << j << "[color=blue, label=" << aux << "];" << endl;
			if(z[i][j] > 0.5)
				out << i << "--" << j << ";" << endl;
		}

	out << "}";
}
