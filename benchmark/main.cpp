#include "iostream"
#include "vector"
#include "sstream"
#include "fstream"
#include "sqlite.h"

using namespace std;

string cmdTerminal(string s){
	char buffer[1024];
    std::string result = "";

    //! > Get the exit of pipe
    FILE* pipe = popen(s.c_str(), "r");
    if (!pipe) return "ERROR";
    
    //! > Read all pipe
    while(!feof(pipe)) {
    	if(fgets(buffer, 1024, pipe) != NULL)
    		result += buffer;
    }

    pclose(pipe);

    return result;
}

vector<string> getAllWorkspaces(){
	vector<string> workspaces;
	string response = cmdTerminal("ls src/data/workspaces/");
    string sp = "";

    for(int i = 0 ; i < response.size() ; i++){
        if(response[i] == '\n'){
            stringstream sap;
            sap << response[i-1];
            workspaces.push_back(sp);
            //cout << workspaces.at(workspaces.size()-1) << endl;
            sp = ""; 
        }else{
            sp += response[i];
        }
    }

    return workspaces;
}

vector<string> separeLines(string s){
    vector<string> new_s;
    string response = s;
    string sp = "";

    for(int i = 0 ; i < response.size() ; i++){
        if(response[i] == '\n'){
            stringstream sap;
            sap << response[i-1];
            new_s.push_back(sp);
            //cout << workspaces.at(workspaces.size()-1) << endl;
            sp = ""; 
        }else{
            sp += response[i];
        }
    }
    return new_s;
}

string cleanString(string s){
    int init_i = 0;
    string new_s;

    char old_char;
    for(init_i = 0 ; init_i < s.size() ; init_i++){
        if(s[init_i] == '\n' && old_char == s[init_i]){
            break;
        }
        old_char = s[init_i];
    }
    init_i++;

    for(int i = init_i ; i < s.size() ; i++){
        new_s += s[i];
    }

    return new_s;
}

void do_benchmark(string save){
    vector<string> workspaces = getAllWorkspaces();

    SQLite db("sqlite/mpp.db", "passwd");

    db.open();
        for(int i = 0 ; i < workspaces.size() ; i++){
            stringstream s_workspace;
            int i_success;        
            stringstream s_qtd_robots;      int i_qtd_robots;
            stringstream s_qtd_objects;     int i_qtd_objects;
            stringstream s_qtd_collisions;  int i_qtd_collisions;
            stringstream s_qtd_steps;       int i_qtd_steps;

            cout << i << endl;
            stringstream ss;
            ss << "./MPP " << workspaces.at(i);

            string s = cmdTerminal(ss.str());           // Get stdout from MPP
            s = cleanString(s);                         // Clean stdout from MPP
                                                    
            vector<string> new_s = separeLines(s);      // Separe stdout by lines
            //cout << new_s.at(5) << endl;

            s_workspace << workspaces.at(i);
            
            if(new_s.at(0)[9] == 'T'){
                i_success = 1; 
            }else{
                i_success = 0;
            }

            s_qtd_robots << new_s.at(2)[12] << new_s.at(2)[13];
            s_qtd_robots >> i_qtd_robots;
            
            s_qtd_objects << new_s.at(3)[15] << new_s.at(3)[16] << new_s.at(4)[17];
            s_qtd_objects >> i_qtd_objects;

            s_qtd_collisions << new_s.at(4)[16] << new_s.at(4)[17] << new_s.at(4)[18];
            s_qtd_collisions >> i_qtd_collisions;

            s_qtd_steps << new_s.at(5)[11] << new_s.at(5)[12] << new_s.at(5)[13] << new_s.at(5)[14];
            s_qtd_steps >> i_qtd_steps;  

            db.insert_workspace(s_workspace.str(), i_success == 1 ? true : false, i_qtd_robots, i_qtd_objects, i_qtd_collisions, i_qtd_steps);           
        }
    db.close();
}

void get_info(){
    SQLite db("sqlite/mpp.db", "passwd");
    db.open(); 



    db.close();
}

int main(int argc, char** argv){
    if(argc > 1){
        //do_benchmark(ss.str());
    }else{
        get_info();
    }

	return 0;
}