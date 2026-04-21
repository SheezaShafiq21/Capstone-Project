#pragma once
#include<string>
#include<vector>


using namespace std;


class User {
private:
	string ID;
	string name;
	string role;
	string password;


public:

	User();
	User(string i, string n, string nam, string pass);

	//setters

	void setName(string);
	void setRole(string);
	void setPassword(string);

	//getters

	string getId() const;
	string getName() const;
	string getRole() const;
	string getPassword() const;

	bool verifyPassword(string) const;
	string toString() const;


	static User fromString(string line);
};
