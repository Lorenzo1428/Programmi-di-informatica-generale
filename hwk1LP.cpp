#include <iostream>
#include <vector>
#include "graphs.h"
#include <algorithm>

namespace homework {

    graphs::Graph reverse_graph(graphs::Graph& graph) {
        graphs::Graph graph_r(graph.getNumVertices());
        for(int i=0; i<graph.getNumVertices(); i++){
            for(int j= 0; j<graph.getNeighbors(i).size(); j++){
                int v = graph.getNeighbors(i).at(j);
                graph_r.addEdge(v,i);
            }
        }    
        return graph_r;
    }

    void dfs(graphs::Graph& graph, unsigned int v, std::vector<int>& visit_order, std::vector<bool>& visited) {
                    if (!visited.at(v)) {
                        visited.at(v) = true;
                        for (int x: graph.getNeighbors(v))
                            if (!visited.at(x))
                                dfs(graph, x, visit_order, visited);
                        visit_order.push_back(v);
                    }
    }

    std::vector<std::vector<int>> stronglyConnectedComponents(graphs::Graph& graph) {
        std::vector<std::vector<int>> scc;
        std::vector<int> post_order;
        std::vector<int> cc;
        std::vector<bool> visit (graph.getNumVertices(), false);

        for(int i=0; i<graph.getNumVertices(); i++){
            if(visit.at(i) == false){
                dfs(graph, i, post_order, visit);
            }
        }
        

        for(int i=0; i< visit.size(); i++){
            visit.at(i) = false;
        }      
        std::reverse(post_order.begin(), post_order.end()); 
        graphs::Graph graph_r = reverse_graph(graph); 

        for(int i=0; i<post_order.size(); i++){
            if(visit.at(post_order.at(i)) == false)
            {
                dfs(graph_r, post_order.at(i), cc, visit);
                scc.push_back(cc);
                cc.clear();
            }            
        }
        return scc;
    }
}
