#pragma once


#include "FileManager.h"
#include "PolicyEngine.h"
#include "AccessLog.h"
#include "User.h"
#include "Door.h"
#include <vector>

using namespace std;

class CampusSystem {
private:
    FileManager fileManager;
    PolicyEngine policyEngine;
    AccessLog accessLog;
    vector<User> users;
    vector<Door> doors;

public:
    CampusSystem();
    void initialize();
    bool processAccess(string userId, string doorId, int currentHour);

    User getUserById(string userId);
    vector<User> getAllUsers();
    void displayAllUsers();

    Door getDoorById(string doorId);
    vector<Door> getAllDoors();
    void displayAllDoors();

    void displayRecentLogs(int count = 10);
    void displayUserHistory(string userId);
    void displayDoorHistory(string doorId);
    void showStatistics();

    void displayAllRules();

    void simulateAccess(string userId, string doorId, int hour);
};