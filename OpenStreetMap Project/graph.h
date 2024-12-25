#pragma once

#include <iostream>
#include <map>
#include <set>
#include <unordered_map>
#include <vector>

using namespace std;

/// @brief Simple directed graph using an adjacency list.
/// @tparam VertexT vertex type
/// @tparam WeightT edge weight type
template <typename VertexT, typename WeightT>
class graph {
   private:

   //This is the graph's adjacency list representation
   //We keep the weight of the edge and a map of the vertex's neighbors for each one.
   unordered_map<VertexT, unordered_map<VertexT, WeightT>> adjList ; 
   
   // TODO_STUDENT
 
   public:
    /// Default constructor
    graph() {
        // TODO_STUDENT
    }

    /// @brief Add the vertex `v` to the graph, must run in at most O(log |V|).
    /// @param v
    /// @return true if successfully added; false if it existed already
    bool addVertex(VertexT v) {
        if (adjList.find(v) != adjList.end()){ //this loop checks if the vertex already exits in the graph or not
            return false;
        }
        
        adjList[v]; //here we add the vertex to the adjacency list
        // TODO_STUDENT
        return true;
    }

    /// @brief Add or overwrite directed edge in the graph, must run in at most O(log |V|).
    /// @param from starting vertex
    /// @param to ending vertex
    /// @param weight edge weight / label
    /// @return true if successfully added or overwritten;
    ///         false if either vertices isn't in graph
    bool addEdge(VertexT from, VertexT to, WeightT weight) {
        
        //here we check if either of the vertices doesnt exits in the graph
        if(adjList.find(from) == adjList.end()) {
            return false;
            }
        if(adjList.find(to) == adjList.end()){
            return false;
        }
        
        adjList[from][to] = weight; //here we add or overwrite the edge in the adhacency list
            // TODO_STUDENT
        return true;
    }

    /// @brief Maybe get the weight associated with a given edge, must run in at most O(log |V|).
    /// @param from starting vertex
    /// @param to ending vertex
    /// @param weight output parameter
    /// @return true if the edge exists, and `weight` is set;
    ///         false if the edge does not exist
    bool getWeight(VertexT from, VertexT to, WeightT& weight) const {

        auto vertexFrom = adjList.find(from); //this function finds the entry corresponding to the "from" vertex in the adjList
        auto vertexTo = vertexFrom -> second.find(to); //this function finds the edge "from" to "to" in the adjList

        if ((vertexFrom == adjList.end()) || (vertexTo == vertexFrom -> second.end())){ //this loop checks if the "from" vertex exists or not in the graph || and checks if the edge from "from" to "to" exists or not
            return false;
            }
        // TODO_STUDENT

        weight = vertexTo -> second; //here we get the weight associated with the edge
        return true;
    }

    /// @brief Get the out-neighbors of `v`. Must run in at most O(|V|).
    /// @param v
    /// @return vertices that v has an edge to
    set<VertexT> neighbors(VertexT v) const {
        set<VertexT> S;
        for (const auto& vertex : adjList){
            if (vertex.first == v){
                for(const auto& edge : vertex.second){
                    S.insert(edge.first);
                }
                break;
            }
        }
        // TODO_STUDENT
        return S;
    }

    /// @brief Return a vector containing all vertices in the graph
    vector<VertexT> getVertices() const {
        vector<VertexT> Vertices;
        for (const auto& pair : adjList){ //here we iterate through the vertices in the graph
            Vertices.push_back(pair.first);
        }
        // TODO_STUDENT
        return Vertices;
    }

    /// @brief Get the number of vertices in the graph. Runs in O(1).
    size_t NumVertices() const {
        // TODO_STUDENT
        return adjList.size(); //returning the number of vertices in the graph
    }

    /// @brief Get the number of directed edges in the graph. Runs in at most O(|V|).
    size_t NumEdges() const {
        // TODO_STUDENT
        size_t edgeNum = 0;

        for (const auto& pair : adjList){ //iterating through the vertices in the graph
            edgeNum += pair.second.size(); //here we add the number of edges of each vertex to the total number
        }
        return edgeNum; //and here we are returning the total number of edges
    }
};
