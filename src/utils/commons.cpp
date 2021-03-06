/*The MIT License (MIT)

Copyright (c) 2016 Johnathan Fercher

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.
*/

#include "commons.h"

namespace common{
	void clearSS(stringstream &ss){
		ss.str(string());
	    ss.clear();
	}

	string toString(int a){
		stringstream ss;
		ss << a;
		return ss.str();
	}

	string toString(float a){
		stringstream ss;
		ss << a;
		return ss.str();
	}

	string toString(double a){
		stringstream ss;
		ss << a;
		return ss.str();
	}

	string toString(long long int a){
		stringstream ss;
		ss << a;
		return ss.str();
	}

	string toString(bool a){
		string s;
		if(a) s = "1";
		else s = "0";
		return s;
	}

	int toInt(char a){
		int aa;
		stringstream ss;
		ss << a;
		ss >> aa;
		return aa; 
	}

	int toInt(string a){
		int aa;
		stringstream ss;
		ss << a;
		ss >> aa;
		return aa; 
	}

	float toFloat(string a){
		float aa;
		stringstream ss;
		ss << a;
		ss >> aa;
		return aa; 
	}

	double toDouble(string a){
		double aa;
		stringstream ss;
		ss << a;
		ss >> aa;
		return aa; 
	}

	long long int toLongLongInt(string a){
		long long int aa;
		stringstream ss;
		ss << a;
		ss >> aa;
		return aa; 
	}

	bool toBool(string a){
		string b = "0";
		if(a.compare(b) == 0) return false;
		else return true;
	}

	string cmdTerminal(string s){
		char buffer[1024];
	    std::string result = "";

	    FILE* pipe = popen(s.c_str(), "r");
	    if (!pipe) return "ERROR";
	    
	    while(!feof(pipe)) {
	    	if(fgets(buffer, 1024, pipe) != NULL)
	    		result += buffer;
	    }
	    pclose(pipe);
	    return result;
	}
}