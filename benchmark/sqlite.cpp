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

int SQLite::total_workspaces(){
	clean_query();
	int total;

	query << "SELECT count(*) FROM workspace;";

	status_db = sqlite3_exec(db, query.str().c_str(), callback, (void*)data, &error_query);
	
	if(status_db != SQLITE_OK){
		fprintf(stderr, "SQL error: %s\n", error_query);
		sqlite3_free(error_query);
	}else{
		fprintf(stdout, "Operation done successfully\n");
	}

	return total;
}

int SQLite::total_workspaces_success(){
	clean_query();
	int total;

	query << "SELECT count(*) FROM workspace where success=1;";

	status_db = sqlite3_exec(db, query.str().c_str(), callback, (void*)data, &error_query);
	
	if(status_db != SQLITE_OK){
		fprintf(stderr, "SQL error: %s\n", error_query);
		sqlite3_free(error_query);
	}else{
		fprintf(stdout, "Operation done successfully\n");
	}

	return total;
}

float SQLite::media_qtd_robots(){
	clean_query();
	float media;

	query << "SELECT avg(qtd_robots) FROM workspace;";

	status_db = sqlite3_exec(db, query.str().c_str(), callback, (void*)data, &error_query);
	
	if(status_db != SQLITE_OK){
		fprintf(stderr, "SQL error: %s\n", error_query);
		sqlite3_free(error_query);
	}else{
		fprintf(stdout, "Operation done successfully\n");
	}

	return media;
}

int SQLite::min_qtd_robots(){
	clean_query();
	int min;

	query << "SELECT min(qtd_robots) FROM workspace;";

	status_db = sqlite3_exec(db, query.str().c_str(), callback, (void*)data, &error_query);
	
	if(status_db != SQLITE_OK){
		fprintf(stderr, "SQL error: %s\n", error_query);
		sqlite3_free(error_query);
	}else{
		fprintf(stdout, "Operation done successfully\n");
	}

	return min;
}

int SQLite::max_qtd_robots(){
	clean_query();
	int max;

	query << "SELECT max(qtd_robots) FROM workspace;";

	status_db = sqlite3_exec(db, query.str().c_str(), callback, (void*)data, &error_query);
	
	if(status_db != SQLITE_OK){
		fprintf(stderr, "SQL error: %s\n", error_query);
		sqlite3_free(error_query);
	}else{
		fprintf(stdout, "Operation done successfully\n");
	}

	return max;
}

float SQLite::media_qtd_obstacles(){
	clean_query();
	float media;

	query << "SELECT avg(qtd_obstacles) FROM workspace;";

	status_db = sqlite3_exec(db, query.str().c_str(), callback, (void*)data, &error_query);
	
	if(status_db != SQLITE_OK){
		fprintf(stderr, "SQL error: %s\n", error_query);
		sqlite3_free(error_query);
	}else{
		fprintf(stdout, "Operation done successfully\n");
	}

	return media;
}

int SQLite::min_qtd_obstacles(){
	clean_query();
	int min;

	query << "SELECT min(qtd_obstacles) FROM workspace;";

	status_db = sqlite3_exec(db, query.str().c_str(), callback, (void*)data, &error_query);
	
	if(status_db != SQLITE_OK){
		fprintf(stderr, "SQL error: %s\n", error_query);
		sqlite3_free(error_query);
	}else{
		fprintf(stdout, "Operation done successfully\n");
	}

	return min;
}

int SQLite::max_qtd_obstacles(){
	clean_query();
	int max;

	query << "SELECT max(qtd_obstacles) FROM workspace;";

	status_db = sqlite3_exec(db, query.str().c_str(), callback, (void*)data, &error_query);
	
	if(status_db != SQLITE_OK){
		fprintf(stderr, "SQL error: %s\n", error_query);
		sqlite3_free(error_query);
	}else{
		fprintf(stdout, "Operation done successfully\n");
	}

	return max;
}

float SQLite::media_qtd_collisions(){
	clean_query();
	float media;

	query << "SELECT avg(qtd_collisions) FROM workspace;";

	status_db = sqlite3_exec(db, query.str().c_str(), callback, (void*)data, &error_query);
	
	if(status_db != SQLITE_OK){
		fprintf(stderr, "SQL error: %s\n", error_query);
		sqlite3_free(error_query);
	}else{
		fprintf(stdout, "Operation done successfully\n");
	}

	return media;
}

int SQLite::min_qtd_collisions(){
	clean_query();
	int min;

	query << "SELECT min(qtd_collisions) FROM workspace;";

	status_db = sqlite3_exec(db, query.str().c_str(), callback, (void*)data, &error_query);
	
	if(status_db != SQLITE_OK){
		fprintf(stderr, "SQL error: %s\n", error_query);
		sqlite3_free(error_query);
	}else{
		fprintf(stdout, "Operation done successfully\n");
	}

	return min;
}

int SQLite::max_qtd_collisions(){
	clean_query();
	int max;

	query << "SELECT max(qtd_collisions) FROM workspace;";

	status_db = sqlite3_exec(db, query.str().c_str(), callback, (void*)data, &error_query);
	
	if(status_db != SQLITE_OK){
		fprintf(stderr, "SQL error: %s\n", error_query);
		sqlite3_free(error_query);
	}else{
		fprintf(stdout, "Operation done successfully\n");
	}

	return max;
}


int SQLite::total_workspaces_qtd_robots(int qtd){
	clean_query();
	int total;

	query << "SELECT count(*) FROM workspace where qtd_robots=" << qtd << ";";

	status_db = sqlite3_exec(db, query.str().c_str(), callback, (void*)data, &error_query);
	
	if(status_db != SQLITE_OK){
		fprintf(stderr, "SQL error: %s\n", error_query);
		sqlite3_free(error_query);
	}else{
		fprintf(stdout, "Operation done successfully\n");
	}

	return total;
}	

int SQLite::total_workspaces_success_qtd_robots(int qtd){
	clean_query();
	int total;

	query << "SELECT count(*) FROM workspace where qtd_robots=" << qtd << " AND success=1;";

	status_db = sqlite3_exec(db, query.str().c_str(), callback, (void*)data, &error_query);
	
	if(status_db != SQLITE_OK){
		fprintf(stderr, "SQL error: %s\n", error_query);
		sqlite3_free(error_query);
	}else{
		fprintf(stdout, "Operation done successfully\n");
	}

	return total;
}	

int SQLite::total_workspaces_collisions_qtd_robots(int qtd){
	clean_query();
	int total;

	query << "SELECT sum(qtd_collisions) FROM workspace where qtd_robots=" << qtd << ";";

	status_db = sqlite3_exec(db, query.str().c_str(), callback, (void*)data, &error_query);
	
	if(status_db != SQLITE_OK){
		fprintf(stderr, "SQL error: %s\n", error_query);
		sqlite3_free(error_query);
	}else{
		fprintf(stdout, "Operation done successfully\n");
	}

	return total;
}			
          

int SQLite::total_workspaces_range_obstacles(int min, int max){
	clean_query();
	int total;

	query << "SELECT count(*) FROM workspace where qtd_obstacles between " << min << " and " << max << ";";

	status_db = sqlite3_exec(db, query.str().c_str(), callback, (void*)data, &error_query);
	
	if(status_db != SQLITE_OK){
		fprintf(stderr, "SQL error: %s\n", error_query);
		sqlite3_free(error_query);
	}else{
		fprintf(stdout, "Operation done successfully\n");
	}

	return total;
}	

int SQLite::total_workspaces_success_range_obstacles(int min, int max){
	clean_query();
	int total;

	query << "SELECT count(*) FROM workspace where success=1 and qtd_obstacles between " << min << " and " << max << ";";

	status_db = sqlite3_exec(db, query.str().c_str(), callback, (void*)data, &error_query);
	
	if(status_db != SQLITE_OK){
		fprintf(stderr, "SQL error: %s\n", error_query);
		sqlite3_free(error_query);
	}else{
		fprintf(stdout, "Operation done successfully\n");
	}

	return total;
}	

int SQLite::total_workspaces_collisions_range_obstacles(int min, int max){
	clean_query();
	int total;

	query << "SELECT sum(qtd_collisions) FROM workspace where qtd_obstacles between " << min << " and " << max << ";";

	status_db = sqlite3_exec(db, query.str().c_str(), callback, (void*)data, &error_query);
	
	if(status_db != SQLITE_OK){
		fprintf(stderr, "SQL error: %s\n", error_query);
		sqlite3_free(error_query);
	}else{
		fprintf(stdout, "Operation done successfully\n");
	}

	return total;
}	