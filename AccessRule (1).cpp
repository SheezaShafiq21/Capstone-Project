#include"AccessRule.h"
#include<sstream>


//==========================constructors==========================

AccessRule::AccessRule() {
    ruleId = "";
    ruleName = "";
    description = "";
    targetRole = "";
    targetDoorType = "";
    minAccessLevel = -1;
    timePeriod = ANYTIME;
    startHour = 0;
    endHour = 23;
    action = ALLOW;
    priority = 0;
}



AccessRule::AccessRule(string id, string name, string desc, string role, string doorType, int accessLvl, RuleAction act, int pri) {
    ruleId = id;
    ruleName = name;
    description = desc;
    targetRole = role;
    targetDoorType = doorType;
    minAccessLevel = accessLvl;
    timePeriod = ANYTIME;
    startHour = 0;
    endHour = 23;
    action = act;
    priority = pri;
}


//===================setters=================================


void AccessRule::setRuleId(string id){
    ruleId = id;
}
void AccessRule::setRuleName(string name) {
    ruleName = name; 
}
void AccessRule::setDescription(string desc) { 
    description = desc; 
}
void AccessRule::setTargetRole(string role) { 
    targetRole = role; 
}
void AccessRule::setTargetDoorType(string doorType) { 
    targetDoorType = doorType; 
}
void AccessRule::setMinAccessLevel(int level) { 
    minAccessLevel = level; 
}
void AccessRule::setTimePeriod(TimePeriod period) { 
    timePeriod = period; 
}

void AccessRule::setCustomTime(int start, int end) {
    timePeriod = CUSTOM;
    startHour = start;
    endHour = end;
}

void AccessRule::setAction(RuleAction act) { 
    action = act; 
}
void AccessRule::setPriority(int pri) { 
    priority = pri; 
}

//=======================getter==========================

string AccessRule::getRuleId() const { 
    return ruleId; 
}
string AccessRule::getRuleName() const { 
    return ruleName; 
}
string AccessRule::getDescription() const { 
    return description; 
}
string AccessRule::getTargetRole() const { 
    return targetRole; 
}
string AccessRule::getTargetDoorType() const { 
    return targetDoorType; 
}
int AccessRule::getMinAccessLevel() const { 
    return minAccessLevel; 
}
TimePeriod AccessRule::getTimePeriod() const { 
    return timePeriod; 
}
int AccessRule::getStartHour() const { 
    return startHour; 
}
int AccessRule::getEndHour() const { 
    return endHour; 
}
RuleAction AccessRule::getAction() const { 
    return action; 
}
int AccessRule::getPriority() const { 
    return priority; 
}



bool AccessRule::matches(const User& user, const Door& door, int currentHour) const {

    bool roleMatches = (targetRole.empty() || targetRole == user.getRole());
    bool doorTypeMatches = (targetDoorType.empty() || targetDoorType == door.getDoorType());
    bool accessLevelMatches = (minAccessLevel == -1 || minAccessLevel <= door.getAccessLevel());

    bool timeMatches = false;

    switch (timePeriod)
    {
    case ANYTIME:
        timeMatches = true;
        break;
    case DAYTIME:
        timeMatches = (currentHour >= 8 && currentHour <= 18);
        break;
    case NIGHTTIME:
        timeMatches = (currentHour >= 22 || currentHour <= 6);
        break;
    case OFFICE_HOURS:
        timeMatches = (currentHour >= 9 && currentHour <= 17);
        break;
    case CUSTOM:
        if (startHour <= endHour) {
            timeMatches = (currentHour >= startHour && currentHour <= endHour);
        }
        else {
            timeMatches = (currentHour >= startHour || currentHour <= endHour);
        }
        break;
    default:
        break;
    }

    return roleMatches && doorTypeMatches && accessLevelMatches && timeMatches;
}



bool AccessRule::allowsAccess() const {
    return (action == ALLOW);
}




string AccessRule::toString() const {
    string result = "Rule: " + ruleName + "\n";
    result += "  ID: " + ruleId + "\n";
    result += "  Description: " + description + "\n";
    result += "  Target Role: " + (targetRole.empty() ? "ANY" : targetRole) + "\n";
    result += "  Target Door Type: " + (targetDoorType.empty() ? "ANY" : targetDoorType) + "\n";
    result += "  Min Access Level: ";
    if (minAccessLevel == -1) {
        result += "NONE\n";
    }
    else {
        result += to_string(minAccessLevel) + "\n";
    }
    result += "  Action: " + string(action == ALLOW ? "ALLOW" : "DENY") + "\n";
    result += "  Priority: " + to_string(priority) + "\n";
    return result;

}


string AccessRule::toFileString() const {
    string result = ruleId + "|" + ruleName + "|" + description + "|";
    result += targetRole + "|" + targetDoorType + "|";
    result += to_string(minAccessLevel) + "|";
    result += to_string(timePeriod) + "|";
    result += to_string(startHour) + "|" + to_string(endHour) + "|";
    result += to_string(action) + "|" + to_string(priority);
    return result;
}

AccessRule AccessRule::fromFileString(string line) {
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

    if (parts.size() <= 0) {
        return AccessRule();
    }
    int level = stoi(parts[5]);


    AccessRule rule;
    rule.setRuleId(parts[0]);
    rule.setRuleName(parts[1]);
    rule.setDescription(parts[2]);
    rule.setTargetRole(parts[3]);
    rule.setTargetDoorType(parts[4]);
    rule.setMinAccessLevel(stoi(parts[5]));
    rule.setTimePeriod(static_cast<TimePeriod>(stoi(parts[6])));
    rule.startHour = stoi(parts[7]);
    rule.endHour = stoi(parts[8]);
    rule.setAction(static_cast<RuleAction>(stoi(parts[9])));
    rule.setPriority(stoi(parts[10]));

    return rule;
}