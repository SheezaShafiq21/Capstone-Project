#include"FileManager.h"
#include<string>
#include<vector>
#include<fstream>
#include<iostream>

//===============constructor===========================

FileManager::FileManager(string userFile, string doorFile, string accessLogFile) {
	userFilePath = userFile;
	doorFilePath = doorFile;
	accessLogFilePath = accessLogFile;
}


//========================user file===========================

vector<User> FileManager::loadUsers() {
	vector<User> users;

	string line;

	ifstream file(userFilePath);
	if (!file.is_open()) {
		cout << "File failed to open: " << userFilePath << endl;
		return users;
	}

	while (getline(file, line)) {
		if (line.empty()) {
			continue;
		}

		User user = User::fromString(line);
		if (!user.getId().empty()) {
			users.push_back(user);
		}


	}
	file.close();
	cout << "Users Loaded from the file: " << users.size() << endl;
	return users;
}


//save users into the file 


bool FileManager::saveUsers(const vector<User>& users) {
	ofstream file(userFilePath);
	if (!file.is_open()) {
		cout << "File failed to open: " << userFilePath << endl;
		return false;
	}
	for (User it : users){
		file << it.toString() << endl;
	}
	file.close();
	cout << "Saved users to the txt file\n";
	return true;
}


//adding a single user in the file

bool FileManager::addUser(const User& user) {
	ofstream file(userFilePath, ios::app);
	if (!file.is_open()) {
		cout << "File failed to open: " << userFilePath << endl;
		return false;
	}
	file << user.toString() << endl;
	file.close();
	cout << "User: " << user.getId() << " successfully added.\n";
	return true;
}


//finding a user by its Id

User FileManager::findUserById(string userId) {
	vector<User> users = loadUsers();
	for (int i = 0; i < users.size(); i++) {
		if (users[i].getId() == userId) {
			return users[i];
		}
	}
	return User();

}


//=========================door file============================================

//loading doors into the vector/array


vector<Door> FileManager::loadDoors() {
	vector<Door> doors;

	string line;

	ifstream file(doorFilePath);
	if (!file.is_open()) {
		cout << "File failed to open: " << doorFilePath << endl;
		return doors;
	}

	while (getline(file, line)) {
		if (line.empty()) {
			continue;
		}

		Door door = Door::fromString(line);
		if (!door.getDoorId().empty()) {
			doors.push_back(door);
		}


	}
	file.close();
	cout << "Users Loaded from the file: " << doors.size() << endl;
	return doors;
}



//saving doors into the file

bool FileManager::saveDoors(const vector<Door>& doors) {
	ofstream file(doorFilePath);

	if (!file.is_open()) {
		cout << "File failed to open: " << doorFilePath << endl;
		return false;
	}


	for (int i = 0; i < doors.size(); i++) {
		file << doors[i].toString() << endl;
	}

	file.close();
	cout << "Saved " << doors.size() << " doors to file." << endl;
	return true;
}


//adding single door to the file

bool FileManager::addDoor(const Door& door) {
	ofstream file(doorFilePath, ios::app);

	if (!file.is_open()) {
		cout << "File failed to open: " << doorFilePath << endl;
		return false;
	}


	file << door.toString() << endl;
	file.close();

	cout << "Added door " << door.getDoorId() << " to file." << endl;
	return true;
}


//finding a door by its ID

Door FileManager::findDoorById(string doorId) {
	vector<Door> doors = loadDoors();

	for (int i = 0; i < doors.size(); i++) {
		if (doors[i].getDoorId() == doorId) {
			return doors[i];
		}
	}

	return Door(); 
}


// Check if a file exists
bool FileManager::fileExists(string filepath) {
	ifstream file(filepath);
	bool exists = file.is_open();
	file.close();
	return exists;
}

// Getters for file paths
string FileManager::getUsersFilePath() const {
	return userFilePath;
}

string FileManager::getDoorsFilePath() const {
	return doorFilePath;
}

string FileManager::getAccessLogFilePath() const {
	return accessLogFilePath;
}