#include "CampusSystem.h"
#include <iostream>

CampusSystem::CampusSystem() {}

void CampusSystem::initialize() {
    cout << "\n========== Initializing Smart Campus System ==========" << endl;
    cout << "\n1. Loading users..." << endl;
    users = fileManager.loadUsers();
    cout << "\n2. Loading doors..." << endl;
    doors = fileManager.loadDoors();
    cout << "\n3. Policy engine ready." << endl;
    cout << "\n4. Access log system ready." << endl;

    cout << "\n========== System Ready! ==========" << endl;
    cout << "Users: " << users.size() << endl;
    cout << "Doors: " << doors.size() << endl;
    cout << "Rules: " << policyEngine.getRuleCount() << endl;
    cout << "Log entries: " << accessLog.getTotalAttempts() << endl;
    cout << "======================================" << endl << endl;
}
\
bool CampusSystem::processAccess(string userId, string doorId, int currentHour) {
    User user = getUserById(userId);
    if (user.getId().empty()) {
        cout << "Error: User " << userId << " not found!" << endl;
        return false;
    }

    Door door = getDoorById(doorId);
    if (door.getDoorId().empty()) {
        cout << "Error: Door " << doorId << " not found!" << endl;
        return false;
    }

    AccessDecision decision = policyEngine.evaluateAccess(user, door, currentHour);

    accessLog.logAccess(user, door, decision.granted,
        decision.reason, decision.appliedRuleId);

    return decision.granted;
}


User CampusSystem::getUserById(string userId) {
    for (int i = 0; i < users.size(); i++) {
        if (users[i].getId() == userId) {
            return users[i];
        }
    }
    return User(); 
}

vector<User> CampusSystem::getAllUsers() {
    return users;
}

void CampusSystem::displayAllUsers() {
    cout << "\n========== ALL USERS ==========" << endl;
    cout << "Total users: " << users.size() << endl << endl;

    for (int i = 0; i < users.size(); i++) {
        cout << i + 1 << ". " << users[i].getId() << " - "
            << users[i].getName() << " (" << users[i].getRole() << ")" << endl;
    }
    cout << "===============================" << endl;
}

Door CampusSystem::getDoorById(string doorId) {
    for (int i = 0; i < doors.size(); i++) {
        if (doors[i].getDoorId() == doorId) {
            return doors[i];
        }
    }
    return Door();  
}

vector<Door> CampusSystem::getAllDoors() {
    return doors;
}

void CampusSystem::displayAllDoors() {
    cout << "\n========== ALL DOORS ==========" << endl;
    cout << "Total doors: " << doors.size() << endl << endl;

    for (int i = 0; i < doors.size(); i++) {
        cout << i + 1 << ". " << doors[i].getDoorId() << " - "
            << doors[i].getDoorLocation()
            << " (Level " << doors[i].getAccessLevel() << ", "
            << doors[i].getDoorType() << ")" << endl;
    }
    cout << "===============================" << endl;
}


void CampusSystem::displayRecentLogs(int count) {
    cout << "\n========== RECENT ACCESS LOGS ==========" << endl;
    vector<LogEntry> recent = accessLog.getRecentLogs(count);
    accessLog.displayLogs(recent);
}

void CampusSystem::displayUserHistory(string userId) {
    User user = getUserById(userId);
    if (user.getId().empty()) {
        cout << "User not found!" << endl;
        return;
    }

    cout << "\n========== ACCESS HISTORY FOR " << user.getName() << " ==========" << endl;
    vector<LogEntry> userLogs = accessLog.getLogsByUser(userId);
    accessLog.displayLogs(userLogs);
}

void CampusSystem::displayDoorHistory(string doorId) {
    Door door = getDoorById(doorId);
    if (door.getDoorId().empty()) {
        cout << "Door not found!" << endl;
        return;
    }

    cout << "\n========== ACCESS HISTORY FOR " << door.getDoorLocation() << " ==========" << endl;
    vector<LogEntry> doorLogs = accessLog.getLogsByDoor(doorId);
    accessLog.displayLogs(doorLogs);
}

void CampusSystem::showStatistics() {
    cout << "\n========== SYSTEM STATISTICS ==========" << endl;
    cout << "Total Access Attempts: " << accessLog.getTotalAttempts() << endl;
    cout << "Successful: " << accessLog.getSuccessfulAttempts() << endl;
    cout << "Failed: " << accessLog.getFailedAttempts() << endl;
    cout << "Success Rate: " << accessLog.getSuccessRate() << "%" << endl;
    cout << "=======================================" << endl;
}

void CampusSystem::displayAllRules() {
    policyEngine.displayAllRules();
}


void CampusSystem::simulateAccess(string userId, string doorId, int hour) {
    cout << "\n========== SIMULATING ACCESS ==========" << endl;

    User user = getUserById(userId);
    Door door = getDoorById(doorId);

    if (user.getId().empty() || door.getDoorId().empty()) {
        cout << "Invalid user or door!" << endl;
        return;
    }

    cout << "User: " << user.getName() << " (" << user.getRole() << ")" << endl;
    cout << "Door: " << door.getDoorLocation() << " (" << door.getDoorType() << ")" << endl;
    cout << "Time: " << hour << ":00" << endl;
    cout << "=======================================" << endl;

    bool granted = processAccess(userId, doorId, hour);

    cout << "\nResult: " << (granted ? "ACCESS GRANTED" : "ACCESS DENIED") << endl;
}