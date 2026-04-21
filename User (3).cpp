#include"User.h"
#include<string>
#include<sstream>

User::User() {
    ID = "";
    name = "";
    role = "";
    password = "";
}

User::User(string i, string na, string rol, string pass) {
    ID = i;
    name = na;
    role = rol;
    password = pass;
}


string User::getId() const {
    return ID;
}

string User::getName() const {
    return name;
}

string User::getRole() const {
    return role;
}

string User::getPassword() const {
    return password;
}


void User::setName(string na) {
    name = na;
}

void User::setRole(string ro) {
    role = ro;
}

void User::setPassword(string pass) {
    password = pass;
}


bool User::verifyPassword(string passHash) const{
    unsigned long hash = 0;
    for (int i = 0; i < passHash.size(); i++) {
        hash = hash * 31 + passHash[i];
    }


    return to_string(hash) == password;
}


string User::toString() const{
    return ID + "|" + name + "|" + role + "|" + password;
}

User User::fromString(string line) {
    vector<string> str;
    string current = "";

    for (int i = 0; i < line.size(); i++) {
        if (line[i] == '|' ) {
            str.push_back(current);
            current = "";
        }
        else {
            current += line[i];
        }
    }

    str.push_back(current);

    if (str.size() > 0) return User(str[0], str[1], str[2], str[3]);

    return User();

}