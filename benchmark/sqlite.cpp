#include "sqlite.h"

SQLite::SQLite(string path_database, string password){
	this->path_database = path_database;
	this->password = password;
}

void SQLite::open(){
	status_db = sqlite3_open(path_database.c_str(), &db);
	
	if(status_db){
		fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
	}
}

void SQLite::close(){
	sqlite3_close(db);
}

int SQLite::callback(void *NotUsed, int argc, char **argv, char **azColName){
   int i;
   for(i=0; i<argc; i++){
      printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
   }
   printf("\n");
   return 0;
}

void SQLite::clean_query(){
	query.str(string());
    query.clear();
}

void SQLite::insert_workspace(string workspace, bool success, int qtd_robots, int qtd_obstacles, int qtd_collisions, int qtd_steps){
	clean_query();

	query << "INSERT INTO workspace (id, success, qtd_robots, qtd_obstacles, qtd_collisions, qtd_steps) ";
	
	query << "VALUES (";
	query << workspace;
	query << ", " << success ? 1 : 0;
	query << ", " << qtd_robots;
	query << ", " << qtd_obstacles;
	query << ", " << qtd_collisions;
	query << ", " << qtd_steps;
	query << ");";

	status_db = sqlite3_exec(db, query.str().c_str(), callback, 0, &error_query);

	if(status_db != SQLITE_OK){
		fprintf(stderr, "SQL error: %s\n", error_query);
		sqlite3_free(error_query);
	}
}

void SQLite::read_workspace(){
	clean_query();

	query << "SELECT * FROM workspace;";

	status_db = sqlite3_exec(db, query.str().c_str(), callback, (void*)data, &error_query);
	
	if(status_db != SQLITE_OK){
		fprintf(stderr, "SQL error: %s\n", error_query);
		sqlite3_free(error_query);
	}/*else{
		fprintf(stdout, "Operation done successfully\n");
	}*/
}

int total_workspaces(){
	int total;

	return total;
}

int total_workspaces_success(){
	int total;

	return total;
}

float media_qtd_robots(){
	float media;

	return media;
}

int min_qtd_robots(){
	int min;

	return min;
}

int max_qtd_robots(){
	int max;

	return max;
}

float media_qtd_obstacles(){
	float media;

	return media;
}

int min_qtd_obstacles(){
	int min;

	return min;
}

int max_qtd_obstacles(){
	int max;

	return max;
}

float media_qtd_collisions(){
	float media;

	return media;
}

int min_qtd_collisions(){
	int min;

	return min;
}

int max_qtd_collisions(){
	int max;

	return max;
}

int total_workspaces_success_qtd_robots(int){
	int total;

	return total;
}	

int total_workspaces_collisions_qtd_robots(int){
	int total;

	return total;
}			
                                              
int total_workspaces_success_rage_obstacles(int, int){
	int total;

	return total;
}	

int total_workspaces_collisions_rage_obstacles(int, int){
	int total;

	return total;
}	