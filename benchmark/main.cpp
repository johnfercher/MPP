#include "iostream"
#include "vector"
#include "sstream"
#include "fstream"

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

int main(){
	stringstream aux;
	ofstream write;

	vector<string> workspaces = getAllWorkspaces();

	aux << "benchmark/results/" << "teste.txt";
    /*cout << workspaces.size() << endl;
    for(int i = 0 ; i < workspaces.size() ; i++){
        cout << i << " - " << workspaces.at(i) << endl;
    }*/
	write.open(aux.str());

	for(int i = 0 ; i < workspaces.size() ; i++){
        cout << i << endl;
		stringstream ss;
		ss << "./MPP " << workspaces.at(i);
		string s = cmdTerminal(ss.str());
        s = cleanString(s);
		write << s << endl;
	}

	write.close();

	return 0;
}