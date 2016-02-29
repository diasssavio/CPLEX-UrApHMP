/*
 * solution.cpp
 *
 *  Created on: Apr 20, 2015
 *      Author: savio
 */

#include "../include/solution.h"

bool solution::my_sol_comparison( solution s1, solution s2 ){
	return (s1.get_total_cost() < s2.get_total_cost());
}

solution::solution(){ }

/*solution::solution( solution sol ){
	this->set_instance(sol.get_instance());
	this->set_p(sol.get_p());
	this->set_r(sol.get_r());
	this->set_alloc_hubs(sol.get_alloc_hubs());
	this->set_assigned_hubs(sol.get_assigned_hubs());
	this->set_f_chosen(sol.get_f_chosen());
	this->set_s_chosen(sol.get_s_chosen());
	this->set_cost(sol.get_cost());
	this->set_hubs_cost(sol.get_hubs_cost());
}*/

solution::solution( uraphmp& instance, int p_cons, int r_cons ) : p(p_cons), r(r_cons) {
	this->set_instance(instance);
}

solution::solution( uraphmp& instance, int p_cons, int r_cons, IloNumArray2& _z, IloNumArray4& _f, double cost ) : p(p_cons), r(r_cons) {
	this->set_instance(instance);
	
	int n = this->instance.get_n();

	set< unsigned > _alloc_hubs;
	vector< vector< unsigned > > _assigned_hubs;
	vector< vector< unsigned > > _f_chosen;
	vector< vector< unsigned > > _s_chosen;

	// Translating _z to alloc_hubs and assigned_hubs
	for(unsigned i = 0; i < n; i++){
		vector< unsigned > aux;
		for(unsigned j = 0; j < n; j++){
			if(_z[i][j] == 0.0) continue;
			if((i == j))
				_alloc_hubs.insert(i);
			else
				aux.push_back(j);
		}
		_assigned_hubs.push_back(aux);
	}

	// Translating _f to _f_chosen and _s_chosen
	for(unsigned i = 0; i < n; i++){
		vector< unsigned > aux1;
		vector< unsigned > aux2;
		for(unsigned j = 0; j < n; j++)
			for(unsigned k = 0; k < n; k++)
				for(unsigned l = 0; l < n; l++)
					if(_f[i][j][k][l] != 0.0){
						aux1.push_back(k);
						aux2.push_back(l);
					}
		_f_chosen.push_back(aux1);
		_s_chosen.push_back(aux2);
	}

	// Setting the instance variables
	set_alloc_hubs(_alloc_hubs);
	set_assigned_hubs(_assigned_hubs);
	set_f_chosen(_f_chosen);
	set_s_chosen(_s_chosen);
	this->_cost = cost;
}

solution::solution( uraphmp& instance, int p_cons, int r_cons, IloNumArray2& _z, IloNumArray2& _w, IloNumArray3& _x, IloNumArray3& _y, double cost ) : p(p_cons), r(r_cons), _cost(cost) {
	this->set_instance(instance);
	
	int n = this->instance.get_n();

	set< unsigned > _alloc_hubs;
	vector< vector< unsigned > > _assigned_hubs;
	vector< vector< unsigned > > _f_chosen;
	vector< vector< unsigned > > _s_chosen;

	// Translating _z to alloc_hubs and assigned_hubs
	for(unsigned i = 0; i < n; i++){
		vector< unsigned > aux;
		for(unsigned j = 0; j < n; j++){
			if(_z[i][j] == 0.0) continue;
			if((i == j))
				_alloc_hubs.insert(i);
			else
				aux.push_back(j);
		}
		_assigned_hubs.push_back(aux);
	}

	// Translating _x _y to _f_chosen and _s_chosen
	for(unsigned i = 0; i < n; i++){
		vector< unsigned > aux1;
		vector< unsigned > aux2;
		for(unsigned j = 0; j < n; j++)
			for(unsigned k = 0; k < n; k++)
				if(_w[i][k] > 0.0)
					for(unsigned l = 0; l < n; l++){
						if(l == k) continue;
						if((_y[i][k][l] > 0.0) && (_x[i][l][j] > 0.0)){
							aux1.push_back(k);
							aux2.push_back(l);
						}
					}
		_f_chosen.push_back(aux1);
		_s_chosen.push_back(aux2);
	}

	// Setting the instance variables
	set_alloc_hubs(_alloc_hubs);
	set_assigned_hubs(_assigned_hubs);
	set_f_chosen(_f_chosen);
	set_s_chosen(_s_chosen);
}

solution::~solution() { }

void solution::set_instance( uraphmp& instance ){ this->instance = instance; }

void solution::set_p( int p ){ this->p = p; }

void solution::set_r( int r ){ this->r = r; }

void solution::set_alloc_hubs( const set< unsigned >& alloc_hubs ){ this->alloc_hubs = alloc_hubs; }

void solution::set_assigned_hubs( vector< vector< unsigned > >& assigned_hubs ){ this->assigned_hubs = assigned_hubs; }

void solution::set_assigned_hub( unsigned i, unsigned j, unsigned assigned_hub ){ this->assigned_hubs[i][j] = assigned_hub; }

void solution::set_f_chosen( vector< vector< unsigned > >& f_chosen ){ this->f_chosen = f_chosen; }

void solution::set_s_chosen( vector< vector< unsigned > >& s_chosen ){ this->s_chosen = s_chosen; }

void solution::set_cost( vector< vector< double > >& cost ){
	this->cost = cost;
	double _cost = 0.0;
	for(int i = 0; i < instance.get_n(); i++)
		for(int j = 0; j < instance.get_n(); j++)
			_cost += this->cost[i][j];
	this->_cost = _cost;
}

void solution::set_hubs_cost( vector< double >& hubs ){ this->hubs_cost = hubs; }

uraphmp& solution::get_instance(){ return this->instance; }

int solution::get_p(){ return this->p; }

int solution::get_r(){ return this->r; }

const set< unsigned >& solution::get_alloc_hubs() const{ return this->alloc_hubs; }

vector< vector< unsigned > >& solution::get_assigned_hubs(){ return this->assigned_hubs; }

vector< unsigned >& solution::get_assigned_hubs( int i ){ return this->assigned_hubs[i]; }

vector< vector< unsigned > >& solution::get_f_chosen(){ return this->f_chosen; }

vector< vector< unsigned > >& solution::get_s_chosen(){ return this->s_chosen; }

vector< vector< double > >& solution::get_cost(){ return this->cost; }

vector< double >& solution::get_hubs_cost(){ return this->hubs_cost; }

double solution::get_total_hubs_cost(){
	double cost = 0.0;
	for(int i = 0; i < p; i++)
		cost += this->hubs_cost[i];

	return cost;
}

double solution::get_total_cost(){ return _cost; }

vector< bool > solution::get_bin_alloc_hubs(){
	vector< bool > to_return(instance.get_n(), false);
	set< unsigned >::iterator h = alloc_hubs.begin();
	for(int k = 0; k < instance.get_n(); k++)
		if(*h == k){
			to_return[k] = true;
			h++;
		}
	return to_return;
}

void solution::generate_hubs_cost(){
	vector< vector< double > > traffics = instance.get_traffics();
	vector< vector< double > > distances = instance.get_distances();
	double X = instance.get_collection_rate();
	double alpha = instance.get_transfer_rate();
	double delta = instance.get_distribution_rate();

	vector< double > costs;
	for(set< unsigned >::iterator h = alloc_hubs.begin(); h != alloc_hubs.end(); h++){
		double value1 = 0.0, value2 = 0.0;
		for(int i = 0; i < instance.get_n(); i++){
			if(is_hub(i)) continue;
			for(int j = 0; j < instance.get_n(); j++){
				if(is_hub(j)) continue;
				if(f_chosen[i][j] == *h){
					value1 += traffics[i][j] * ((X * distances[i][f_chosen[i][j]]) +
						((alpha/2) * distances[f_chosen[i][j]][s_chosen[i][j]]));
				}
				if(s_chosen[i][j] == *h){
					value2 += traffics[i][j]* (((alpha/2) * distances[s_chosen[i][j]][f_chosen[i][j]]) +
							delta * distances[s_chosen[i][j]][j]);
				}
			}
		}
		costs.push_back(value1 + value2);
	}

	this->hubs_cost = costs;
}

void solution::show_data(){
	// printf("\np: %d\tr: %d\n", this->p, this->r);
	printf("Allocated Hubs: ");
	for(set< unsigned >::iterator h = alloc_hubs.begin(); h != alloc_hubs.end(); h++)
		printf("%d\t", (*h + 1));

	/*printf("\n\nAssigned Hubs:\n");
	for(int i = 0; i < instance.get_n(); i++){
		if(is_hub(i)) continue;
		printf("H[%d]: ", i);
		for(int j = 0; j < r; j++)
			printf("%d\t", assigned_hubs[i][j]);
		printf("\n");
	}

	printf("\n\nTraffics Routes:\n");
	for(int i = 0; i < instance.get_n(); i++){
		if(is_hub(i)) continue;
		printf("i[%d]: ", i);
		for(int j = 0; j < instance.get_n(); j++){
			if(is_hub(j)) continue;
			printf("%.2lf\t", cost[i][j]);
		}
		printf("\n");
	}

	printf("\n\nH1:\n");
	for(int i = 0; i < instance.get_n(); i++){
		if(is_hub(i)) continue;
		printf("i[%d]: ", i);
		for(int j = 0; j < instance.get_n(); j++){
			if(is_hub(j)) continue;
			printf("%d\t", f_chosen[i][j]);
		}
		printf("\n");
	}
	printf("\n\nH2:\n");
	for(int i = 0; i < instance.get_n(); i++){
		if(is_hub(i)) continue;
		printf("i[%d]: ", i);
		for(int j = 0; j < instance.get_n(); j++){
			if(is_hub(j)) continue;
			printf("%d\t", s_chosen[i][j]);
		}
		printf("\n");
	}*/

	/*printf("\n\nHUBS COST:\t");
	for(int i = 0; i < p; i++)
		printf("%.2lf\t", hubs_cost[i]);*/

	printf("\nTOTAL COST: %.2lf\n", get_total_cost());
}

bool solution::is_hub( unsigned index ){ return (alloc_hubs.find(index) != alloc_hubs.end()); }

bool solution::my_comparison( pair< double, int > p1, pair< double, int > p2 ){ return (p1.first < p2.first); }

void solution::assign_hubs(){
	vector< vector< unsigned > > assigned;

	vector< vector< double > > traffics = instance.get_traffics();
	vector< vector< double > > distances = instance.get_distances();

	for(int i = 0; i < instance.get_n(); i++){
		// Calculating alloc(i,h), i.e., the assignment cost of i to every hub h
		vector< pair< double, unsigned> > alloc_value;
		set< unsigned >::iterator it;
		for(it = alloc_hubs.begin(); it != alloc_hubs.end(); it++){
			double value = accumulate(traffics[i].begin(), traffics[i].end(), 0.0);
			value *= distances[i][*it];
			
			for(int j = 0; j < instance.get_n(); j++)
				value += traffics[i][j] * distances[*it][j];
			alloc_value.push_back(make_pair(value, *it));
		}

		// Sorting the elements of alloc_value
		sort(alloc_value.begin(), alloc_value.end(), my_comparison);

		// Selecting the r hubs to be assigned to i
		vector< unsigned > i_assigned;
		for(int j = 0; j < r; j++)
			i_assigned.push_back(alloc_value[j].second);

		// Adding the hubs assigned to i (Hi)
		assigned.push_back(i_assigned);
	}

	set_assigned_hubs(assigned);
}

void solution::assign_partial_hubs( int i, int j, int new_hub ){ this->assigned_hubs[i][j] = new_hub; }

void solution::route_traffics(){
	vector< vector< double > > traffics = instance.get_traffics();
	vector< vector< double > > distances = instance.get_distances();

	vector< vector< double > > cost;
	vector< vector< unsigned > > H1, H2;
	for(int i = 0; i < instance.get_n(); i++){
		vector< double > temp_cost;
		vector< unsigned > temp_H1, temp_H2;
		for(int j = 0; j < instance.get_n(); j++){
			// Calculating all costs using all hubs assigned to i
			vector< vector< double > > c_ij;
			for(int hi = 0; hi < r; hi++){
				vector< double > aux;
				for(int hj = 0; hj < r; hj++){
					double temp = traffics[i][j] * ((instance.get_collection_rate() * distances[i][assigned_hubs[i][hi]]) +
							(instance.get_transfer_rate() * distances[assigned_hubs[i][hi]][assigned_hubs[j][hj]]) +
							(instance.get_distribution_rate() * distances[assigned_hubs[j][hj]][j]));
					aux.push_back(temp);
				}
				c_ij.push_back(aux);
			}

			// Finding the minimum distance and hubs
			vector< double >::iterator it = min_element(c_ij[0].begin(), c_ij[0].end());
			int h1 = 0;
			int h2 = it - c_ij[0].begin();
			for(int hi = 1; hi < r; hi++){
				vector< double >::iterator it2 = min_element(c_ij[hi].begin(), c_ij[hi].end());
				if(*it2 < *it){
					it = it2;
					h1 = hi;
					h2 = it - c_ij[hi].begin();
				}
			}

			temp_cost.push_back(*it);
			temp_H1.push_back(assigned_hubs[i][h1]);
			temp_H2.push_back(assigned_hubs[j][h2]);
		}

		cost.push_back(temp_cost);
		H1.push_back(temp_H1);
		H2.push_back(temp_H2);
	}

	set_cost(cost);
	set_f_chosen(H1);
	set_s_chosen(H2);
	generate_hubs_cost();
}

void solution::route_partial_traffics( int i ){
	vector< vector< double > > traffics = instance.get_traffics();
	vector< vector< double > > distances = instance.get_distances();
	double cr = instance.get_collection_rate();
	double tr = instance.get_transfer_rate();
	double dr = instance.get_distribution_rate();

	vector< double > temp_cost, _temp_cost;
	vector< unsigned > temp_H1, _temp_H1, temp_H2, _temp_H2;
	// Calculating the new hubs to route traffic of the node changed (only the line)
	for(int j = 0; j < instance.get_n(); j++){
		// Calculating all costs using all hubs assigned from i to j
		vector< vector< double > > c_ij; // r x r
		vector< vector< double > > c_ji; // r x r
		for(int hi = 0; hi < r; hi++){
			vector< double > aux, aux2;
			for(int hj = 0; hj < r; hj++){
				double temp = traffics[i][j] * ((cr * distances[i][assigned_hubs[i][hi]]) +
						(tr * distances[assigned_hubs[i][hi]][assigned_hubs[j][hj]]) +
						(dr * distances[assigned_hubs[j][hj]][j]));
				aux.push_back(temp);
				temp = traffics[j][i] * ((cr * distances[j][assigned_hubs[j][hj]]) +
						(tr * distances[assigned_hubs[j][hj]][assigned_hubs[i][hi]]) +
						(dr * distances[assigned_hubs[i][hi]][i]));
				aux2.push_back(temp);
			}
			c_ij.push_back(aux);
			c_ji.push_back(aux2);
		}

		// Finding the minimum distance and hubs
		vector< double >::iterator it = min_element(c_ij[0].begin(), c_ij[0].end());
		vector< double >::iterator _it = min_element(c_ji[0].begin(), c_ji[0].end());
		int h1 = 0, _h1 = 0;
		int h2 = it - c_ij[0].begin(), _h2 = _it - c_ji[0].begin();
		for(int hi = 1; hi < r; hi++){
			vector< double >::iterator it2 = min_element(c_ij[hi].begin(), c_ij[hi].end());
			vector< double >::iterator _it2 = min_element(c_ji[hi].begin(), c_ji[hi].end());
			if(*it2 < *it){
				it = it2;
				h1 = hi;
				h2 = it - c_ij[hi].begin();
			}
			if(*_it2 < *_it){
				_it = _it2;
				_h1 = hi;
				_h2 = _it - c_ji[hi].begin();
			}
		}

		// Values for the line i changed
		temp_cost.push_back(*it);
		temp_H1.push_back(assigned_hubs[i][h1]);
		temp_H2.push_back(assigned_hubs[j][h2]);

		// Values to the column j that must be changed
		_temp_cost.push_back(*_it);
		_temp_H1.push_back(assigned_hubs[j][_h1]);
		_temp_H2.push_back(assigned_hubs[i][_h2]);
	}

	// Adding values to the lines
	cost[i] = temp_cost;
	f_chosen[i] = temp_H1;
	s_chosen[i] = temp_H2;

	// Adding values to the columns
	for(int j = 0; j < instance.get_n(); j++){
		cost[j][i] = _temp_cost[j];
		f_chosen[j][i] = _temp_H1[j];
		s_chosen[j][i] = _temp_H2[j];
	}
}
