#pragma once

#include <string>
#include <vector>

using namespace std;

struct Location {
    string id;
    string name;

    Location() : id(""), name("") {}
    Location(string i, string n) : id(i), name(n) {}
};

struct Edge {
    string from;
    string to;
    int distance;

    Edge() : from(""), to(""), distance(0) {}
    Edge(string f, string t, int d) : from(f), to(t), distance(d) {}
};

class CampusGraph {
private:
    vector<Location> locations;
    vector<Edge> edges;

    int findLocationIndex(string id);
    void sortEdgesByWeight(vector<Edge>& edgeList);

public:
    CampusGraph();
    void addLocation(string id, string name);
    void addPath(string from, string to, int distance);
    vector<Location> getAllLocations();
    vector<Edge> getNeighbors(string locationId);
    int getLocationCount();
    int getDegree(string locationId);
    int getShortestDistance(string from, string to);
    vector<string> getShortestPath(string from, string to);
    vector<Edge> kruskalMST();
    vector<string> BFS(string start);
    vector<string> DFS(string start);
    void DFSHelper(string current, vector<bool>& visited, vector<string>& result);
    bool hasEulerianPath();
    int getOddDegreeCount();
    bool hasHamiltonianPath();
    bool hamiltonianHelper(int pos, vector<bool>& visited, vector<string>& path);
    void displayGraph();
    void displayMST(vector<Edge> mst);
    void loadDefaultCampus();
};