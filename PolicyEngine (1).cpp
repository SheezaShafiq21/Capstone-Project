#include<iostream>
#include<fstream>
#include"PolicyEngine.h"

//======================constructor=================

PolicyEngine::PolicyEngine(string rulesFile) {
	rulesFilePath = rulesFile;

	if (!loadRulesFromFile()) {
		cout << "No rules file found. Loading default rules......\n";
		loadDefaultRules();
		saveRulesToFile();
	}
}


//=================setting && managing rules=============

void PolicyEngine::addRule(const AccessRule& rule) {
	rules.push_back(rule);
}

void PolicyEngine::removeRule(string ruleId) {
	for (int i = 0; i < rules.size(); i++) {
		if (rules[i].getRuleId() == ruleId) {
			rules.erase(rules.begin() + i);
			cout << "Rule " << ruleId << " removed.\n";
			return;
		}
	}
	cout << "Rule " << ruleId << " not found.\n";
}


AccessRule PolicyEngine::getRuleById(string ruleId) {
	for (int i = 0; i < rules.size(); i++) {
		if (rules[i].getRuleId() == ruleId) {
			return rules[i];
		}
	}
	return AccessRule();
}

vector<AccessRule> PolicyEngine::getAllRules() const {
	return rules;
}


//============actual policy/decision making=============

AccessDecision PolicyEngine::evaluateAccess(const User& user, const Door& door, int currentHour) {
	AccessDecision decision;
	decision.granted = false;
	decision.reason = "No applicable rule found";
	decision.appliedRuleId = "";
	decision.priority = -1;

	vector<AccessRule> matchingRules;
	for (int i = 0; i < rules.size(); i++) {
		if (rules[i].matches(user, door, currentHour)) {
			matchingRules.push_back(rules[i]);
		}
	}

	if (matchingRules.empty()) {
		decision.granted = false;
		decision.reason = "No applicable rule found.";
		return decision;
	}

	AccessRule bestRule = matchingRules[0];
	for (int i = 1; i < matchingRules.size(); i++) {
		if (matchingRules[i].getPriority() > bestRule.getPriority()) {
			bestRule = matchingRules[i];
		}
		else if (matchingRules[i].getPriority() == bestRule.getPriority() && !matchingRules[i].allowsAccess()) {
			bestRule = matchingRules[i];
		}
	}


	decision.granted = bestRule.allowsAccess();
	decision.appliedRuleId = bestRule.getRuleId();
	decision.priority = bestRule.getPriority();
	decision.reason = bestRule.getDescription();

	return decision;

}

// ============default rules for the ruleaccess file============

void PolicyEngine::loadDefaultRules() {
    cout << "Loading default access rules..." << endl;

    //guards can access everything at any time so it allows everthing
    AccessRule guardRule("R001", "Guard Access", "Guards have access to all doors at all times","guard", "", -1, ALLOW, 100);
    addRule(guardRule);

    //lab attendants can access all labs at anytime 
    AccessRule labAttendantRule("R002", "Lab Attendant Access", "Lab attendants can access all labs", "lab_attendant", "lab", -1, ALLOW, 90);
    addRule(labAttendantRule);

    //teachers can access classrooms and exam rooms at anytime
    AccessRule teacherClassroom("R003", "Teacher Classroom Access", "Teachers can access classrooms anytime", "teacher", "classroom", -1, ALLOW, 80);
    addRule(teacherClassroom);

    AccessRule teacherExam("R004", "Teacher Exam Room Access", "Teachers can access exam rooms anytime", "teacher", "exam_room", -1, ALLOW, 80);
    addRule(teacherExam);

    //staff can access data center at only office hours 
    AccessRule staffDataCenter("R005", "Staff Data Center Access", "Staff members can access data center during office hours", "staff", "data_center", -1, ALLOW, 85);
    staffDataCenter.setTimePeriod(OFFICE_HOURS);
    addRule(staffDataCenter);

    //students can access classrooms during daytime
    AccessRule studentClassroom("R006", "Student Classroom Access", "Students can access classrooms during daytime", "student", "classroom", -1, ALLOW, 50);
    studentClassroom.setTimePeriod(DAYTIME);
    addRule(studentClassroom);

    //students can't access labs at night
    AccessRule studentLabNight("R007", "Student Lab Night Restriction", "Students cannot access labs at night", "student", "lab", -1, DENY, 60);
    studentLabNight.setTimePeriod(NIGHTTIME);
    addRule(studentLabNight);

    //students can't access exam rooms at anytime
    AccessRule studentExam("R008", "Student Exam Room Restriction", "Students cannot access exam rooms", "student", "exam_room", -1, DENY, 70);
    addRule(studentExam);

    //students can't access data center
    AccessRule studentDataCenter("R009", "Student Data Center Restriction", "Students cannot access data center", "student", "data_center", -1, DENY, 70);
    addRule(studentDataCenter);

    //everyone can access entrances
    AccessRule publicEntrance("R010", "Public Entrance Access", "Everyone can access public entrances",  "", "entrance", -1, ALLOW, 40);
    addRule(publicEntrance);

    cout << "Loaded " << rules.size() << " default rules." << endl;
}


//====================file handling===============================



bool PolicyEngine::loadRulesFromFile() {
	ifstream file(rulesFilePath);
	if (!file.is_open()) {
		cout << "File is uable to open\n";
		return false;
	}
	rules.clear();
	string line;
	while (getline(file, line)) {
		if (line.empty()) continue;

		AccessRule rule = AccessRule::fromFileString(line);
		if (!rule.getRuleId().empty()) {
			rules.push_back(rule);
		}
	}

	file.close();
	if (rules.size() == 0) return false;
	cout << "Loaded " << rules.size() << " rules from file." << endl;
	return true;
}

bool PolicyEngine::saveRulesToFile() {
	ofstream file(rulesFilePath);

	if (!file.is_open()) {
		cout << "file is uable to open." << endl;
		return false;
	}

	for (int i = 0; i < rules.size(); i++) {
		file << rules[i].toFileString() << endl;
	}

	file.close();
	cout << "Saved " << rules.size() << " rules to file." << endl;
	return true;
}


void PolicyEngine::displayAllRules() const {
	cout << "\n========== ALL ACCESS RULES ==========" << endl;
	cout << "Total rules: " << rules.size() << endl << endl;

	for (int i = 0; i < rules.size(); i++) {
		cout << "Rule " << (i + 1) << ":" << endl;
		cout << rules[i].toString() << endl;
	}
}


int PolicyEngine::getRuleCount() const {
	return rules.size();
}