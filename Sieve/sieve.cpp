/*******************************************************************************
 * Name        : sieve.cpp
 * Author      : Zane ThummBorst
 * Date        : 9/17/20
 * Description : Sieve of Eratosthenes
 * Pledge      : I pledge my honor that I have abided by the Stevens Honor System.
 ******************************************************************************/
#include <cmath>
#include <iomanip>
#include <iostream>
#include <sstream>

using namespace std;

class PrimesSieve {
public:
    PrimesSieve(int limit);

    ~PrimesSieve() {
        delete [] is_prime_;
    }

    int num_primes() const {
        return num_primes_;
    }

    void display_primes() const;

private:
    // Instance variables
    bool * const is_prime_;
    const int limit_;
    int num_primes_, max_prime_;

    // Method declarations
    int count_num_primes() const;
    void sieve();
    static int num_digits(int num);
};

PrimesSieve::PrimesSieve(int limit) :
        is_prime_{new bool[limit + 1]}, limit_{limit} {
    sieve();
}

        /*
         * Display primes has to keep track of a few things. First, the number of primes one line can have.
         * Second, the width of the max prime, so that it can determine the space between a multi line display.
         *  also it needs to keep track of when we switch lines and the specific spacing need for when the output does switch.
         * There is also a special case where we have a max prime of two digits and an output of one line that needs to be specified.
         */
void PrimesSieve::display_primes() const {

	const int max_prime_width = num_digits(max_prime_);
	int primes_per_row = 80 / (max_prime_width + 1);
	int j = 0;
	if(max_prime_width <=2){
		cout << endl;
		for(int i = 0; i <= limit_; i++){
			if (is_prime_[i] == true){
				if(i == max_prime_){
					cout << i;
				}else{
					cout << i << " ";
				}
			}
		}
	}
	else{

		for(int i = 0; i <= limit_; i++){
			if (is_prime_[i] == true){
				if(j%primes_per_row == 0){
					cout << endl;
					cout << setw(max_prime_width) << i;
				}

				else{
					cout << setw(max_prime_width+1) << i;
				}
				j++;
			}
		}
	}
}
/*
 * startingr from the first possible prime, this segment just tallies up all known primes( those labeled true) in is_prime_.
 */
int PrimesSieve::count_num_primes() const {
    // TODO: write code to count the number of primes found
	int count = 0;
	for (int i = 2; i <= limit_; i++){
		if(is_prime_[i] == true){
			count++;
		}
	}
    return count;
}
/*
 * this is our algorithm for calculating primes using Sieve method. the code starts by setting all possible primes starting from 2 to positive.
 * then we run through the function we learned in class to change any non primes to false in our boolean array. also of note, we can calculate the max
 * number of primes and total number of primes in this function and hold there values.
 */
void PrimesSieve::sieve() {
    // TODO: write sieve algorithm

	for(int i= 2; i <= limit_; i++){
		is_prime_[i] = true;
	}

	for(int i=2; i <= sqrt(limit_) ; i++){
		if(is_prime_[i]==true)
		{
			for(int j= i*i;j <= limit_; j+=i)
			{
				is_prime_[j]=false;
			}
		}
	}
	for(int i= 0; i <= limit_;i++){
			if(is_prime_[i] == true)
			{
				max_prime_=i;
			}
	}
	num_primes_=count_num_primes();

}
 /*
  * in order to find out the number of digits we can divide by 10 repeatedly and keep a tally until we hit 0, then we return our tally;
  */
int PrimesSieve::num_digits(int num) {
    // TODO: write code to determine how many digits are in an integer
    // Hint: No strings are needed. Keep dividing by 10.
	int n = 0;
	while( num >= 1){
		n++;
		num = num/10;
	}
    return n;
}

int main() {
    cout << "**************************** " <<  "Sieve of Eratosthenes" <<
            " ****************************" << endl;
    cout << "Search for primes up to: ";
    string limit_str;
    cin >> limit_str;
    int limit;
    //cout << "hey";
    // Use stringstream for conversion. Don't forget to #include <sstream>
    istringstream iss(limit_str);

    // Check for error.
    if ( !(iss >> limit) ) {
        cerr << "Error: Input is not an integer." << endl;
        return 1;
    }
    if (limit < 2) {
        cerr << "Error: Input must be an integer >= 2." << endl;
        return 1;
    }

    // TODO: write code that uses your class to produce the desired output.
   PrimesSieve n = PrimesSieve(limit);
   cout << endl << "Number of primes found: " << n.num_primes() << endl;
   cout << "Primes up to " << limit_str << ":";
   n.display_primes();
    return 0;
}
