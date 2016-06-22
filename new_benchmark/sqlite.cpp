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

void SQLite::insert_workspace(string robots, string obstacles, int qtd_robots, int qtd_obstacles){
	clean_query();

	query << "INSERT INTO workspace (robots, obstacles, qtd_robots, qtd_obstacles";
	
	query << "VALUES (";
	query << robots;
	query << ", " << obstacles;
	query << ", " << qtd_robots;
	query << ", " << qtd_obstacles;
	query << ");";

	open();

	status_db = sqlite3_exec(db, query.str().c_str(), callback, 0, &error_query);

	if(status_db != SQLITE_OK){
		fprintf(stderr, "SQL error: %s\n", error_query);
		sqlite3_free(error_query);
	}

	close();
}
