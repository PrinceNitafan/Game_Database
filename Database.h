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

  
#include "Game.h"
using namespace std;

// Below are helper functions not part of the class

// converts strings to ints
int conv(const string& a) {
	int num;
	stringstream ss;  
  	ss << a;  
  	ss >> num;
	return num;
}

// turns strings to lowercase
void decap(string& nam) {
    for_each(nam.begin(), nam.end(), [](char& c) {
        c = tolower(c);
    });
}

// This removes the space around the strings/nubmers
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

class Database : public Game {
private:

	int cap;
	Game* data;
	int sze;

public:


	// Constructor
	Database(void) 
	: cap(10), data(new Game[cap]), sze(0) {}

	// Destructor
	~Database() {
		cap = 0;
		delete[] data;
		sze = 0;
	}

	// size, capacity, used
	int size() const {
		return sze;
	}

	int capacity() const {
		return cap;
	}

	// to string (changed for menu.h)
	string to_str() const {
  		string stri = "style: [game | studio | year]\n\n";
		int pos = 1;
  		for (int i = 0; i < sze; i++) {
			Game game = data[i];

			string n = to_string(pos);
			stri += n; stri += ". [";
    		stri += game.get_name(); stri += " | ";
			stri += game.get_studio(); stri += " | ";
			stri += to_string(game.get_year());
      		stri += "]\n";
			pos++;
  		}
 		
  		return stri;
	}	

	void print() const {
		cout << to_str();
	}

	void println() const {
		cout << to_str() << "\n";
	}

	// get and set
	Game get(int i) const {
		if (sze == 0) {
			cmpt::error("The size is 0; no ints in the array, so can't get\n");
		} else if (i > sze - 1 || i < 0) 
		cmpt::error("The place you inputted is out of bounds for get.\n");
		else {
			Game result = data[i];
			cout << "name: " << result.get_name() << "\n" << endl;
			return result;
		}
		Game n;
		return n;
	}

	void set(int i, Game s) {
		if (sze == 0) {
			cmpt::error("The size is 0; no ints in the array, so can't set\n");
		} else if (i > sze - 1 || i < 0) {
		cmpt::error("The place you inputted is out of bounds for set.\n");
		} else {
			data[i] = s;
		}
	}

	///////////////////////////////////////////////////////////////////////////

	// appends new records to the database
	void append(Game gam) {
  		int caps = cap;
  		int siz = sze;

		// if the work 
		if (siz < 0) cmpt::error("the size is still less than zero");
		if (sze >= cap) {
			caps = caps*2;

			Game* lis = new Game[caps];
			
			for (int i = 0; i < siz; i++) {
				lis[i] = data[i];
			}

			for (int j = siz; j < caps; j++) {
				Game n;
				lis[j] = n;
			}
			
			delete[] data; 
			data = lis; 
			sze = sze + 1; cap = caps;
			data[sze-1] = gam;
			
		} else {
			sze = sze + 1;
			data[sze-1] = gam;
		}
	}

	// reverses the records in the database
	void reverse() {
		Game* lis = new Game[cap];

		int j = 0;  // This gets the last number in the OG data, 
					// and goes down by 1 each iteration
		for (int i = (sze-1); i >= 0; i--) {
			lis[i] =  data[j];
			j++;
		}

		delete[] data; 
		data = lis;
	}

	// File constructor that gets records from the files
	Database(const string& fname) 
	: cap(10), data(new Game[cap]), sze(0) {
		ifstream fin;
		string game;

  		// Checks if the file is opened
 	 	fin.open(fname);
  		if (!fin.is_open()) {
    		cout << "Can't open inputted file. \n" << endl;
			fin.close();
    		return;
  		}
		
  		// This section reads the file and appends them to an array
  		while(getline(fin, game)) {
			int checkpoint = 0;
			if (game.empty()) continue;

			for (int i = 0; i < game.length(); i++) {
				if (game.at(i) != ' ') {
					checkpoint = 1; break;
				}
			}

			if (checkpoint == 0) continue;

			if (!(game.find("|") != string::npos)) continue;

			string n; string s; string y1;

			int j = 0;
			while(game.at(j) != '|') {
				n += game.at(j);j++;
			}
			remove_space(n); j++;

			while(game.at(j) != '|') {
				s += game.at(j); j++;
			}
			remove_space(s); j++;

			while(j < game.length()) {
				y1 += game.at(j); j++;
			}
			remove_space(y1);
			int y2 = conv(y1);
			
			Game new_game(n, s, y2);
    		append(new_game);
  		}
  		fin.close();
	}

	// function to write the database records into a file
	void write_file(const string& fname) {
		ofstream fin; 

		fin.open(fname);
  		if (!fin.is_open()) {
    		cout << "Can't open inputted file. \n" << endl;
			fin.close();
    		return;
  		}

		// writes the records in a certain wat to the file
		for (int i = 0; i < sze; i++) {
			fin << data[i].get_name() << " | " << data[i].get_studio()
			<< " | " << to_string(data[i].get_year()) << endl;
		}
		fin.close();
	}

///////////////////////////////////////////////////////////////////
	// Below are functions that directly help with the menu options or
	// the options of the menu options

	// this function helps with option 1 of the menu
	// this function compares the games if there is anything similar in the database
    bool game_compare(string gam_nam, string gam_stud, int gam_ye) {
        remove_space(gam_nam); remove_space(gam_stud); decap(gam_nam); decap(gam_stud);

        for (int i = 0; i < sze; i++) {
			if (sze == 0) {
				break;
			}
            Game gm = data[i];

            string nam = gm.get_name(); 
            remove_space(nam); decap(nam);

            string stu = gm.get_studio();
            remove_space(stu); decap(stu);

            int ye = gm.get_year();

            if (gam_nam == nam && gam_stud == stu && gam_ye == ye) {
                return false;
            }
        }
        return true;
    }

	//---------------------------------------------------------------//
	// Functions below help with option 2 of the menu

	// this function searches for the exact strings or specified year
    void exact(int m, const string& inp) {
        string str = inp; int num = 1;
        remove_space(str); decap(str);
        cout << "\n" << endl;

        // This section searches for game names 
        int check = 0;
        if (m == 1) {
            cout << "Games with names: " << inp << "\n" << endl;
            for (int i = 0; i < sze; i++) {
                Game gm_nam = data[i]; 

                string nam = gm_nam.get_name();
                remove_space(nam); decap(nam);

				// writes the records found
                if (nam == str) {
                    check = 1;
                    cout << num << ". "; gm_nam.printing_gam();
					num++;
                }
            }
        // This section searches for game studios 
        } else if (m == 2) {
            cout << "Games with studios: " << str << "\n" << endl;

            for (int i = 0; i < sze; i++) {
                Game gm_stud = data[i];

                string stud = gm_stud.get_studio();
                remove_space(stud); decap(stud);

                if (stud == str) {
                    check = 2;
                    cout << num << ". "; gm_stud.printing_gam();
					num++;
                }

            }   
		// This section searches for game releases
        } else if (m == 3) {
            cout << "Games released in: " << str << "\n" << endl;
			int year_find = conv(str); 

            for (int i = 0; i < sze; i++) {
                Game gm_ye = data[i];

				int y = gm_ye.get_year();

                if (y == year_find) {
                    check = 2;
                    cout << num << ". "; gm_ye.printing_gam();
					num++;
                }
            }   
		}
        cout << "\n";

        // This section checks if search has found records or not
        if (check != 0) {
            cout << "\n";
            cout << "Search done\n" << endl;
            return;
            
        } else if (check == 0) {
            cout << "\n";
            cout << "No matching records are found\n" << endl;
            return;
        }
    }

	// this function searches for the record with the matchin name, studio, and year
	void exact_all(string gm_nam, string gm_stud, int gm_ye){
		decap(gm_nam); decap(gm_stud);

		cout << "Exact match:\n";
		int check = 0; int num = 1;
		for (int i = 0; i < sze; i++) {
			Game gam = data[i];

			string n = gam.get_name(); string s = gam.get_studio(); int y = gam.get_year();
			decap(n); decap(s);

			if (gm_nam == n && gm_stud == s && gm_ye == y) {
				check = 4;
				cout << num << ". "; gam.printing_gam();
				num++;
			}
		}

		// checks if search is successful or not
        if (check != 0) {
            cout << "\n";
            cout << "Search done\n" << endl;
            return;
            
        } else if (check == 0) {
            cout << "\n";
            cout << "No matching records are found\n" << endl;
            return;
        }
	}

	//-------------------------------------------------------//
	// functions below help with option 3 of the menu

	// this functions finds the records containing the specified substrings
    void contain(int m, const string& inp) {
        string str = inp; int num = 1;
        remove_space(str); decap(str);
        cout << "\n" << endl;

        // This section searches for game names containing certain strings
        int check = 0;
        if (m == 1) {
            cout << "Games with names that contain: " << inp << "\n" << endl;
            for (int i = 0; i < sze; i++) {
                Game gm_nam = data[i];

                string nam = gm_nam.get_name();
                remove_space(nam); decap(nam);
    
                if (nam.find(str) != string::npos) {
                    check = 1;
                    cout << num << ". "; gm_nam.printing_gam();
					num++;
                }
            }
        // This section searches for game studios containing certain strings
        } else if (m == 2) {
            cout << "Games with studios that contain: " << str << "\n" << endl;

            for (int i = 0; i < sze; i++) {
                Game gm_stud = data[i];

                string stud = gm_stud.get_studio();
                remove_space(stud); decap(stud);

                if (stud.find(str) != string::npos) {
                    check = 2;
                    cout << num << ". "; gm_stud.printing_gam();
					num++;
                }

            }   
        }

		// this sections checks if the search was successful or not
        if (check != 0) {
            cout << "\n";
            cout << "Search done\n" << endl;
            return;
            
        } else if (check == 0) {
            cout << "\n";
            cout << "No matching records are found\n" << endl;
            return;
        }
    }

	// this searches for records between specified years
    void contain_num(int year1, int year2) {
        cout << "Games released between the years: " << year1 << "-" << year2 << "\n" << endl;

        int check = 0; int num = 1;
        for (int i = 0; i < sze; i++) {
            Game gm_year = data[i];

            int y = gm_year.get_year();

            if (y >= year1 && y <= year2) {
                check = 1;
                cout << num << ". "; gm_year.printing_gam();
				num++;
            }
        }

        // This section checks if search has found records or not
        if (check != 0) {
            cout << "\n";
            cout << "Search done\n" << endl;
            return;
            
        } else if (check == 0) {
            cout << "\n";
            cout << "No matching records are found\n" << endl;
            return;
        }
    }


	//---------------------------------------------------------------//
	// functions below help directly with the option 3 of the menu

	// this function deletes the specified records
	void exact_del(int m, const string& inp) {
        string str = inp;
        remove_space(str); decap(str);
        cout << "\n" << endl;

		Game* new_data = new Game[cap]; string ans;
		int success = 0; int plac = 0; int size = 0; int num = 1;

        // This section searches for game names 
        if (m == 5) {
            cout << "Games for deletion with: " << inp << "\n" << endl;
            for (int i = 0; i < sze; i++) {
                Game gm_nam = data[i];

                string nam = gm_nam.get_name();
                remove_space(nam); decap(nam);

                if (nam == str) {
                    cout << num << ". "; gm_nam.printing_gam();
					success = 1; num++; continue;
                }

				new_data[plac] = gm_nam;
				size++; plac++; 
            }

			cout << "\n";
			// asks the user if they want to delete the records 
			while (true) {
				if (success == 0) break;
		
				cout << "Do you want to delete (Y/N)?";
				getline(cin, ans); cout << "\n" << endl;
				remove_space(ans);

				if (ans == "Y" || ans == "y") {
					delete[] data;
					data = new_data; sze = size;
					cout << "Deletion successful\n";
					return;
				} else if (ans == "N" || ans == "n") {
					delete[] new_data;
					cout << "Deletion stopped\n";
					return;
				}
			}

        // This section searches for game studios 
        } else if (m == 6) {
            cout << "Games for deletion with: " << str << "\n" << endl;

            for (int i = 0; i < sze; i++) {
                Game gm_stud = data[i];

                string stud = gm_stud.get_studio();
                remove_space(stud); decap(stud);

                if (stud == str) {
                    cout << num << ". "; gm_stud.printing_gam();
					success = 1; num++; continue;
                }

				new_data[plac] = gm_stud;
				size++; plac++;

            }   

			cout << "\n";
			// asks the user if they want to delete the records
			while (true) {
				if (success == 0) break;
				cout << "Do you want to delete (Y/N)?";
				getline(cin, ans); cout << "\n" << endl;
				remove_space(ans);

				if (ans == "Y" || ans == "y") {
					delete[] data;
					data = new_data; sze = size;
					cout << "Deletion successful\n";
					return;
				} else if (ans == "N" || ans == "n") {
					delete[] new_data;
					cout << "Deletion stopped\n";
					return;
				}
			}

		// This section searches for game releases
        } else if (m == 7) {
            cout << "Games for deletion released in: " << str << "\n" << endl;
			int year_find = conv(str);

            for (int i = 0; i < sze; i++) {
                Game gm_ye = data[i]; 

				int y = gm_ye.get_year();

                if (y == year_find) {
                    cout << num << ". "; gm_ye.printing_gam();
					success = 1; num++; continue;
                }

				new_data[plac] = gm_ye;
				size++; plac++; 
            }   
		
			cout << "\n";
			// asks the user if they want to delete the records
			while (true) {
				if (success == 0) break;

				cout << "Do you want to delete (Y/N)?";
				getline(cin, ans); cout << "\n" << endl;
				remove_space(ans); 

				if (ans == "Y" || ans == "y") {
					delete[] data;
					data = new_data; sze = size;
					cout << "Deletion successful\n";
					return;
				} else if (ans == "N" || ans == "n") {
					delete[] new_data;
					cout << "Deletion stopped\n";
					return;
				}
			}
		} 

        cout << "\n";
        // This section checks if search has found records or not
        if (success == 0) {
            cout << "No matching records are found\n" << endl;
			delete[] new_data;
            return;
        }
    }

	// This function deletes records with the specified name, studio and release year
	void exact_all_del(string gm_nam, string gm_stud, int gm_ye){
		decap(gm_nam); decap(gm_stud); int num = 1;

		Game* new_data = new Game[cap]; string ans;
		int success = 0; int plac = 0; int size = 0;

		cout << "Exact match: \n";
		for (int i = 0; i < sze; i++) {
			Game gam = data[i]; 

			string n = gam.get_name(); string s = gam.get_studio(); int y = gam.get_year();
			decap(n); decap(s);

			if (gm_nam == n && gm_stud == s && gm_ye == y) {
				cout << num << ". "; gam.printing_gam();
				success = 1; num++; continue;
			}

			new_data[plac] = gam;
			size++; plac++;
		}

		cout << "\n";
		// This sections asks if they want to delete the data or not
		while (true) {
			if (success == 0) break;
			
			cout << "Do you want to delete (Y/N)?";
			getline(cin, ans); cout << "\n" << endl;
			remove_space(ans);

			if (ans == "Y" || ans == "y") {
				delete[] data;
				data = new_data; sze = size;
				cout << "Deletion successful\n";
				return;
			} else if (ans == "N" || ans == "n") {
				delete[] new_data;
				cout << "Deletion stopped\n";
				return;
			}
		}

        if (success == 0) {
            cout << "No matching records found\n" << endl;
			delete[] new_data;
            return;
        }
	}

	// This searches for substrings for deletion
    void contain_del(int m, const string& inp) {
        string str = inp;
        remove_space(str); decap(str);
        cout << "\n" << endl;

		Game* new_data = new Game[cap]; string ans;
		int success = 0; int plac = 0; int size = 0; int num = 1;

        // This section searches for game names containing certain strings
        if (m == 5) {
            cout << "Games with names that contain: " << inp << "\n" << endl;
            for (int i = 0; i < sze; i++) {
                Game gm_nam = data[i];

                string nam = gm_nam.get_name();
                remove_space(nam); decap(nam);
    
                if (nam.find(str) != string::npos) {
                    cout << num << ". "; gm_nam.printing_gam();
					success = 1; num++; continue;
                }

				new_data[plac] = gm_nam;
				size++; plac++;
            }

			cout << "\n";
			// This section asks the user if they want to delete the records
			while (true) {
				if (success == 0) break;

				cout << "Do you want to delete (Y/N)?";
				getline(cin, ans); cout << "\n" << endl;
				remove_space(ans);

				if (ans == "Y" || ans == "y") {
					delete[] data;
					data = new_data; sze = size;
					cout << "Deletion successful\n";
					return;
				} else if (ans == "N" || ans == "n") {
					delete[] new_data;
					cout << "Deletion stopped\n";
					return;
				}
			}

        // This section searches for game studios containing certain strings
        } else if (m == 6) {
            cout << "Games for deletion containing: " << str << "\n" << endl;
            for (int i = 0; i < sze; i++) {
                Game gm_stud = data[i];

                string stud = gm_stud.get_studio();
                remove_space(stud); decap(stud);

				// prints out records found for deletion
                if (stud.find(str) != string::npos) {
                    cout << num << ". "; gm_stud.printing_gam();
					success = 1; num++; continue;
                }

				new_data[plac] = gm_stud;
				size++; plac++;
            }   

			cout << "\n";
			// asks the user if they want to delete the records
			while (true) {
				if (success == 0) break;

				cout << "Do you want to delete (Y/N)?";
				getline(cin, ans); cout << "\n" << endl;
				remove_space(ans);

				if (ans == "Y" || ans == "y") {
					delete[] data;
					data = new_data; sze = size;
					cout << "Deletion successful\n";
					return;
				} else if (ans == "N" || ans == "n") {
					delete[] new_data;
					cout << "Deletion stopped\n";
					return;
				}
			}
        }

		// checks if any records had been found
		if (success == 0) {
			cout << "No matching records found\n";
			delete[] new_data;
			return;
		}

    }

	// searches games between 2 specified years for deletion
    void contain_num_del(int year1, int year2) {
        cout << "Games for deletion: " << year1 << "-" << year2 << "\n" << endl;

		Game* new_data = new Game[cap]; string ans;
		int success = 0; int plac = 0; int size = 0; int num = 1;

        for (int i = 0; i < sze; i++) {
            Game gm_year = data[i]; 

            int y = gm_year.get_year();

            if (y >= year1 && y <= year2) {
                cout << num << ". "; gm_year.printing_gam();
				success = 1; num++; continue;
            }

			new_data[plac] = gm_year; 
			size++; plac++;
        }
		
		cout << "\n";
		while (true) {
			if (success == 0) break;
			
			cout << "Do you want to delete (Y/N)?";
			getline(cin, ans); cout << "\n" << endl;
			remove_space(ans);

			if (ans == "Y" || ans == "y") {
				delete[] data;
				data = new_data; sze = size;
				cout << "Deletion successful\n";
				return;
			} else if (ans == "N" || ans == "n") {
				delete[] new_data;
				cout << "Deletion stopped\n";
				return;
			}
		}

        if (success == 0) {
            cout << "No matching records found\n" << endl;
			delete[] new_data;
            return;
        }
    }

	//---------------------------------------------------------------//
	// The functions below help directly option 4 of the menu
	// https://www.softwaretestinghelp.com/merge-sort/
	// Use this link for help in mergesort
	void merging(int mode) {
		merge_sort(mode, 0, sze);
	}

	void merge_sort(int mode, int begin, int end) {
    	int mid;
    	if (begin < end){
        	// Divides the array at mid and then sorts both halves
        	mid = (begin + end)/2;
        	merge_sort(mode, begin, mid);
        	merge_sort(mode, mid + 1, end);
       	 	merge(mode, begin, end, mid);
    	}
	}

	// https://www.softwaretestinghelp.com/merge-sort/
	// Use this link for help in mergesort
	void merge(int mode, int begin, int end, int mid) {

		int size = sze + 10;
    	int i, j, k; Game new_data[size];
    	i = begin; k = begin; j = mid + 1;

		// This section merges based on the mode given by the user 
		if (mode == 1) { // This section is for sorting Game names
    		while (i <= mid && j <= end) {
				string n1 = data[i].get_name(); string n2 = data[j].get_name();
				decap(n1); decap(n2);

        		if (n1 < n2) {
            		new_data[k] = data[i]; k++; i++;
        		} else  {
            		new_data[k] = data[j]; k++; j++;
        		}	
    		}
		} else if (mode == 2) { //  This section is for sorting studio names
    		while (i <= mid && j <= end) {
				string s1 = data[i].get_studio(); string s2 = data[j].get_studio();
				decap(s1); decap(s2);

        		if (s1 < s2) {
            		new_data[k] = data[i]; k++; i++;
        		} else  {
            		new_data[k] = data[j]; k++; j++;
        		}	
    		}

		} else if (mode == 3) { // This section is for sorting game years
    		while (i <= mid && j <= end) {
        		if (data[i].get_year() < data[j].get_year()) {
            		new_data[k] = data[i]; k++; i++;
        		} else  {
            		new_data[k] = data[j]; k++; j++;
        		}	
    		}
		}

		// This section re-adds the data items to the new_data;
		// Then it puts the new_data items back to data
    	while (i <= mid) {
        	new_data[k] = data[i]; k++; i++;

    	}

    	while (j <= end) {
        	new_data[k] = data[j]; k++; j++;

    	}

		int r = begin;
    	for (i = begin; i < k; i++)  {
			if ((new_data[i].get_year() != 0) ) {
        		data[r] = new_data[i]; r++;
			}
    	}
	}
};

