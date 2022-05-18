/////////////////////////////////////////////////////////////////////////
//
// Student Info
// ------------
//
// Name : Prince Emmanuel Nitafan
// St.# : 301445340
// Email: pen@sfu.ca
//
//
// Statement of Originality
// ------------------------
//
// All the code and comments below are my own original work. For any non-
// original work, I have provided citations in the comments with enough
// detail so that someone can see the exact source and extent of the
// borrowed work.
//
// In addition, I have not shared this work with anyone else, and I have
// not seen solutions from other students, tutors, websites, books,
// etc.
//
/////////////////////////////////////////////////////////////////////////

#include "cmpt_error.h" 
#include <iostream>     
#include <fstream>     
#include <string>          
#include <cassert> 
#include <sstream>   

using namespace std;

class Game {
    string name;
    string studio;
    int year;

public:    

    Game(const string& nam, const string stud, int ye) : name(nam), studio(stud), year(ye) {}

    Game() {
        name = "n/a"; studio = "n/a"; year = 0;
    }

    string get_name() {
        return name;
    }

    string get_studio() {
        return studio;
    }

    int get_year() {
        return year;
    }

	void printing_gam() {
        cout << "[" << name << " | " << studio << " | " 
		<< to_string(year) << "]\n";
    }

};

