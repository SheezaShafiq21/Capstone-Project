#include"Door.h"

#include<string>
#include<sstream>




Door::Door() {
	doorId = "";
	doorLocation = "";
	accessLevel = 0;
	doorType = "";
}


Door::Door(string Id, string location, int level, string type) {
	doorId = Id;
	doorLocation = location;
	accessLevel = level;
	doorType = type;
}

//setters


void Door::setDoorId(string Id) {
	doorId = Id;
}
void Door::setDoorLocation(string location) {
	doorLocation = location;
}
void Door::setAccessLevel(int level) {
	accessLevel = level;
}
void Door::setDoorType(string type) {
	doorType = type;
}


//getters

string Door::getDoorId() const
{
	return doorId;
};
string Door::getDoorLocation() const {
	return doorLocation;
}

int Door::getAccessLevel() const {
	return accessLevel;
}

string Door::getDoorType() const {
	return doorType;
}
//parsing


string Door::toString() const{
	return doorId + "|" + doorLocation + "|" + to_string(accessLevel) + "|" + doorType;
}


Door Door::fromString(string line) {
	vector<string> str;
	string current = "";

	for (int i = 0; i < line.size(); i++) {
		if (line[i] == '|') {
			str.push_back(current);
			current = "";
		}
		else {
			current += line[i];
		}
	}

	str.push_back(current);

	if (str.size() > 4) {
		int level = stoi(str[2]);
		return Door(str[0], str[1], level, str[3]);
	}

	return Door();
}