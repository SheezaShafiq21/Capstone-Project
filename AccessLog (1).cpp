#include "AccessLog.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <iomanip>

//========logentry construcotrs=======================

LogEntry::LogEntry() {
    timestamp = 0;
    dateTime = "";
    userId = "";
    userName = "";
    userRole = "";
    doorId = "";
    doorLocation = "";
    doorType = "";
    accessGranted = false;
    reason = "";
    ruleId = "";
}

LogEntry::LogEntry(const User& user, const Door& door, bool granted, string reasonStr, string ruleIdStr) {
    userId = user.getId();
    userName = user.getName();
    userRole = user.getRole();
    doorId = door.getDoorId();
    doorLocation = door.getDoorLocation();
    doorType = door.getDoorType();
    accessGranted = granted;
    reason = std::move(reasonStr);
    ruleId = std::move(ruleIdStr);
}

string LogEntry::toString() const {
    string result = to_string(timestamp) + "|" + dateTime + "|";
    result += userId + "|" + userName + "|" + userRole + "|";
    result += doorId + "|" + doorLocation + "|" + doorType + "|";
    result += static_cast<bool>(accessGranted ? "1" : "0") + "|" + reason + "|" + ruleId;
    return result;
}


LogEntry LogEntry::fromString(string line) {
    vector<string> parts;
    string current = "";

    for (int i = 0; i < line.length(); i++) {
        if (line[i] == '|') {
            parts.push_back(current);
            current = "";
        }
        else {
            current += line[i];
        }
    }
    parts.push_back(current);

    if (parts.size() < 11) {
        return LogEntry();  
    }

    LogEntry entry;
    entry.timestamp = stoll(parts[0]); 
    entry.dateTime = parts[1];
    entry.userId = parts[2];
    entry.userName = parts[3];
    entry.userRole = parts[4];
    entry.doorId = parts[5];
    entry.doorLocation = parts[6];
    entry.doorType = parts[7];
    entry.accessGranted = (parts[8] == "1");
    entry.reason = parts[9];
    entry.ruleId = parts[10];

    return entry;
}



//========================access log================================

AccessLog::AccessLog(string logFile) {
    logFilePath = logFile;
    loadLogs();  
}


void AccessLog::logAccess(const User& user, const Door& door, bool granted, string reason, string ruleId) {

    LogEntry entry(user, door, granted, reason, ruleId);
    logs.push_back(entry);
    saveLog(entry);

    cout << "\n[LOG] " << entry.dateTime << " | " << user.getName() << " | " << door.getDoorLocation() << " | " << (granted ? "GRANTED" : "DENIED") << endl;
}



bool AccessLog::loadLogs() {
    ifstream file(logFilePath);

    if (!file.is_open()) {
        cout << "No existing log file found. Starting fresh." << endl;
        return false;
    }

    logs.clear();
    string line;

    while (getline(file, line)) {
        if (line.empty()) continue;

        LogEntry entry = LogEntry::fromString(line);
        if (!entry.userId.empty()) {
            logs.push_back(entry);
        }
    }

    file.close();
    cout << "Loaded " << logs.size() << " log entries." << endl;
    return true;
}



bool AccessLog::saveLog(const LogEntry& entry) {
    ofstream file(logFilePath, ios::app);

    if (!file.is_open()) {
        cout << "Error: Could not open log file for writing." << endl;
        return false;
    }

    file << entry.toString() << endl;
    file.close();
    return true;
}



vector<LogEntry> AccessLog::getAllLogs() const {
    return logs;
}


vector<LogEntry> AccessLog::getLogsByUser(string userId) const {
    vector<LogEntry> result;

    for (int i = 0; i < logs.size(); i++) {
        if (logs[i].userId == userId) {
            result.push_back(logs[i]);
        }
    }

    return result;
}



vector<LogEntry> AccessLog::getLogsByDoor(string doorId) const {
    vector<LogEntry> result;

    for (int i = 0; i < logs.size(); i++) {
        if (logs[i].doorId == doorId) {
            result.push_back(logs[i]);
        }
    }

    return result;
}


vector<LogEntry> AccessLog::getDeniedLogs() const {
    vector<LogEntry> result;

    for (int i = 0; i < logs.size(); i++) {
        if (!logs[i].accessGranted) {
            result.push_back(logs[i]);
        }
    }

    return result;
}


vector<LogEntry> AccessLog::getGrantedLogs() const {
    vector<LogEntry> result;

    for (int i = 0; i < logs.size(); i++) {
        if (logs[i].accessGranted) {
            result.push_back(logs[i]);
        }
    }

    return result;
}

//=======getting attemps by the users============

int AccessLog::getTotalAttempts() const {
    return logs.size();
}

int AccessLog::getSuccessfulAttempts() const {
    int count = 0;
    for (int i = 0; i < logs.size(); i++) {
        if (logs[i].accessGranted) {
            count++;
        }
    }
    return count;
}

int AccessLog::getFailedAttempts() const {
    int count = 0;
    for (int i = 0; i < logs.size(); i++) {
        if (!logs[i].accessGranted) {
            count++;
        }
    }
    return count;
}

double AccessLog::getSuccessRate() const {
    if (logs.size() == 0) return 0.0;
    return (double)getSuccessfulAttempts() / logs.size() * 100.0;
}


vector<LogEntry> AccessLog::getRecentLogs(int count) const {
    vector<LogEntry> result;

    int start = logs.size() - count;
    if (start < 0) start = 0;

    for (int i = start; i < logs.size(); i++) {
        result.push_back(logs[i]);
    }

    return result;
}

//===========displayingLogs========================

void AccessLog::displayLogs(const vector<LogEntry>& logList) const {
    if (logList.empty()) {
        cout << "No logs to display." << endl;
        return;
    }

    cout << "\n========================================" << endl;
    cout << "Total entries: " << logList.size() << endl;
    cout << "========================================" << endl;

    for (int i = 0; i < logList.size(); i++) {
        const LogEntry& log = logList[i];

        cout << "\nEntry " << (i + 1) << ":" << endl;
        cout << "  Time: " << log.dateTime << endl;
        cout << "  User: " << log.userName << " (" << log.userRole << ")" << endl;
        cout << "  Door: " << log.doorLocation << " [" << log.doorId << "]" << endl;
        cout << "  Result: " << (log.accessGranted ? "GRANTED" : "DENIED") << endl;
        cout << "  Reason: " << log.reason << endl;
        cout << "  Rule: " << log.ruleId << endl;
    }

    cout << "\n========================================" << endl;
}

void AccessLog::displayAllLogs() const {
    displayLogs(logs);
}


void AccessLog::clearLogs() {
    logs.clear();
    ofstream file(logFilePath);
    file.close();

    cout << "All logs cleared." << endl;
}

string AccessLog::getLogFilePath() const {
    return logFilePath;
}