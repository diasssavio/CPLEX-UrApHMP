#include <cstdio>
#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <ilcplex/ilocplex.h>

#include "UrApHMP.h"
#include "solution.h"
#include "model.h"
#include "draw_graph.h"

using namespace std;
ILOSTLBEGIN
int main(){
	// Reading input file
	int n;

	scanf("%d", &n);
	double X = 1.0, alpha_1 = 0.2, delta = 1.0;
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
	int p = 5;
	int r = 3;
	solution sol(instance, p, r);

	// Initializing cplex environment
	IloEnv env;

	try{
		model mod(env, instance, sol);
		IloCplex cplex(mod);
		cplex.setParam(IloCplex::Threads, 1);
		// cplex.setParam(IloCplex::NodeLim, 0);
		cplex.setParam(IloCplex::Param::Preprocessing::Aggregator, 0);
		cplex.setParam(IloCplex::Param::Preprocessing::Presolve, 0);
		cplex.setParam(IloCplex::PreInd, IloFalse);
		// cplex.exportModel("test.lp");
		cplex.solve();

		// Getting the allocation optima from cplex solver
		double **z = (double**) malloc(sizeof(double*) * n);
		for(unsigned i = 0; i < n; i++){
			z[i] = (double*) malloc(sizeof(double) * n);
			for(unsigned j = 0; j < n; j++)
				z[i][j] = cplex.getValue(mod.z[i][j]);
		}

		// Drawing the allocation graph
		drawer to_draw(z, sol);
		ofstream dot_file("out.dot");
		to_draw.draw(dot_file);

		// Printing solution
		vector<IloNum> result;
		for(int i = 0; i < n; i++)
			result.push_back(cplex.getValue(mod.z[i][i]));
		cout << "Max Objective value = " << fixed << cplex.getObjValue() << endl << "Hubs: ";
		for(int i = 0; i < n; i++)
			if(result[i] != 0.0)
				cout << i + 1 << " ";
		cout << endl;
	}catch(IloException& e){
		cerr << "Concert Exception" << e << endl;
	}
	// Closing the environment
	env.end();

	return 0;
}
