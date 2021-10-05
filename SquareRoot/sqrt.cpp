/*******************************************************************************
 * Name    : sqrt.cpp
 * Author  : Zane ThummBorst
 * Version : 1.0
 * Date    : September 9th, 2020
 * Description : Computes the square root of a number
 * Pledge : I pledge my honor that I have abided by the Stevens Honor System.
 ******************************************************************************/
#include <iostream>
#include <sstream>
#include <iomanip>
#include <limits>
using namespace std;

double sqrt(double num, double epsilon){
	if(num < 0){
		return  numeric_limits<double>::quiet_NaN();
	}
	if(num == 0 || num == 1){
		return num;
	}
	double last_guess = num;
	double next_guess = (last_guess + num/last_guess) / 2;
	while(!(abs(last_guess - next_guess) <= epsilon)){
		last_guess = next_guess;
		next_guess = (last_guess + num/last_guess) / 2;
	}
	return next_guess;
}
int main(int argc, char* const argv[]) {
    double m;
    double n;
    istringstream iss;
    if (argc != 3 && argc != 2) {
        cerr << "Usage: " << argv[0] << " <value> [epsilon]"
             << endl;
        return 1;
    }
    iss.str(argv[1]);
    if ( !(iss >> m) ) {
        cerr << "Error: Value argument must be a double."
             << endl;
        return 1;
    }
    iss.clear();
    if(argc == 3){
    	iss.str(argv[2]);
    	if (!(iss >> n) || (n <= 0)) {
    		cerr << "Error: Epsilon argument must be a positive double."
    				<< endl;
    		return 1;
    	}
    }
    if( argc == 3){
    	cout << fixed << setprecision(8) << sqrt(m, n) << endl;
    }
    else{
    	cout << fixed << setprecision(8) << sqrt(m, 1e-7) << endl;
    }

}



