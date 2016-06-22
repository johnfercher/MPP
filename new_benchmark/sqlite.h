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
	void open();
	void close();
	
public:
	SQLite(string, string);
	

	void insert_workspace(string robots, string obstacles, int qtd_robots, int qtd_obstacles);
};

#endif // _SQLite_H_