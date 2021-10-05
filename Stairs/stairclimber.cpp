/*******************************************************************************
 * Name        : stairclimber.cpp
 * Author      : Zane ThummBorst
 * Date        : September 27th, 2020
 * Description : Lists the number of ways to climb n stairs.
 * Pledge      : I pledge my honor that I have abided by the Stevens Honor System.
 ******************************************************************************/
#include <iostream>
#include <vector>
#include <algorithm>
#include <sstream>
#include <iomanip>

using namespace std;
vector< vector<int> > get_ways(int num_stairs) {
    // TODO: Return a vector of vectors of ints representing
    // the different combinations of ways to climb num_stairs
    // stairs, moving up either 1, 2, or 3 stairs at a time.
	vector<vector<int>> m = {};
	vector<vector<int>> n = {};
	if (num_stairs <= 0){
		return {{}};
	}
	const int four = 4;
	const int var = num_stairs +1;
	for(int i = 1; i < min(four, var); i++){
		m = get_ways(num_stairs - i);
		for(int j = 0; j < (int) m.size(); j++){
			m[j].push_back({i});
			n.push_back(m.at(j));
		}
	}

	return n;
}

void display_ways(const vector< vector<int> > &ways) {
    // TODO: Display the ways to climb stairs by iterating over
    // the vector of vectors and printing each combination.
	int n = 0;
	int num = ways.size();
	while( num >= 1){
		n++;
		num = num/10;
	}
	for(int i = 0; i < (int) ways.size(); i++){
		cout << setw(n) << i+1;
		cout <<". [";
		for(int j = ways[i].size() - 1; j >= 0; j--){
			cout << ways.at(i).at(j);
			if( j != 0){
				cout << ", ";
			}
		}
		cout << "]";
		if(i + 1 != num){
			cout << endl;
		}
	}
}

int main(int argc, char * const argv[]) {
	int m;
	if(argc != 2){
		cerr << "Usage: ./stairclimber <number of stairs>" << endl;
		return 1;
	}
	istringstream iss;
	iss.str(argv[1]);
    if ( !(iss >> m)) {
        cerr << "Error: Number of stairs must be a positive integer."<< endl;
        return 1;
    }
    if(m <= 0){
        cerr << "Error: Number of stairs must be a positive integer."<< endl;
         return 1;
    }
	vector<vector<int>> w = get_ways(m);
	cout << w.size();
	if(m == 1){
		cout << " way to climb " << m <<" stair." << endl;
	}else{
		cout << " ways to climb " << m <<" stairs." << endl;
	}
	display_ways(w);
	return 0;
}
