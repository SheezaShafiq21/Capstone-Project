#pragma once

#include <string>
#include <vector>
#include <ctime>
#include "User.h"
#include "Door.h"

using namespace std;

struct LogEntry {
    
    string userId;
    string userName;
    string userRole;
    string doorId;
    string doorLocation;
    string doorType;
    bool accessGranted;
    string reason;
    string ruleId;
    time_t timestamp;
    string dateTime;

    //===========constructor==============================
    LogEntry();
    LogEntry(const User& user, const Door& door, bool granted, string reason, string ruleId);
    string toString() const;
    static LogEntry fromString(string line);
};





class AccessLog {
private:
    string logFilePath;
    vector<LogEntry> logs; 

public:

    AccessLog(string logFile = "accesslog.txt");

    void logAccess(const User& user, const Door& door, bool granted, string reason, string ruleId);

    bool loadLogs();
    bool saveLog(const LogEntry& entry);
    vector<LogEntry> getAllLogs() const;


    vector<LogEntry> getLogsByUser(string userId) const;
    vector<LogEntry> getLogsByDoor(string doorId) const;
    vector<LogEntry> getDeniedLogs() const;
    vector<LogEntry> getGrantedLogs() const;



    int getTotalAttempts() const;
    int getSuccessfulAttempts() const;
    int getFailedAttempts() const;
    double getSuccessRate() const;


    vector<LogEntry> getRecentLogs(int count) const;

    void displayLogs(const vector<LogEntry>& logs) const;
    void displayAllLogs() const;

    void clearLogs();

    string getLogFilePath() const;
};