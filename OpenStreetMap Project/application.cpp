#include "application.h"

#include <cassert>
#include <cstdlib>
#include <cstring>
#include <iomanip> /*setprecision*/
#include <iostream>
#include <map>
#include <queue>
#include <set>
#include <stack>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "dist.h"
#include "graph.h"
#include "osm.h"
#include "tinyxml2.h"

using namespace std;
using namespace tinyxml2;

double INF = numeric_limits<double>::max();

graph<long long, double> buildGraph(
    const map<long long, Coordinates>& Nodes,
    const vector<FootwayInfo>& Footways,
    const vector<BuildingInfo>& Buildings) {
    graph<long long, double> G;

    // TODO_STUDENT
    for (const auto& node : Nodes) {
    G.addVertex(node.first); // adding each node to the graph
}

int numSidewalkEdges = 1; //counter for sidewalk edges
int numBedgeCount = 0; //counter for bulding edges

// Add sidewalk edges to the graph
for (const auto& sidewalk : Footways) { //this function loops through each sidewalk segment
    for (size_t i = 0; i < sidewalk.Nodes.size() - 1; ++i) { // here we loop through each pair of consecutive nodes in the sidewalk segment
        long long startNode = sidewalk.Nodes[i];
        long long endNode = sidewalk.Nodes[i + 1];
        
        // here we calculate the length of the sidewalk segment using the distance between start and end nodes
        double length = distBetween2Points(
            Nodes.at(startNode).Lat, Nodes.at(startNode).Lon,
            Nodes.at(endNode).Lat, Nodes.at(endNode).Lon);
        
        G.addEdge(startNode, endNode, length);// adding an edge between the start and end nodes of the sidewalk segment
        G.addEdge(endNode, startNode, length); 
    
        numSidewalkEdges += 2; //incrementing the count of sidewalk edges by 2
    }
}


for (const auto& building : Buildings) {// here we iterate through each building in the Buildings vector
    G.addVertex(building.Coords.ID);// adding a vertex to the graph with the ID of the building's coordinates
}

const double maxDist = 0.041;

for (const auto& building : Buildings) {//here we iterate through each building in the Buildings vector again
    G.addVertex(building.Coords.ID);// adding a vertex to the graph with the ID of the building's coordinates

    for (const auto& node : Nodes) { //here we iterate through each node in the Nodes map

        if (node.second.OnFootway) {// here we check if the node is on a footway

            double length = distBetween2Points(
                building.Coords.Lat, building.Coords.Lon,
                node.second.Lat, node.second.Lon);

            if (length <= maxDist) {  // here ff the distance is less than or equal to the maximum distance threshold

                //adding an edge between the building and the node
                G.addEdge(building.Coords.ID, node.first, length);
                //adding an edge between the node and the building
                G.addEdge(node.first, building.Coords.ID, length);
                numBedgeCount += 2; // incrementing the count of bidirectional edges added

            }
        }
    }
}

    return G;
} 


vector<long long> dijkstra(
    const graph<long long, double>& G,
    long long start,
    long long target,
    const set<long long>& ignoreNodes) {
    vector<long long> path;

    // TODO_STUDENT
       
    if (start == target) {
    path.push_back(start); //here if the start node is the same as the target node, it adds the start node to the path and return
    return path;
}

const auto& vertices = G.getVertices(); // here we get the vertices of the graph
const long long Numvertices = vertices.size(); // here we get the number of vertices
unordered_map<long long, size_t> vertexIndexMap; // a map to store vertex indices

// Assign indices to vertices
for (size_t i = 0; i < Numvertices; ++i) {
    vertexIndexMap[vertices[i]] = i; //here we map each vertex to its index
}
vector<double> dist(Numvertices, numeric_limits<double>::max()); //here the vector is to store shortest distance to each vertex
// Initialize previous node vector with -1
vector<long long> prev(Numvertices, -1); // here the vector will store the previous node on the shortest path to each vertex
priority_queue<pair<double, long long>, vector<pair<double, long long>>, greater<pair<double, long long>>> pq;
    dist[vertexIndexMap[start]] = 0.0;
    pq.emplace(0.0, start);

    while (!pq.empty()) {
        long long u = pq.top().second; //here it gets the vertex with the smallest tentative distance
        pq.pop();

        if (u == target) { // here if we reach the target node, break out of the loop
            break;
        }

        if (ignoreNodes.count(u) && u != start && u != target) { //here if the current node is to be ignored, skip it
            continue;
        }

        for (const auto& neighbor : G.neighbors(u)) {// this for loop will iterate over neighbors of the current node
            long long v = neighbor;
            size_t vIndex = vertexIndexMap[v];
            double weight;
            if (G.getWeight(u, v, weight)) {// here we get the weight of the edge between the current node and its neighbor
                double alt = dist[vertexIndexMap[u]] + weight; //this function will calculate the tentative distance to the neighbor through the current node
               
                if (alt < dist[vIndex]) {//here if the tentative distance is smaller than the current distance to the neighbor
                    dist[vIndex] = alt;
                   //here we update the distance to the neighbor and the previous node on the shortest path to it
                    prev[vIndex] = u;
                    pq.emplace(alt, v);
                }
            }
        }
    }

    size_t targetIndex = vertexIndexMap[target]; // reconstructing the shortest path
    if (prev[targetIndex] == -1) { // here if there is no path to the target node, it returns an empty path
        return path;
    }

    long long curr = target; // here we trace back the shortest path from the target node to the start node
    while (curr != -1) {
        path.push_back(curr);
        curr = prev[vertexIndexMap[curr]];
    }
    reverse(path.begin(), path.end());// and now this function will reverse the path to get it in the correct order
    return path;
}

double pathLength(const graph<long long, double>& G, const vector<long long>& path) {
    double length = 0.0;
    double weight;
    for (size_t i = 0; i + 1 < path.size(); i++) {
        bool res = G.getWeight(path.at(i), path.at(i + 1), weight);
        assert(res);
        length += weight;
    }
    return length;
}

void outputPath(const vector<long long>& path) {
    for (size_t i = 0; i < path.size(); i++) {
        cout << path.at(i);
        if (i != path.size() - 1) {
            cout << "->";
        }
    }
    cout << endl;
}

void application(
    const vector<BuildingInfo>& Buildings,
    const graph<long long, double>& G) {
    string person1Building, person2Building;

    set<long long> buildingNodes;
    for (const auto& building : Buildings) {
        buildingNodes.insert(building.Coords.ID);
    }

    cout << endl;
    cout << "Enter person 1's building (partial name or abbreviation), or #> ";
    getline(cin, person1Building);

    while (person1Building != "#") {
        cout << "Enter person 2's building (partial name or abbreviation)> ";
        getline(cin, person2Building);

        //
        // find the building coordinates
        //
        bool foundP1 = false;
        bool foundP2 = false;
        Coordinates P1Coords, P2Coords;
        string P1Name, P2Name;

        for (const BuildingInfo& building : Buildings) {
            if (building.Abbrev == person1Building) {
                foundP1 = true;
                P1Name = building.Fullname;
                P1Coords = building.Coords;
            }
            if (building.Abbrev == person2Building) {
                foundP2 = true;
                P2Name = building.Fullname;
                P2Coords = building.Coords;
            }
        }

        for (const BuildingInfo& building : Buildings) {
            if (!foundP1 &&
                building.Fullname.find(person1Building) != string::npos) {
                foundP1 = true;
                P1Name = building.Fullname;
                P1Coords = building.Coords;
            }
            if (!foundP2 && building.Fullname.find(person2Building) != string::npos) {
                foundP2 = true;
                P2Name = building.Fullname;
                P2Coords = building.Coords;
            }
        }

        if (!foundP1) {
            cout << "Person 1's building not found" << endl;
        } else if (!foundP2) {
            cout << "Person 2's building not found" << endl;
        } else {
            cout << endl;
            cout << "Person 1's point:" << endl;
            cout << " " << P1Name << endl;
            cout << " (" << P1Coords.Lat << ", " << P1Coords.Lon << ")" << endl;
            cout << "Person 2's point:" << endl;
            cout << " " << P2Name << endl;
            cout << " (" << P2Coords.Lat << ", " << P2Coords.Lon << ")" << endl;

            string destName;
            Coordinates destCoords;

            Coordinates centerCoords = centerBetween2Points(
                P1Coords.Lat, P1Coords.Lon, P2Coords.Lat, P2Coords.Lon);

            double minDestDist = numeric_limits<double>::max();

            for (const BuildingInfo& building : Buildings) {
                double dist = distBetween2Points(
                    centerCoords.Lat, centerCoords.Lon,
                    building.Coords.Lat, building.Coords.Lon);
                if (dist < minDestDist) {
                    minDestDist = dist;
                    destCoords = building.Coords;
                    destName = building.Fullname;
                }
            }

            cout << "Destination Building:" << endl;
            cout << " " << destName << endl;
            cout << " (" << destCoords.Lat << ", " << destCoords.Lon << ")" << endl;

            vector<long long> P1Path = dijkstra(G, P1Coords.ID, destCoords.ID, buildingNodes);
            vector<long long> P2Path = dijkstra(G, P2Coords.ID, destCoords.ID, buildingNodes);

            // This should NEVER happen with how the graph is built
            if (P1Path.empty() || P2Path.empty()) {
                cout << endl;
                cout << "At least one person was unable to reach the destination building. Is an edge missing?" << endl;
                cout << endl;
            } else {
                cout << endl;
                cout << "Person 1's distance to dest: " << pathLength(G, P1Path);
                cout << " miles" << endl;
                cout << "Path: ";
                outputPath(P1Path);
                cout << endl;
                cout << "Person 2's distance to dest: " << pathLength(G, P2Path);
                cout << " miles" << endl;
                cout << "Path: ";
                outputPath(P2Path);
            }
        }

        //
        // another navigation?
        //
        cout << endl;
        cout << "Enter person 1's building (partial name or abbreviation), or #> ";
        getline(cin, person1Building);
    }
}
