#include "draw_graph.h"

string _to_string(int t){
  stringstream ss;
  ss << t;
  return ss.str();
}

drawer::drawer(double** z, solution _sol){
  sol = _sol;
  int n = sol.get_instance().get_n();

  for(IloInt i = 0; i < n; i++){
    for(IloInt j = 0; j < n; j++)
      if((z[i][j] != 0.0) && (i != j)){
        edges.push_back(Edge(i, j));
        weights.push_back(100 * z[i][j]);
        // cout << z[i][j] << endl;
      }
    names.push_back(_to_string(i + 1));
  }
//  for(IloInt i = 0; i < n; i++)
//	  for(IloInt j = i+1; j < n; j++)
//		  if(z[i][i] != 0.0 && z[j][j] != 0.0){
//			  edges.push_back(Edge(i, j));
//			  weights.push_back(100 * sol.get_instance().get_transfer_rate());
//		  }

  Edge* _edges = (Edge*) malloc(sizeof(Edge) * edges.size());
  for(unsigned i = 0; i < edges.size(); i++)
    _edges[i] = edges[i];
  int* _weights = (int*) malloc(sizeof(int) * weights.size());
  for(unsigned i = 0; i < weights.size(); i++)
    _weights[i] = weights[i];

  this->g = Graph(_edges, _edges + edges.size(), _weights, n);
}

void drawer::draw(ostream& out){
  write_graphviz(out, g, make_label_writer(&names[0]));
}
