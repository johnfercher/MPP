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

void do_benchmark(string save){
    stringstream aux;
    ofstream write;

    vector<string> workspaces = getAllWorkspaces();

    aux << "benchmark/results/" << save << ".txt";
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
}

void get_info(string save){
    ofstream write;
    ifstream read;
    stringstream aux[2];
    string line;

    vector<int> success;
    vector<int> qtd_robots;
    vector<int> qtd_objects;
    vector<int> qtd_collisions;
    vector<int> qtd_steps;

    float success_rate = 0;
    float qtd_robots_rate = 0;
    float qtd_objects_rate = 0;
    float qtd_collisions_rate = 0;
    float qtd_steps_rate = 0;

    vector< vector<int> > id_robots; 
    vector< vector<int> > id_objects;

    for(int i = 0 ; i < 7 ; i++){
        vector<int> id_robot;
        id_robot.push_back(-1);
        id_robots.push_back(id_robot);
    }

    for(int i = 0 ; i < 5 ; i++){
        vector<int> id_object;
        id_object.push_back(-1);
        id_objects.push_back(id_object);
    }

    aux[0] << "benchmark/results/" << save << ".txt";

    /*
     * Get Info from files
     */
    read.open(aux[0].str());
        while(getline(read, line)){
            stringstream s_qtd_robots;      int i_qtd_robots;
            stringstream s_qtd_objects;     int i_qtd_objects;
            stringstream s_qtd_collisions;  int i_qtd_collisions;
            stringstream s_qtd_steps;       int i_qtd_steps;

            if(line[9] == 'T'){
                success.push_back(1);
            }else{
                success.push_back(0);
            }

            getline(read, line);
            
            getline(read, line);
            s_qtd_robots << line[12] << line[13];
            s_qtd_robots >> i_qtd_robots;
            qtd_robots.push_back(i_qtd_robots);
            
            
            getline(read, line);
            s_qtd_objects << line[15] << line[16] << line[17];
            s_qtd_objects >> i_qtd_objects;
            qtd_objects.push_back(i_qtd_objects);
            
            getline(read, line);
            s_qtd_collisions << line[16] << line[17] << line[18];
            s_qtd_collisions >> i_qtd_collisions;
            qtd_collisions.push_back(i_qtd_collisions);

            getline(read, line);
            s_qtd_steps << line[11] << line[12] << line[13] << line[14] << endl;
            s_qtd_steps >> i_qtd_steps;
            qtd_steps.push_back(i_qtd_steps);

            getline(read, line);
        }
    read.close(); 

    /* 
     * Calc GLOBAL statistics
     */
    for(int i = 0 ; i < success.size() ; i++){
        success_rate += success.at(i);
        qtd_robots_rate += qtd_robots.at(i);
        qtd_objects_rate += qtd_objects.at(i);
        qtd_collisions_rate += qtd_collisions.at(i);
        qtd_steps_rate += qtd_steps.at(i);
    }
      
    success_rate = success_rate/success.size()*100.0;
    qtd_robots_rate = qtd_robots_rate/qtd_robots.size();
    qtd_objects_rate = qtd_objects_rate/qtd_objects.size();
    qtd_collisions_rate = qtd_collisions_rate/qtd_collisions.size();
    qtd_steps_rate = qtd_steps_rate/qtd_steps.size();

    cout << "TAMANHO DO ESPACO: 1000 x 1000" << endl;
    cout << "MEDIA DE ROBOS: " << qtd_robots_rate << " - QUANTIDADE DE ROBOS (4 a 10)" << endl;
    cout << "MEDIA DE OBSTACULOS: " << qtd_objects_rate << "- QUANTIDADE DE OBSTACULOS (1 a 100)"<< endl;
    cout << "Porcentagem de resolução GLOBAL: " << success_rate << endl;
    cout << "Media de colisões GLOBAL: " << qtd_collisions_rate << endl;
    cout << "Media de steps GLOBAL: " << qtd_steps_rate << endl;

    /* 
     * Calc LOCAL statistics
     */
    for(int i = 0 ; i < success.size() ; i++){
        switch(qtd_robots.at(i)){
            case 4:{
                id_robots.at(0).push_back(i);
            }break;
            case 5:{
                id_robots.at(1).push_back(i);
            }break;
            case 6:{
                id_robots.at(2).push_back(i);
            }break;
            case 7:{
                id_robots.at(3).push_back(i);
            }break;
            case 8:{
                id_robots.at(4).push_back(i);
            }break;
            case 9:{
                id_robots.at(5).push_back(i);
            }break;
            case 10:{
                id_robots.at(6).push_back(i);
            }break; 
        }

        if(qtd_objects.at(i) >= 0 && qtd_objects.at(i) < 20){
            id_objects.at(0).push_back(i);
        }else
        if(qtd_objects.at(i) >= 20 && qtd_objects.at(i) < 40){
            id_objects.at(1).push_back(i);
        }else
        if(qtd_objects.at(i) >= 40 && qtd_objects.at(i) < 60){
            id_objects.at(2).push_back(i);
        }else
        if(qtd_objects.at(i) >= 60 && qtd_objects.at(i) < 80){
            id_objects.at(3).push_back(i);
        }else
        if(qtd_objects.at(i) >= 80 && qtd_objects.at(i) <= 100){
            id_objects.at(4).push_back(i);
        }
    }
}

int main(int argc, char** argv){
    stringstream ss;

    if(argc > 1){
        ss << argv[1];
        //do_benchmark(ss.str());
        get_info(ss.str());
    }

	return 0;
}