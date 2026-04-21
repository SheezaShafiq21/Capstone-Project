#include <iostream>
#include "FileManager.h"
#include "PolicyEngine.h"
#include "AccessLog.h"
#include "CampusGraph.h"

using namespace std;

FileManager fm;
PolicyEngine pe;
AccessLog al;
CampusGraph graph;

void pause() {
    cout << "\nPress Enter...";
    cin.ignore();
    cin.get();
}

void menu() {
    cout << "\n--- Smart Campus System ---" << endl;
    cout << "1. Test Access" << endl;
    cout << "2. View Users" << endl;
    cout << "3. View Doors" << endl;
    cout << "4. View Logs" << endl;
    cout << "5. Campus Map" << endl;
    cout << "6. Shortest Path" << endl;
    cout << "7. MST (Kruskal)" << endl;
    cout << "8. BFS/DFS" << endl;
    cout << "9. Euler Path Check" << endl;
    cout << "10. Hamilton Path Check" << endl;
    cout << "11. Statistics" << endl;
    cout << "12. Run Demo" << endl;
    cout << "0. Exit" << endl;
    cout << "Choice: ";
}

void testAccess() {
    string userId, doorId;
    int hour;

    cout << "\nUser ID: ";
    cin >> userId;
    cout << "Door ID: ";
    cin >> doorId;
    cout << "Hour (0-23): ";
    cin >> hour;

    User user = fm.findUserById(userId);
    Door door = fm.findDoorById(doorId);

    if (user.getId().empty() || door.getDoorId().empty()) {
        cout << "User or Door not found!" << endl;
        return;
    }

    cout << "\nUser: " << user.getName() << " (" << user.getRole() << ")" << endl;
    cout << "Door: " << door.getDoorLocation() << endl;
    cout << "Time: " << hour << ":00" << endl;

    AccessDecision d = pe.evaluateAccess(user, door, hour);
    al.logAccess(user, door, d.granted, d.reason, d.appliedRuleId);

    if (d.granted) {
        cout << "\nAccess GRANTED" << endl;
    }
    else {
        cout << "\nAccess DENIED" << endl;
    }
    cout << "Reason: " << d.reason << endl;
}

void viewUsers() {
    vector<User> users = fm.loadUsers();
    cout << "\nTotal Users: " << users.size() << endl;
    for (int i = 0; i < users.size(); i++) {
        cout << i + 1 << ". " << users[i].getId() << " - "
            << users[i].getName() << " (" << users[i].getRole() << ")" << endl;
    }
}

void viewDoors() {
    vector<Door> doors = fm.loadDoors();
    cout << "\nTotal Doors: " << doors.size() << endl;
    for (int i = 0; i < doors.size(); i++) {
        cout << i + 1 << ". " << doors[i].getDoorId() << " - "
            << doors[i].getDoorLocation() << " (L" << doors[i].getAccessLevel() << ")" << endl;
    }
}

void viewLogs() {
    cout << "\n1. Recent Logs" << endl;
    cout << "2. Failed Only" << endl;
    cout << "3. Success Only" << endl;
    cout << "Choice: ";
    int c;
    cin >> c;

    if (c == 1) {
        vector<LogEntry> logs = al.getRecentLogs(10);
        al.displayLogs(logs);
    }
    else if (c == 2) {
        vector<LogEntry> logs = al.getDeniedLogs();
        al.displayLogs(logs);
    }
    else if (c == 3) {
        vector<LogEntry> logs = al.getGrantedLogs();
        al.displayLogs(logs);
    }
}

void campusMap() {
    graph.displayGraph();
}

void shortestPath() {
    string from, to;
    cout << "\nFrom: ";
    cin >> from;
    cout << "To: ";
    cin >> to;

    int dist = graph.getShortestDistance(from, to);
    vector<string> path = graph.getShortestPath(from, to);

    if (dist == -1) {
        cout << "No path found!" << endl;
        return;
    }

    cout << "\nPath: ";
    for (int i = 0; i < path.size(); i++) {
        cout << path[i];
        if (i < path.size() - 1) cout << " -> ";
    }
    cout << "\nDistance: " << dist << "m" << endl;
}

void showMST() {
    cout << "\nCalculating MST using Kruskal..." << endl;
    vector<Edge> mst = graph.kruskalMST();
    graph.displayMST(mst);
}

void showTraversal() {
    string start;
    cout << "\nStart location: ";
    cin >> start;

    cout << "\n1. BFS" << endl;
    cout << "2. DFS" << endl;
    cout << "Choice: ";
    int c;
    cin >> c;

    if (c == 1) {
        vector<string> path = graph.BFS(start);
        cout << "BFS: ";
        for (int i = 0; i < path.size(); i++) {
            cout << path[i] << " ";
        }
        cout << endl;
    }
    else {
        vector<string> path = graph.DFS(start);
        cout << "DFS: ";
        for (int i = 0; i < path.size(); i++) {
            cout << path[i] << " ";
        }
        cout << endl;
    }
}

void checkEuler() {
    cout << "\n=== Euler Path Check (Guard Patrol) ===" << endl;
    bool hasPath = graph.hasEulerianPath();
    int oddCount = graph.getOddDegreeCount();

    cout << "Odd degree vertices: " << oddCount << endl;

    if (hasPath) {
        cout << "Yes, guard can patrol all corridors exactly once!" << endl;
    }
    else {
        cout << "No, guard cannot patrol all corridors without repeating" << endl;
    }
}

void checkHamilton() {
    cout << "\n=== Hamilton Path Check (Lab Inspection) ===" << endl;
    cout << "Checking... (may take time)" << endl;

    bool hasPath = graph.hasHamiltonianPath();

    if (hasPath) {
        cout << "Yes, can visit all buildings exactly once!" << endl;
    }
    else {
        cout << "No, cannot visit all buildings without repeating" << endl;
    }
}

void statistics() {
    cout << "\n=== Statistics ===" << endl;
    cout << "Total attempts: " << al.getTotalAttempts() << endl;
    cout << "Successful: " << al.getSuccessfulAttempts() << endl;
    cout << "Failed: " << al.getFailedAttempts() << endl;
    cout << "Success rate: " << al.getSuccessRate() << "%" << endl;
}

void runDemo() {
    cout << "\n=== Running Demo ===" << endl;

    User s = fm.findUserById("25F-1234");
    User t = fm.findUserById("25T-1031");
    User g = fm.findUserById("25G-6789");

    Door cls = fm.findDoorById("D001");
    Door lab = fm.findDoorById("D101");
    Door dc = fm.findDoorById("D444");

    cout << "\n1. Student -> Classroom (day)" << endl;
    AccessDecision d1 = pe.evaluateAccess(s, cls, 10);
    al.logAccess(s, cls, d1.granted, d1.reason, d1.appliedRuleId);
    cout << (d1.granted ? "GRANTED" : "DENIED") << endl;

    cout << "\n2. Student -> Lab (night)" << endl;
    AccessDecision d2 = pe.evaluateAccess(s, lab, 23);
    al.logAccess(s, lab, d2.granted, d2.reason, d2.appliedRuleId);
    cout << (d2.granted ? "GRANTED" : "DENIED") << endl;

    cout << "\n3. Teacher -> Lab (day)" << endl;
    AccessDecision d3 = pe.evaluateAccess(t, lab, 14);
    al.logAccess(t, lab, d3.granted, d3.reason, d3.appliedRuleId);
    cout << (d3.granted ? "GRANTED" : "DENIED") << endl;

    cout << "\n4. Guard -> Data Center (night)" << endl;
    AccessDecision d4 = pe.evaluateAccess(g, dc, 2);
    al.logAccess(g, dc, d4.granted, d4.reason, d4.appliedRuleId);
    cout << (d4.granted ? "GRANTED" : "DENIED") << endl;

    cout << "\nDemo completed!" << endl;
}

int main() {
    cout << "Smart Campus Access & Analytics System" << endl;
    cout << "Loading..." << endl;

    fm.loadUsers();
    fm.loadDoors();
    al.loadLogs();
    graph.loadDefaultCampus();

    cout << "System ready!" << endl;

    while (true) {
        menu();
        int choice;
        cin >> choice;
        cin.ignore();

        if (choice == 0) {
            cout << "Goodbye!" << endl;
            break;
        }

        switch (choice) {
        case 1: testAccess(); break;
        case 2: viewUsers(); break;
        case 3: viewDoors(); break;
        case 4: viewLogs(); break;
        case 5: campusMap(); break;
        case 6: shortestPath(); break;
        case 7: showMST(); break;
        case 8: showTraversal(); break;
        case 9: checkEuler(); break;
        case 10: checkHamilton(); break;
        case 11: statistics(); break;
        case 12: runDemo(); break;
        default: cout << "Invalid choice!" << endl;
        }

        pause();
    }

    return 0;
}