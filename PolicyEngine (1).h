#pragma once

#include<string>
#include<vector>
#include"AccessRule.h"
#include"User.h"
#include"Door.h"

using namespace std;


struct AccessDecision {
	bool granted;
	string reason;
	string appliedRuleId;
	int priority;
};


class PolicyEngine {
private:
	vector<AccessRule> rules;
	string rulesFilePath;
public:
	PolicyEngine(string rulesFiles = "access_rules.txt");

	void addRule(const AccessRule& rule);
	void removeRule(string ruleId);
	AccessRule getRuleById(string ruleId);
	vector<AccessRule> getAllRules() const;

	AccessDecision evaluateAccess(const User& user, const Door& door, int currentHour);

	void loadDefaultRules();
	
	bool loadRulesFromFile();
	bool saveRulesToFile();

	void displayAllRules() const;
	int getRuleCount() const;

};