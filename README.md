# Capstone-Project
Overview:
The Smart Campus Access & Analytics System is a C++ application designed to manage and regulate access to campus facilities. It uses a rule-based policy engine to determine whether a user is allowed to access a specific location. The system integrates authentication, logging, file handling, and graph-based navigation.The design focuses on modularity, scalability, and maintainability.

Features:
User authentication and management
Door and facility access control
Rule-based policy evaluation
Access logging and analytics
File-based data persistence
Campus navigation using graph algorithms
Centralized system control

->System Architecture
User Class:
Represents individuals interacting with the system.

Responsibilities:
Store user details (ID, name, role, password)
Verify user credentials
Convert data to and from file format

Door Class:
Represents campus access points.

Attributes:
Door ID
Location
Access level
Door type

AccessRule Class:
Defines rules for granting or denying access.

Key Elements:
Role-based access control
Door type restrictions
Time-based conditions
Priority-based rule selection

Core Functions:
Check if a rule applies
Determine allow or deny decision

PolicyEngine Class:
Evaluates rules and makes access decisions.

Process:
Identify matching rules
Select the highest priority rule
Return access decision (granted or denied)

LogEntry:
Stores details of each access attempt.

Includes:
User information
Door information
Access result
Reason and applied rule
Timestamp

AccessLog Class:
Manages access history.

Capabilities:
Store and retrieve logs
Filter logs by user or door
Generate statistics (total, success, failure rate)

FileManager Class:
Handles file-based storage.

Functions:
Load and save users, doors, and logs
Search data
Ensure persistence across system runs

CampusGraph Class:
Implements graph-based campus navigation.

Features:
Shortest path calculation
Minimum Spanning Tree (Kruskal)
BFS and DFS traversal
Eulerian and Hamiltonian path checks

CampusSystem Class:
Central controller that integrates all components.

Responsibilities:
Initialize system data
Process access requests
Coordinate between modules
Manage system operations

System Workflow:
User scans ID at a door
System retrieves user and door data
Policy engine evaluates rules
Access is granted or denied
Result is logged
Logs and statistics are available for review

Technologies Used:
C++
Object-Oriented Programming
File Handling
Graph Algorithms

Conclusion:
This system provides a structured and scalable solution for campus access control. Its modular design ensures maintainability, while the rule-based engine and logging system provide reliability and transparency.
