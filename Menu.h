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
#include <algorithm>    
#include <cassert> 
#include <sstream>  


#include "Database.h"
using namespace std;

class Menu{
private:

    Database database;
    string filename;
    int notice = 0;

public:

    Menu(string filen) : database(filen), filename(filen) {
        main_menu();
    }

    // Main menu which the user can use to navigate otptions
    void main_menu() {
        cout << "\n" << endl;
        cout << "Welcome to the Video Game Database!\n";
        cout << "Games are between 1950 and 2021 (case insensitive)\n";
        cout << "-----------------------------\n";
        cout << "\n";

        while (true) {
            cout << "(A)dd a game.\n";
            cout << "(F)ind a game.\n";
            cout << "(D)elete a game.\n";
            cout << "(L)ist games.\n";
            cout << "(Q)uit\n";
            cout << "\n";
        
            cout << "Enter the letter of your choice: ";

            string line;
            getline(cin, line);
            remove_space(line);
            cout << "\n" << endl;

            if (line == "A" || line == "a") {
                return add_game();

            } else if (line == "F" || line == "f") {
                return find_game();

            } else if (line == "D" || line == "d") {
                return delete_game();

            } else if (line == "L" || line == "l") {
                return list_games();

            } else if (line == "Q" || line == "q") {
                cout << "Writing data into: " << filename << "\n";
                database.write_file(filename);
                cout << "Writing data to " << filename << " done\n" << endl;
                cout << "Program Ended\n" << endl;
                return;

            } else {
                cout << "Your input is invalid, please try again\n" << endl;
                continue;
            }
        }
    }

    void return_func() {
        while (true) {
            cout << "Press (R) or (r) to (R)eturn to main menu: ";
            string ans;
            getline(cin, ans);
            remove_space(ans);
            cout << "\n" << endl;

            if (ans == "R" || ans == "r") {
                return main_menu();
            } else {
                cout << "Please return to main menu for more options\n";
                continue;
            }
        }
    }

    
///////////////////////////////////////////////////////////////////////////////
    // Options for the menu:

    // Option 1: adding games to the database
    void add_game() {
        if (notice == 0) {
            cout << "Note:\nIf the game's name and studio are the same as another " 
            << "in the database,\nbut the year is different, it is fine to add. " 
            << "However, if all 3 are\nthe same as another in the database, the game "
            << "wll not be added. This\nfunctionality is due to remakes, remasters, "
            << "and reboots causing\ngames to have the same name and studio, but different "
            << "release year.\nThis notice will only appear once.\n" << endl;
            notice = 1;
        }

        // This section asks the user for the name of the game
        string nam; int pass = 0;
        while (pass == 0) {
            cout << "What is the name of the game: ";
            getline(cin, nam);
            cout << "\n";

            // if the input is empty or is all spaces, user is asked to retry
            if (nam.empty()) {
                cout << "Your input is empty, please try again.\n"; continue;
            }

            for (int i = 0; i < nam.length(); i++) {
                if (nam.at(i) != ' '){
                    pass = 1; break;
                }
            }  
            if (pass == 0) cout << "Your input is empty, please try again.\n";
        } 
        remove_space(nam);
    
        // This section asks the user for the studio of the game
        string studi; int pass2 = 0;
        while (pass2 == 0) {
            cout<< "What is the studio of the game: ";
            getline(cin, studi);
            cout << "\n";

            // if the input is empty or is all spaces, user is asked to retry
            if (studi.empty()) {
                cout << "Your input is empty, please try again.\n"; continue;
            }

            for (int i = 0; i < studi.length(); i++) {
                if (studi.at(i) != ' '){
                    pass2 = 1; break;
                }
            }  
            if (pass2 == 0) cout << "Your input is empty, please try again.\n";
        } 
        remove_space(studi);

        // This section asks the user for the release year of the game
        int pass3 = 0; string rel_year; int new_year = 0;
        while (pass3 == 0) {
            cout << "What is the game's release year (1950-2021): ";
            getline(cin, rel_year);
            remove_space(rel_year);
            cout << "\n";
            
            if (rel_year.empty()) {
                cout << "Your input is empty, please try again.\n"; continue;
            }

            // https://www.cplusplus.com/reference/string/string/find_first_not_of/
            // learned how to use find_first_not_of() from above link
            if (rel_year.find_first_not_of("0123456789") == std::string::npos) {
                new_year = conv(rel_year);

                if (new_year > 1950 && new_year <= 2021) {
                    pass3 = 1;
                } else {
                    cout << "Year inputted is not valid, please try again. \n";
                }

            } else {
                cout << "Year inputted is not valid, please try again. \n";
            }
        } 

        // This sections checks if a similar game is already in the data
        string n = nam; string s = studi; int y = new_year;
        if (database.game_compare(n, s, y) == false) {
            cout << "\n" << endl;
            cout << "You already have this game in the database.\n";
            cout << "No games added.\n" << endl;

            return return_func();
        }

        // if passed all previous sections, 
        // appends the user's inputted game data to the database
        Game new_game(nam, studi, new_year);
        database.append(new_game);

        cout << "\n" << endl;
        cout << "Your game has been added!\n" << endl;
        return return_func();
    }

    //-------------------------------------------------------------------//

    // Option 2: find and search for games/studios/years
    void find_game() {

        cout << "\n" << endl;
        cout << "Find a Game\n" << endl;
        cout << "-----------------------------\n";
        cout << "\n";

        cout << "You can search by: \n" << endl;
        cout << "(N)ame\n";
        cout << "(S)tudio\n";
        cout << "(Y)ear of release\n";
        cout << "(F)ull Record\n";

        cout <<"\n";
        cout << "(R)eturn to main menu\n" << endl;

        int mod = 0;

        // This section asks the user how to searche
        // if input is invalid, it asks the user again for valid input
        while (mod != 10) {
            if (mod == 0) {
                cout << "Enter the letter of your choice: ";
            } else if (mod == 1) {
                cout << "Your input is invalid, please try again: ";
            }

            string line;
            getline(cin, line);
            remove_space(line);
            cout << "\n";

            // Based on the input, it calls the appropiate function for searching
            if (line == "N" || line == "n") {
                return cont_exa(1);
            
            } else if (line == "S" || line == "s") {
                return cont_exa(2);

            } else if (line == "Y" || line == "y") {
                return cont_exa(3);

            } else if (line == "R" || line == "r") {
                return main_menu(); 

            } else if (line == "F" || line == "f") {
                return cont_exa(4);
            
            } else {
                mod = 1;

            }
        }
    }

    //-------------------------------------------------------------------//

    //Option 3: Deleting Games
    void delete_game() {
        cout << "\n" << endl;
        cout << "Delete Games\n" << endl;
        cout << "-----------------------------\n";
        cout << "\n";

        cout << "You can delete by: \n" << endl;
        cout << "(N)ame\n";
        cout << "(S)tudio\n";
        cout << "(Y)ear of release\n";
        cout << "(F)ull record\n";
        
        cout << "\n";
        cout << "(R)eturn to main menu\n" << endl;

        int mod = 0;

        // This section asks the user how to delete
        // if input is invalid, it asks the user again for valid input    
        while (mod != 10) {
            if (mod == 0) {
                cout << "Enter the letter of your choice: ";
            } else if (mod == 1) {
                cout << "Your input is invalid, please try again: ";
            }

            string line;
            getline(cin, line);
            remove_space(line);
            cout << "\n";

            // Based on the input, it calls the appropiate function for deleting
            if (line == "N" || line == "n") {
                return cont_exa(5);
            
            } else if (line == "S" || line == "s") {
                return cont_exa(6);

            } else if (line == "Y" || line == "y") {
                return cont_exa(7);

            } else if (line == "R" || line == "r") {
                return main_menu();

            } else if (line == "F" || line == "f") {    
                return cont_exa(8);

            } else {
                mod = 1;

            }
        }
    }

    //-------------------------------------------------------------------//

    // Option 4: Listing the database
    void list_games() {
        cout << "\n" << endl;
        cout << "List Games\n" << endl;
        cout << "-----------------------------\n";
        cout << "\n";

        cout << "You can list by: \n" << endl;
        cout << "(1) Ascending alphabetically by names\n";
        cout << "(2) Descending alphabetically by names\n";
        cout << "\n";
        cout << "(3) Ascending alphabetically by studios\n";
        cout << "(4) Descending alphabetically by studios\n";
        cout << "\n";
        cout << "(5) Ascending by release years\n";
        cout << "(6) Descending by release years\n";
        cout << "\n";
        cout << "(R)eturn to main menu\n";
        cout << "\n";
        
        int mod = 0;

        // This section asks the user how to delete
        // if input is invalid, it asks the user again for valid input   
        while (true) {
            if (mod == 0) {
                cout << "Enter the number of your choice: ";
            } else if (mod == 1) {
                cout << "Your input is invalid, please try again: ";
            }

            string line;
            getline(cin, line);
            remove_space(line);
            cout << "\n";

            // Based on the input, it calls the appropiate function for listing
            if (line == "1") {
                database.merging(1);
                cout  << "Ascending by names: \n";
                database.println();
                return return_func();
            
            } else if (line == "2") {
                database.merging(1); database.reverse();
                cout  << "Descending by names: \n";
                database.println();
                return return_func();

            } else if (line == "3") {
                database.merging(2);
                cout  << "Ascending by studios: \n";
                database.println();
                return return_func();

            } else if (line == "4") {
                database.merging(2); database.reverse();
                cout  << "Descending by studios: \n";
                database.println();
                return return_func();

            } else if (line == "5") {    
                database.merging(3);
                cout  << "Ascending by release years: \n";
                database.println();
                return return_func();

            } else if (line == "6") {
                database.merging(3); database.reverse();
                cout  << "Descending by release years: \n";
                database.println();
                return return_func();

            } else if (line == "R" || line == "r") {
                return main_menu();

            } else {
                mod = 1;

            }
        }
    }
    
    //-------------------------------------------------------------------//

    
///////////////////////////////////////////////////////////////////////////////
    // The following functions are helper or calling funcntions for the options above

    // converts strings to numbers
    int conv(const string& a) {
	    int num;
	    stringstream ss;  
  	    ss << a;  
  	    ss >> num;
	    return num;
    }

    // Turns all the characters in a string to lowercase
    void decap(string& nam) {
        for_each(nam.begin(), nam.end(), [](char& c) {
            c = tolower(c);
        });
    }


    // https://stackoverflow.com/questions/23834624/remove-first-and-last-character-c
    // Used the link above to learn about the "erase function"
    // https://www.cplusplus.com/reference/string/string/at/
    // Used link above to learn about the "at function"
    // Removes the space left or right of the string (not within)
    void remove_space(string& word) {

        int check = 0; int pass = 0;
        int len = word.length();

        for (int i = 0; i < len; i++) {
            if (word.at(i) != ' ') {
                pass = 1; break;
            }
        }

        if (pass == 0) return;

        while (check == 0) {
            if (word.at(0) == ' ') word.erase(0,1);

            len = word.length();
            if (word.at(len-1) == ' ') word.erase(len-1);

            len  = word.length();
            if (word.at(0) == ' ' || word.at(len-1) == ' ') {
                check = 0;

            } else {
                check = 1;
            }
        }
    }

    // This function helps in searching/finding games (find_game()) 
    // and deleting games (delete_game())
    // This function asks whether the user wants to search by math or containing
    void cont_exa(int node) {   
        int mod = 0; 

        // This checks if user wants to search by exact match or by containing
        if (node == 1 || node == 2 || node == 3 || node == 5 || node == 6 || node == 7) {
            while (true) {
                cout << "Searching by: \n";
                cout << "(1) Search exact match\n";

                if (node == 3 || node == 7) 
                cout << "(2) Search between 2 specified years\n" << endl;
                else cout << "(2) Search containing sub words\n" << endl;

                // Asks if the user wants to search by containing or by exact
                cout << "Input: ";
                string a;
                getline(cin, a);
                remove_space(a); cout << "\n";

                if (a.empty()) {
                    cout << "Input is empty, please try again\n";
                    continue;
                } else remove_space(a);

                if (a == "1") {
                    mod = 1;
                    break;
                } else if (a == "2") {
                    mod = 2;
                    break;
                } else {
                    cout << "Invalid input. Please try again.\n";
                    continue;
                }
            }
        }
        
        string word; int num = 0; string num1; string num2; int first = 0; int second = 0;
        
        // This section checks if searching for exact strings or substrings or exact year
        if (node == 1 || node == 2 || (node == 3 && mod == 1) || node == 5 || node == 6 || 
        (node == 7 && mod == 1)) {
            while(true) {
                int checking1 = 0;

                if ((node == 3 && mod == 1) || (node == 7 && mod == 1))
                cout << "Search (1950-2021): ";
                else cout << "Search: ";

                getline(cin, word);
                remove_space(word);
                cout << "\n";

                // This section checks for invalid inputs
                if (word.empty()) {
                    cout << "Empty input, please try again.\n";
                    continue;
                } 

                for (int i = 0; i < word.length(); i++) {
                    if (word.at(i) != ' ') {
                        checking1 = 1; break;
                    }
                }
                if (checking1 == 0) {
                    cout << "Input invalid, please try again.\n"; continue;
                }

                // If user is searching for exact year, this part checks for validity
                if ((node == 3 && mod == 1) || (node == 7 && mod == 1)) {
                    if (word.find_first_not_of("0123456789") != string::npos) {
                        cout << "Your input has non numbers, please try again\n";
                        continue;
                    } 
                    
                    num = conv(word);
                    if (num < 1950 || num > 2021) {
                        cout << "Your input is not in 1950-2021, please try again\n";
                        continue;
                    }
                }

                break;
            }

        // This section checks if user is searching for release years between 2 specified years
        } else if ((node == 3 && mod == 2) || (node == 7 && mod == 2)) {
            while (true) {
                cout << "Beginning and equal year (1950-2021): "; getline(cin, num1);
                remove_space(num1);
                cout << "Before and equal year (1950-2021): "; getline(cin, num2);
                remove_space(num2); cout << "\n" << endl;

                // checks if the user inputted numbers are valid
                if (num1.empty() || num2.empty() ||
                num1.find_first_not_of("0123456789") != string::npos || 
                num2.find_first_not_of("0123456789") != string::npos) {
                    cout << "Input is invalid, please try again\n";
                    continue;
                }

                // Checks if the number is valid for searching/deleting
                first = conv(num1); second = conv(num2);
                if (first > second || first < 1950 || first > 2021 || 
                second < 1950 || second > 2021) {
                    cout << "The years you inputted are invalid, please try again\n";
                    continue;
                }
                break;
            }

        // This section checks if the program is to search or delete records 
        // by the whole record's info
        } else if (node == 4 || node == 8) {

            string n; int checking2 = 0;
            while (true) {
                cout <<"Name of the game: ";
                getline(cin, n); 

                if (n.empty()) {
                    cout << "Empty input, please try again\n";
                    continue;
                }

                for (int i = 0; i < n.length(); i++) {
                    if (n.at(i) != ' ') {
                        checking2 = 1; break;
                    }
                }

                if (checking2 == 1) {
                    remove_space(n); break;
                }

                cout << "Input invalid, please try again.\n";
            }

            string s; int checking3 = 0;
            while (true) {
                cout <<"Name of the studio: ";
                getline(cin, s);

                if (s.empty()) {
                    cout << "Empty input, please try again\n";
                    continue;
                }

                for (int i = 0; i < s.length(); i++) {
                    if (s.at(i) != ' ') {
                        checking3 = 1; break;
                    }
                }

                if (checking3 == 1) {
                    remove_space(s); break;
                }

                cout << "Input invalid, please try again.\n";
            }

            string ye; int y;
            while (true) {
                cout <<"Year of release (1950-2021): ";
                getline(cin, ye); remove_space(ye);

                if (ye.empty() || ye.find_first_not_of("0123456789") != string::npos) {
                    cout << "Input is invalid, please try again\n";
                    continue;
                }

                y = conv(ye); 
                if (y < 1950 || y > 2021) {
                    cout << "Year is invalid, please try again\n";
                    continue;
                }
                break;
            }

            cout << "\n";

            if (node == 4) {
                database.exact_all(n, s, y);
            } else if (node == 8) {
                database.exact_all_del(n, s, y);
            }

            return return_func();
        }
        
        cout << "\n" << endl;
        
        // This section returns the proper function according to user's inputs:

        // This sections is meant to call searching functions
        if (node == 1 || node == 2 || node == 3) { 
            // if the user wants to search by exact
            if (mod == 1) {
                if (node == 1) {
                    database.exact(node, word);
                } else if (node == 2) {
                    database.exact(node, word);
                } else if (node == 3) {
                    database.exact(node, word);
                }

            // if the user wants to search containing sub word or between 2 nums
            } else if (mod == 2) {
                if (node == 1) {
                    database.contain(node, word);
                } else if (node == 2) {
                    database.contain(node, word);
                } else if (node == 3) {
                    database.contain_num(first, second);
                }
            }
        
        // This section is meant to call functions for deleting recrods
        } else if (node == 5 || node == 6 || node == 7) {
            // if the user wants to delete by exact
            if (mod == 1) {
                if (node == 5) {
                    database.exact_del(node, word);
                } else if (node == 6) {
                    database.exact_del(node, word);
                } else if (node == 7) {
                    database.exact_del(node, word);
                }

            // if the user wants to delete by  containing sub words or between 2 nums
            } else if (mod == 2) {
                if (node == 5) {
                    database.contain_del(node, word);
                } else if (node == 6) {
                    database.contain_del(node, word);
                } else if (node == 7) {
                    database.contain_num_del(first, second);
                }
            }
        }
        return return_func();
    }
};

