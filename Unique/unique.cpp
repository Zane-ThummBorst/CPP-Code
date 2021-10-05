/*******************************************************************************
 * Name        : unique.cpp
 * Author      : Zane ThummBorst
 * Date        : 9/20/20
 * Description : Determining uniqueness of chars with int as bit vector.
 * Pledge      : I pledge my honor that I have abided by the Stevens Honor System.
 ******************************************************************************/
#include <iostream>
#include <cctype>

using namespace std;

bool is_all_lowercase(const string &s) {
    // TODO: returns true if all characters in string are lowercase
    // letters in the English alphabet; false otherwise.
	char n;
	int l = s.length();
	for(int i = 0; i < l; i++){
		n = s[i];
		if( !(islower(n)) ){
			return false;
		}
	}
	return true;
}

bool all_unique_letters(const string &s) {
    // TODO: returns true if all letters in string are unique, that is
    // no duplicates are found; false otherwise.
    // You may use only a single int for storage and work with bitwise
    // and bitshifting operators.
    // No credit will be given for other solutions.
	unsigned int vector = 0;
	int l = s.length();
	for(int i = 0; i < l; i++){
		if(vector & (1 << (s[i] - 'a'))){
			return false;
		}
		else{
			vector = vector | (1 << (s[i] - 'a'));
		}
	}
	return true;
}

int main(int argc, char * const argv[]) {
    // TODO: reads and parses command line arguments.
    // Calls other functions to produce correct output.
	string s;
	if(argc == 1 || argc > 2){
		cerr << "Usage: ./unique <string>" << endl;
		return 1;
	}

	s = argv[1];
	bool isLowerCase = is_all_lowercase(s);
	if(isLowerCase != true){
		cerr << "Error: String must contain only lowercase letters." << endl;
		return 1;
	}

	bool uniqueL = all_unique_letters(s);
	if(uniqueL != true){
		cout << "Duplicate letters found." << endl;
	}
	else{
		cout << "All letters are unique." << endl;
	}
	return 0;

}
