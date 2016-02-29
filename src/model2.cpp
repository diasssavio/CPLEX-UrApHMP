/*
 * model.cpp
 *
 *  Created on: Aug 21, 2015
 *      Author: savio
 */

#include "../include/model2.h"

model2::model2(IloEnv _env, uraphmp& _instance, solution& _sol) : IloModel(_env), instance(_instance), sol(_sol) {
	init();
	add_obj();
	add_const();
}

model2::~model2() { }

void model2::init(){
	int n = instance.get_n();
	w = IloNumVarArray2(getEnv(), n);
	x = IloNumVarArray3(getEnv(), n);
	y = IloNumVarArray3(getEnv(), n);
	z = IloNumVarArray2(getEnv(), n);

	for(IloInt i = 0; i < n; i++){
		x[i] = IloNumVarArray2(getEnv(), n);
		y[i] = IloNumVarArray2(getEnv(), n);
		for(IloInt k = 0; k < n; k++){
			x[i][k] = IloNumVarArray(getEnv(), n);
			y[i][k] = IloNumVarArray(getEnv(), n);
			for(IloInt j = 0; j < n; j++){
				x[i][k][j] = IloNumVar(getEnv(), 0, +IloInfinity, ILOFLOAT);
				stringstream x_name;
				x_name << "x(" << i << ")(" << k << ")(" << j << ")";
				x[i][k][j].setName(x_name.str().c_str());
				add(x[i][k][j]);

				y[i][k][j] = IloNumVar(getEnv(), 0, +IloInfinity, ILOFLOAT);
				stringstream y_name;
				y_name << "y(" << i << ")(" << k << ")(" << j << ")";
				y[i][k][j].setName(y_name.str().c_str());
				add(y[i][k][j]);
			}
		}

		w[i] = IloNumVarArray(getEnv(), n);
		z[i] = IloNumVarArray(getEnv(), n);
		for(IloInt k = 0; k < n; k++){
			z[i][k] = IloNumVar(getEnv(), 0, 1, ILOINT);
			// z[i][k] = IloNumVar(getEnv(), 0, 1, ILOFLOAT);
			stringstream z_name;
			z_name << "z(" << i << ")(" << k << ")";
			z[i][k].setName(z_name.str().c_str());
			add(z[i][k]);

			w[i][k] = IloNumVar(getEnv(), 0, +IloInfinity, ILOFLOAT);
			stringstream w_name;
			w_name << "w(" << i << ")(" << k << ")";
			w[i][k].setName(w_name.str().c_str());
			add(w[i][k]);
		}
	}
}

void model2::add_const(){
	int n = instance.get_n();
	int p = sol.get_p();
	int r = sol.get_r();
	vector< vector< double > > traffics = instance.get_traffics();

	// Constraints (2): attribution to at most r hubs
	for(IloInt i = 0; i < n; i++){
		IloExpr expr(getEnv());
		for(IloInt k = 0; k < n; k++)
			expr += z[i][k];
		IloConstraint c2 = (expr <= r);
		stringstream c2_name;
		c2_name << "Cons(2)[" << i << "]";
		c2.setName(c2_name.str().c_str());
		add(c2);
	}

	// Constraints (3): attribution only to hubs
	for(IloInt i = 0; i < n; i++){
		for(IloInt k = 0; k < n; k++){
			IloConstraint c3 = (z[i][k] <= z[k][k]);
			stringstream c3_name;
			c3_name << "Cons(3)[" << i << "]" << "[" << k << "]";
			c3.setName(c3_name.str().c_str());
			add(c3);
		}
	}

	// Constraint (4): allocation of p hubs
	IloExpr expr(getEnv());
	for(IloInt k = 0; k < n; k++)
		expr += z[k][k];
	IloConstraint c4 = (expr == p);
	stringstream c4_name;
	c4_name << "Cons(4)";
	c4.setName(c4_name.str().c_str());
	add(c4);

	// Flow constraints
	// Constraints (5): Traffics from i must travel
	for(IloInt i = 0; i < n; i++){
		IloExpr expr1(getEnv());
		for(IloInt k = 0; k < n; k++)
			expr1 += w[i][k];

		IloNum sum = 0.0;
		for(IloInt j = 0; j < n; j++)
			sum += traffics[i][j];

		IloConstraint c5 = (expr1 == sum);
		stringstream c5_name;
		c5_name << "Cons(5)(" << i << ")";
		c5.setName(c5_name.str().c_str());
		add(c5);
	}

	// Constraints (6): 
	for(IloInt i = 0; i < n; i++)
		for(IloInt k = 0; k < n; k++){
			IloExpr expr1(getEnv());
			for(IloInt l = 0; l < n; l++){
				if(l == k) continue;
				expr1 += y[i][k][l] - y[i][l][k];
			}

			IloExpr expr2(getEnv());
			expr2 = w[i][k];
			for(IloInt j = 0; j < n; j++)
				expr2 -= x[i][k][j];

			IloConstraint c6 = (expr1 == expr2);
			stringstream c6_name;
			c6_name << "Cons(6)(" << i << ")(" << k << ")";
			c6.setName(c6_name.str().c_str());
			add(c6);
		}

	// Constraints (7):
	for(IloInt i = 0; i < n; i++)
		for(IloInt j = 0; j < n; j++){
			IloExpr expr(getEnv());
			for(IloInt l = 0; l < n; l++)
				expr += x[i][l][j];
			IloConstraint c7 = (expr == traffics[i][j]);
			stringstream c7_name;
			c7_name << "Cons(7)(" << i << ")(" << j << ")";
			c7.setName(c7_name.str().c_str());
			add(c7);
		}

	// Constraints (8):
	for(IloInt i = 0; i < n; i++)
		for(IloInt k = 0; k < n; k++){
			IloExpr expr(getEnv());
			for(IloInt j = 0; j < n; j++)
				expr += traffics[i][j] * z[i][k];
			IloConstraint c8 = (w[i][k] <= expr);
			stringstream c8_name;
			c8_name << "Cons(8)(" << i << ")(" << k << ")";
			c8.setName(c8_name.str().c_str());
			add(c8);
		}

	// Constraints (9):
	for(IloInt i = 0; i < n; i++)
		for(IloInt j = 0; j < n; j++)
			for(IloInt l = 0; l < n; l++){
				IloConstraint c9 = (x[i][l][j] <= traffics[i][j] * z[j][l]);
				stringstream c9_name;
				c9_name << "Cons(9)(" << i << ")(" << j << ")(" << l << ")";
				c9.setName(c9_name.str().c_str());
				add(c9);
			}

	// Defining values of z[k][k] and w[i][k]
	/*vector< bool > alloc_hubs = sol.get_bin_alloc_hubs();
	for(IloInt k = 0; k < n; k++){
		IloConstraint c8;
		if(alloc_hubs[k])
			c8 = (z[k][k] == 1.0);
		else{
			c8 = (z[k][k] == 0.0);
		}
		stringstream c8_name;
		c8_name << "Cons(8)(" << k << ")";
		c8.setName(c8_name.str().c_str());
		add(c8);
	}
	for(IloInt k = 0; k < n; k++)
		if(!alloc_hubs[k])
			for(IloInt i = 0; i < n; i++){
				IloConstraint c8 = (z[i][k] == 0);
				stringstream c8_name;
				c8_name << "Cons_8(" << i << ")(" << k << ")";
				c8.setName(c8_name.str().c_str());
				add(c8);

				IloConstraint c9 = (w[i][k] == 0);
				stringstream c9_name;
				c9_name << "Cons_9(" << i << ")(" << k << ")";
				c9.setName(c9_name.str().c_str());
				add(c9);
			}

	// Defining values of x[i][k][j] & y[i][k][l] where z[k][k] == 0.0 or z[l][l] == 0.0
	for(IloInt i = 0; i < n; i++)
		for(IloInt j = 0; j < n; j++)
			for(IloInt k = 0; k < n; k++)
				if(!alloc_hubs[k]){
					IloConstraint c10 = (x[i][k][j] == 0);
					stringstream c10_name;
					c10_name << "Cons_10(" << i << ")(" << k << ")(" << j << ")";
					c10.setName(c10_name.str().c_str());
					add(c10);
				}
	for(IloInt i = 0; i < n; i++)
		for(IloInt k = 0; k < n; k++)
			for(IloInt l = 0; l < n; l++)
				if(!alloc_hubs[k] || !alloc_hubs[l]){
					IloConstraint c10 = (y[i][k][l] == 0);
					stringstream c10_name;
					c10_name << "Cons_10(" << i << ")(" << k << ")(" << l << ")";
					c10.setName(c10_name.str().c_str());
					add(c10);
				}*/
}

void model2::add_obj(){
	int n = instance.get_n();
	vector< vector< double > > traffics = instance.get_traffics();
	vector< vector< double > > distances = instance.get_distances();
	double collection_rate = instance.get_collection_rate();
	double transfer_rate = instance.get_transfer_rate();
	double distribution_rate = instance.get_distribution_rate();

	// Creating OF expression
	IloExpr expr(getEnv());
	for(IloInt i = 0; i < n; i++){
		for(IloInt k = 0; k < n; k++)
			expr += collection_rate * distances[i][k] * w[i][k];
		for(IloInt k = 0; k < n; k++)
			for(IloInt l = 0; l < n; l++){
				if(l == k) continue;
				expr += transfer_rate * distances[k][l] * y[i][k][l];
			}
		for(IloInt j = 0; j < n; j++)
			for(IloInt l = 0; l < n; l++)
				expr += distribution_rate * distances[l][j] * x[i][l][j];
	}

	// for(IloInt i = 0; i < n; i++)
	// 	for(IloInt j = 0; j < n; j++)
	// 		for(IloInt k = 0; k < n; k++)
	// 			for(IloInt l = 0; l < n; l++)
	// 				expr += traffics[i][j] * ((collection_rate * distances[i][k]) + (transfer_rate * distances[k][l]) + (distribution_rate * distances[l][j])) * f[i][j][k][l];

	// Adding objective function
	add(IloMinimize(getEnv(), expr));
}

