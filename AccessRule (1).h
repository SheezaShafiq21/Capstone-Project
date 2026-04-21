#pragma once
#include<string>
#include<vector>
#include"Door.h"
#include"User.h"


using namespace std;

enum RuleAction {
	ALLOW,
	DENY
};


enum TimePeriod {
	ANYTIME,
	DAYTIME,
	NIGHTTIME,
	OFFICE_HOURS,
	CUSTOM
};


class AccessRule {
private:
	string ruleId;
	string ruleName;
	string description;
	string targetRole;
	string targetDoorType;
	int minAccessLevel;

	TimePeriod timePeriod;
	int startHour;
	int endHour;

	RuleAction action;
	int priority;


public:
	AccessRule();
	AccessRule(string id, string name, string desc, string role, string doorType, int accesLevel, RuleAction act, int pri = 0);


	//setters
	void setRuleId(string id);
	void setRuleName(string name);
	void setDescription(string desc);
	void setTargetRole(string role);
	void setTargetDoorType(string doorType);
	void setMinAccessLevel(int level);
	void setTimePeriod(TimePeriod period);
	void setCustomTime(int start, int end);
	void setAction(RuleAction act);
	void setPriority(int pri);


	//getters
	string getRuleId() const;
	string getRuleName() const;
	string getDescription() const;
	string getTargetRole() const;
	string getTargetDoorType() const;
	int getMinAccessLevel() const;
	TimePeriod getTimePeriod() const;
	int getStartHour() const;
	int getEndHour() const;
	RuleAction getAction() const;
	int getPriority() const;



	bool matches(const User& user, const Door& door, int currentHour) const;

	bool allowsAccess() const;
	string toString() const;
	string toFileString() const;
	static AccessRule fromFileString(string line);


};