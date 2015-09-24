#include <boost/graph/graphviz.hpp>
#include <utility>
#include <vector>
#include <string>
#include <sstream>
#include <iostream>
#include <cstring>
#include <cstdlib>

#include "typedef.hpp"
#include "solution.h"

using namespace std;
using namespace boost;

typedef adjacency_list< vecS, vecS, directedS,
    property< vertex_color_t, default_color_type >,
    property< edge_weight_t, int >
  > Graph;

typedef pair<int,int> Edge;

class drawer{
private:
  Graph g;
  vector<Edge> edges;
  vector<double> weights;
  vector<string> names;

  solution sol;

public:
  drawer(double**, solution);
  drawer(IloNumArray4, IloNumArray2, solution);
  void draw(ostream&);
};
