#pragma once

#include<string>
#include"User.h"
#include"Door.h"
#include<vector>

using namespace std;



class FileManager {
private:
	string userFilePath;
	string doorFilePath;
	string accessLogFilePath;
public:
	FileManager(string userFile = "users.txt", string doorFile = "doors.txt", string logFile = "accesslog.txt");
	

	//=================User file operations==============================

	//loading users into vectors/arrays 

	vector<User> loadUsers();
	

	//save users into file

	bool saveUsers(const vector<User>& users);

	//add a single user to the file 

	bool addUser(const User& user);

	//find a user by Id

	User findUserById(string userId);


	//====================Door File operations==============================

	//loading doors into vector/array

	vector<Door> loadDoors();

	//save doors into the file

	bool saveDoors(const vector<Door>& doors);

	//add a single door to the file

	bool addDoor(const Door& door);

	//search a door by its Id

	Door findDoorById(string doorId);


	//=====================Utility functions===================================

	//checking if file exists

	bool fileExists(string filepath);


	//getting file paths

	string getUsersFilePath() const;
	string getDoorsFilePath() const;
	string getAccessLogFilePath() const;






};
