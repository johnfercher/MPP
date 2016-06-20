#ifndef _SQLite_H_
#define _SQLite_H_

#include <stdio.h>

#include "iostream"
#include "sstream"

#include <sqlite3.h> 

using namespace std;

class SQLite{
protected:
	sqlite3 *db;

	string path_database;
	string password;

	stringstream query;

	int status_db;
	char *error_query;
	const char* data;
	
	static int callback(void *NotUsed, int argc, char **argv, char **azColName);
	void clean_query();
public:
	SQLite(string, string);
	void open();
	void close();

	void insert_workspace(string workspace, bool success, int qtd_robots, int qtd_obstacles, int qtd_collisions, int qtd_steps);
	void read_workspace();

	int total_workspaces();
	int total_workspaces_success();
	
	float media_qtd_robots();
	int min_qtd_robots();
	int max_qtd_robots();

	float media_qtd_obstacles();
	int min_qtd_obstacles();
	int max_qtd_obstacles();

	float media_qtd_collisions();
	int min_qtd_collisions();
	int max_qtd_collisions();

	int total_workspaces_success_qtd_robots(int);				// 5 a 10
	int total_workspaces_collisions_qtd_robots(int);			// 5 a 10
	                                              
	int total_workspaces_success_rage_obstacles(int, int); 		// 1 a 100
	int total_workspaces_collisions_rage_obstacles(int, int); 	// 1 a 100
};

#endif // _SQLite_H_