#include "CampusGraph.h"
#include <iostream>

CampusGraph::CampusGraph() {}

int CampusGraph::findLocationIndex(string id) {
    for (int i = 0; i < locations.size(); i++) {
        if (locations[i].id == id) {
            return i;
        }
    }
    return -1;
}

void CampusGraph::addLocation(string id, string name) {
    Location loc(id, name);
    locations.push_back(loc);
}

void CampusGraph::addPath(string from, string to, int distance) {
    Edge e1(from, to, distance);
    Edge e2(to, from, distance);
    edges.push_back(e1);
    edges.push_back(e2);
}

vector<Location> CampusGraph::getAllLocations() {
    return locations;
}

vector<Edge> CampusGraph::getNeighbors(string locationId) {
    vector<Edge> neighbors;
    for (int i = 0; i < edges.size(); i++) {
        if (edges[i].from == locationId) {
            neighbors.push_back(edges[i]);
        }
    }
    return neighbors;
}

int CampusGraph::getLocationCount() {
    return locations.size();
}

int CampusGraph::getDegree(string locationId) {
    int degree = 0;
    for (int i = 0; i < edges.size(); i++) {
        if (edges[i].from == locationId) {
            degree++;
        }
    }
    return degree;
}

int CampusGraph::getShortestDistance(string from, string to) {
    if (from == to) return 0;

    int n = locations.size();
    int dist[100];
    bool visited[100];

    for (int i = 0; i < n; i++) {
        dist[i] = 999999;
        visited[i] = false;
    }

    int startIdx = findLocationIndex(from);
    if (startIdx == -1) return -1;

    dist[startIdx] = 0;

    for (int count = 0; count < n; count++) {
        int minDist = 999999;
        int minIdx = -1;

        for (int i = 0; i < n; i++) {
            if (!visited[i] && dist[i] < minDist) {
                minDist = dist[i];
                minIdx = i;
            }
        }

        if (minIdx == -1) break;
        visited[minIdx] = true;

        vector<Edge> neighbors = getNeighbors(locations[minIdx].id);
        for (int i = 0; i < neighbors.size(); i++) {
            int neighborIdx = findLocationIndex(neighbors[i].to);
            if (neighborIdx != -1 && !visited[neighborIdx]) {
                int newDist = dist[minIdx] + neighbors[i].distance;
                if (newDist < dist[neighborIdx]) {
                    dist[neighborIdx] = newDist;
                }
            }
        }
    }

    int endIdx = findLocationIndex(to);
    return (endIdx == -1 || dist[endIdx] == 999999) ? -1 : dist[endIdx];
}

vector<string> CampusGraph::getShortestPath(string from, string to) {
    vector<string> path;
    if (from == to) {
        path.push_back(from);
        return path;
    }

    int n = locations.size();
    int dist[100];
    bool visited[100];
    string parent[100];

    for (int i = 0; i < n; i++) {
        dist[i] = 999999;
        visited[i] = false;
        parent[i] = "";
    }

    int startIdx = findLocationIndex(from);
    if (startIdx == -1) return path;

    dist[startIdx] = 0;

    for (int count = 0; count < n; count++) {
        int minDist = 999999;
        int minIdx = -1;

        for (int i = 0; i < n; i++) {
            if (!visited[i] && dist[i] < minDist) {
                minDist = dist[i];
                minIdx = i;
            }
        }

        if (minIdx == -1) break;
        visited[minIdx] = true;

        vector<Edge> neighbors = getNeighbors(locations[minIdx].id);
        for (int i = 0; i < neighbors.size(); i++) {
            int neighborIdx = findLocationIndex(neighbors[i].to);
            if (neighborIdx != -1 && !visited[neighborIdx]) {
                int newDist = dist[minIdx] + neighbors[i].distance;
                if (newDist < dist[neighborIdx]) {
                    dist[neighborIdx] = newDist;
                    parent[neighborIdx] = locations[minIdx].id;
                }
            }
        }
    }

    int endIdx = findLocationIndex(to);
    if (endIdx == -1 || dist[endIdx] == 999999) return path;

    string current = to;
    while (current != "") {
        path.insert(path.begin(), current);
        int idx = findLocationIndex(current);
        current = parent[idx];
    }

    return path;
}

void CampusGraph::sortEdgesByWeight(vector<Edge>& edgeList) {
    for (int i = 0; i < edgeList.size() - 1; i++) {
        for (int j = 0; j < edgeList.size() - i - 1; j++) {
            if (edgeList[j].distance > edgeList[j + 1].distance) {
                Edge temp = edgeList[j];
                edgeList[j] = edgeList[j + 1];
                edgeList[j + 1] = temp;
            }
        }
    }
}

vector<Edge> CampusGraph::kruskalMST() {
    vector<Edge> mst;
    vector<Edge> allEdges;

    for (int i = 0; i < edges.size(); i++) {
        bool found = false;
        for (int j = 0; j < allEdges.size(); j++) {
            if ((allEdges[j].from == edges[i].from && allEdges[j].to == edges[i].to) ||
                (allEdges[j].from == edges[i].to && allEdges[j].to == edges[i].from)) {
                found = true;
                break;
            }
        }
        if (!found) {
            allEdges.push_back(edges[i]);
        }
    }

    sortEdgesByWeight(allEdges);

    string parent[100];
    for (int i = 0; i < locations.size(); i++) {
        parent[i] = locations[i].id;
    }

    auto find = [&](string x) -> string {
        int idx = findLocationIndex(x);
        if (parent[idx] == x) return x;
        string root = x;
        while (parent[findLocationIndex(root)] != root) {
            root = parent[findLocationIndex(root)];
        }
        return root;
        };

    for (int i = 0; i < allEdges.size(); i++) {
        string root1 = find(allEdges[i].from);
        string root2 = find(allEdges[i].to);

        if (root1 != root2) {
            mst.push_back(allEdges[i]);
            parent[findLocationIndex(root2)] = root1;

            if (mst.size() == locations.size() - 1) break;
        }
    }

    return mst;
}

vector<string> CampusGraph::BFS(string start) {
    vector<string> result;
    vector<bool> visited(locations.size(), false);
    vector<string> queue;

    int startIdx = findLocationIndex(start);
    if (startIdx == -1) return result;

    visited[startIdx] = true;
    queue.push_back(start);

    while (!queue.empty()) {
        string current = queue[0];
        queue.erase(queue.begin());
        result.push_back(current);

        vector<Edge> neighbors = getNeighbors(current);
        for (int i = 0; i < neighbors.size(); i++) {
            int neighborIdx = findLocationIndex(neighbors[i].to);
            if (neighborIdx != -1 && !visited[neighborIdx]) {
                visited[neighborIdx] = true;
                queue.push_back(neighbors[i].to);
            }
        }
    }

    return result;
}

void CampusGraph::DFSHelper(string current, vector<bool>& visited, vector<string>& result) {
    int idx = findLocationIndex(current);
    visited[idx] = true;
    result.push_back(current);

    vector<Edge> neighbors = getNeighbors(current);
    for (int i = 0; i < neighbors.size(); i++) {
        int neighborIdx = findLocationIndex(neighbors[i].to);
        if (neighborIdx != -1 && !visited[neighborIdx]) {
            DFSHelper(neighbors[i].to, visited, result);
        }
    }
}

vector<string> CampusGraph::DFS(string start) {
    vector<string> result;
    vector<bool> visited(locations.size(), false);

    int startIdx = findLocationIndex(start);
    if (startIdx == -1) return result;

    DFSHelper(start, visited, result);
    return result;
}

int CampusGraph::getOddDegreeCount() {
    int count = 0;
    for (int i = 0; i < locations.size(); i++) {
        if (getDegree(locations[i].id) % 2 == 1) {
            count++;
        }
    }
    return count;
}

bool CampusGraph::hasEulerianPath() {
    int oddCount = getOddDegreeCount();
    return (oddCount == 0 || oddCount == 2);
}

bool CampusGraph::hamiltonianHelper(int pos, vector<bool>& visited, vector<string>& path) {
    if (pos == locations.size()) {
        return true;
    }

    string current = path[pos - 1];
    vector<Edge> neighbors = getNeighbors(current);

    for (int i = 0; i < neighbors.size(); i++) {
        int neighborIdx = findLocationIndex(neighbors[i].to);
        if (neighborIdx != -1 && !visited[neighborIdx]) {
            visited[neighborIdx] = true;
            path.push_back(neighbors[i].to);

            if (hamiltonianHelper(pos + 1, visited, path)) {
                return true;
            }

            visited[neighborIdx] = false;
            path.pop_back();
        }
    }

    return false;
}

bool CampusGraph::hasHamiltonianPath() {
    for (int i = 0; i < locations.size(); i++) {
        vector<bool> visited(locations.size(), false);
        vector<string> path;

        visited[i] = true;
        path.push_back(locations[i].id);

        if (hamiltonianHelper(1, visited, path)) {
            return true;
        }
    }
    return false;
}

void CampusGraph::displayGraph() {
    cout << "\n=== Campus Graph ===" << endl;
    cout << "Locations: " << locations.size() << endl;
    cout << "Paths: " << edges.size() / 2 << endl;

    for (int i = 0; i < locations.size(); i++) {
        cout << "\n" << locations[i].id << " - " << locations[i].name << endl;
        cout << "  Degree: " << getDegree(locations[i].id) << endl;
        cout << "  Connected to: ";
        vector<Edge> neighbors = getNeighbors(locations[i].id);
        for (int j = 0; j < neighbors.size(); j++) {
            cout << neighbors[j].to << "(" << neighbors[j].distance << "m) ";
        }
        cout << endl;
    }
}

void CampusGraph::displayMST(vector<Edge> mst) {
    int totalWeight = 0;
    cout << "\n=== Minimum Spanning Tree ===" << endl;
    for (int i = 0; i < mst.size(); i++) {
        cout << i + 1 << ". " << mst[i].from << " -- " << mst[i].to
            << " (" << mst[i].distance << "m)" << endl;
        totalWeight += mst[i].distance;
    }
    cout << "Total: " << totalWeight << "m" << endl;
}

void CampusGraph::loadDefaultCampus() {
    addLocation("CS", "Computer Science Building");
    addLocation("LIB", "Library");
    addLocation("LAB", "Research Labs");
    addLocation("ADMIN", "Admin Building");
    addLocation("CAF", "Cafeteria");
    addLocation("GATE", "Main Gate");

    addPath("CS", "LIB", 100);
    addPath("CS", "LAB", 80);
    addPath("CS", "CAF", 150);
    addPath("LIB", "ADMIN", 120);
    addPath("LAB", "CAF", 90);
    addPath("ADMIN", "CAF", 100);
    addPath("GATE", "CS", 200);
    addPath("GATE", "ADMIN", 150);
}