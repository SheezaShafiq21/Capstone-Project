#pragma once

#include<string>
#include<vector>


using namespace std;


class Door {
private:
	string doorId;
	string doorLocation;
	int accessLevel;
	string doorType;

public:
	Door();
	Door(string Id, string location, int level, string type);
	//setters

	void setDoorId(string Id);
	void setDoorLocation(string location);
	void setAccessLevel(int level);
	void setDoorType(string type);

	//getters

	string getDoorId() const;
	string getDoorLocation() const;
	
	int getAccessLevel() const;

	string getDoorType() const;


	string toString() const;

	static Door fromString(string);
};


