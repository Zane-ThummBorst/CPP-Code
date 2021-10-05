/*******************************************************************************
 * Name    : waterjugpuzzle.ccp
 * Author  : Zane ThummBorst & Faraz Pathan
 * Version : 1.0
 * Date    : October 15th 2020
 * Description : Pour in, pour out
 * Pledge : I pledge my honor that I have abided by the Stevens Honor System.
 ******************************************************************************/
#include <iostream>
#include <sstream>
#include <vector>
#include <queue>
using namespace std;

// Struct to represent state of water in the jugs.
struct State {
    int a, b, c;
    string directions;
    State *parent;



    State(int _a, int _b, int _c, string _directions) :
        a{_a}, b{_b}, c{_c}, directions{_directions}, parent{nullptr} { }
     // tried altering parent without extra constructor, but this was easier and we couldn't modify parent without makefile throwing a fit.
     State(int _a, int _b, int _c, string _directions,State *_parent) :
         a{_a}, b{_b}, c{_c}, directions{_directions}, parent{_parent} { }

    // String representation of state in tuple form.
    string to_string() {
        ostringstream oss;
        oss << "(" << a << ", " << b << ", " << c << ")";
        return oss.str();
    }
};

// saves plenty of lines in the water jug function. without it there would be one per pour check.
int change_helper(int max, int x){
	if(x >= max){
		return max;
	}
	else{
		return x;
	}
}

// tried doing void first but doing recursion with it was a little bit more difficult, so we opted on just returning a string.
// that way when we return we can just concatenate all of our steps into one string and send it back.

string print_outs(State n){
//	if(n.parent != NULL){
//		cout << "HERE" <<endl;
//		return print_outs(*n.parent);
//		cout << n.directions << " " << n.to_string() << endl;
//	}
//	cout << n.directions << " " << n.to_string() << endl;

	if(n.parent != NULL){
		return print_outs(*n.parent) + n.directions + " " + n.to_string() + "\n";
	}
	return n.directions + " " + n.to_string() + "\n";
}

//void delete_parents(State n){
//	if(n.parent != NULL){
//		return delete_parents(*n.parent);
//	}
//	delete *n;
//}


// why does there have to be a difference between gallons and gallon? could have made a work around maybe to reduce the amount of cout lines,
// but just distinguishing between a single gallon pour was easier. once we find our goal, we delete everything so valgrind doesn't yell.
// we thought that once we exited the while loop we would also have to delete the queue and 2d vector, but valgrind never said there was any leakage.
// so we just left it as is.
//UPDATE: we learned the hard way why we keep track of the visited states as oppose to make a 2d vector of ints. even if the thing that held the pointer
// is deleted (in this case ron) we need to be able to go back and delete all of the pointers somehow, which is why for every pointer we make, we must have a
// way to go back and delete it somehow. valgrind now does not yell for all three cases.

// also learned the hard way why we need to keep a
string waterjug(int a, int b, int c, int a2, int b2, int c2){
	State *s = new State (0, 0, c, "Initial state.");
//	if(s->a == a2 && s->b == b2 && s->c == c2){
//		return s;
//	}
	queue<State> *q = new queue<State>;
	q->push(*s);
	State *ron;
	vector<vector<State *>> visited(a+1 , vector<State *> ((b+1),NULL));
	//vector<vector<int>> visited(a+1 , vector<int> ((b+1),0));
	while(q->empty() == false){


		ron = new State(q->front());
		q->pop();
		if(ron->a == a2 && ron->b == b2 && ron->c == c2){
			cout << print_outs(*ron);
			delete ron;
			for(int i = 0; i <= a; i++){
				for(int j = 0; j<=b; j++){
					ron = visited[i][j];
					delete ron;
				}
			}
			delete q;
			delete s;
			return "";
		}


		else if(visited[ron->a][ron->b] == NULL){
			visited[ron->a][ron->b] = ron;
			int change;
			int max_pour;

			if(ron->a != a){
				max_pour = a - ron->a;
				change = 0;
				if(ron->c != 0){
					change = change_helper(max_pour, ron->c);
				}
				if(change == 1){
					q->push(State(ron->a + change, ron->b, ron->c - change, "Pour " + to_string(change) + " gallon from C to A.", ron));
				}
				else{
					q->push(State(ron->a + change, ron->b, ron->c - change, "Pour " + to_string(change)+ " gallons from C to A.", ron));
				}
				change = 0;
				if(ron->b != 0){
					change = change_helper(max_pour, ron->b);
				}
				if(change == 1){
					q->push(State(ron->a + change, ron->b - change, ron->c, "Pour " + to_string(change) + " gallon from B to A.", ron));
				}
				else{
					q->push(State(ron->a + change, ron->b - change, ron->c, "Pour " + to_string(change) + " gallons from B to A.", ron));
				}
			}
			if(ron->b != b){
				max_pour = b - ron->b;
				change = 0;
				if(ron->c != 0){
					change = change_helper(max_pour, ron->c);
				}
				if(change == 1){
					q->push(State(ron->a, ron->b + change, ron->c - change, "Pour " + to_string(change) + " gallon from C to B.", ron));
				}
				else{
					q->push(State(ron->a, ron->b + change, ron->c - change, "Pour " + to_string(change) + " gallons from C to B.", ron));
				}
				change = 0;
				if(ron->a != 0){
					change = change_helper(max_pour, ron->a);
				}
				if(change == 1){
					q->push(State(ron->a - change, ron->b + change, ron->c, "Pour " + to_string(change) + " gallon from A to B.", ron));
				}
				else{
					q->push(State(ron->a - change, ron->b + change, ron->c, "Pour " + to_string(change) + " gallons from A to B.", ron));
				}
			}
			if(ron->c != c){
				max_pour = c - ron->c;
				change = 0;
				if(ron->b != 0){
					change = change_helper(max_pour, ron->b);
				}
				if(change == 1){
					q->push(State(ron->a, ron->b - change, ron->c + change, "Pour " + to_string(change) + " gallon from B to C.", ron));
				}
				else{
					q->push(State(ron->a, ron->b - change, ron->c + change, "Pour " + to_string(change) + " gallons from B to C.", ron));
				}
				change = 0;
				if(ron->a != 0){
					change = change_helper(max_pour, ron->a);
				}
				if(change == 1){
					q->push(State(ron->a - change, ron->b, ron->c + change, "Pour " + to_string(change) + " gallon from A to C.", ron));
				}
				else{
					q->push(State(ron->a - change, ron->b, ron->c + change, "Pour " + to_string(change) + " gallons from A to C.", ron));
				}
			}
		}


		else{
			delete ron;
		}
	}
	for(int i = 0; i <= a; i++){
		for(int j = 0; j<=b; j++){
			ron = visited[i][j];
			delete ron;
		}
	}
	delete q;
	delete s;
	return "No solution.";
}
int main(int argc, char* const argv[]) {

	if( argc != 7 ){
		cerr << "Usage: ./waterjugpuzzle <cap A> <cap B> <cap C> <goal A> <goal B> <goal C>";
		return 1;
	}
	istringstream iss;
	string jugs = "ABCABC";
	int sum = 0;
    int nums[7];
    for(int i = 1; i < argc; i++){
    	iss.str(argv[i]);
    	if ( !(iss >> nums[i]) || nums[i] < 0) {
    		if( i <= 3){
    			cerr << "Error: Invalid capacity " << '\'' << argv[i] << '\'' <<" for jug " << jugs[i-1] << "." << endl;
    		}
    		else{
    			cerr << "Error: Invalid goal " << '\'' << argv[i] << '\'' <<" for jug " << jugs[i-1] << "." << endl;
    		}
    		return 1;
    	}
    	if(i == 3 && stoi(argv[i]) == 0){
    		cerr << "Error: Invalid capacity " << '\'' << argv[i] << '\'' <<" for jug " << jugs[i-1] << "." << endl;
    		return 1;
    	}
    	iss.clear();
    }

    for(int i = 4; i <= 6; i++){
    	sum = sum + nums[i];
    }

    int num = stoi(argv[3]);
    if(nums[1]< nums[4]){
    	cerr << "Error: Goal cannot exceed capacity of jug A." << endl;
    	return 1;
    }
    if(nums[2] < nums[5]){
    	cerr << "Error: Goal cannot exceed capacity of jug B." << endl;
    	return 1;
    }
    if(nums[3] < nums[6]){
    	cerr << "Error: Goal cannot exceed capacity of jug C." << endl;
    	return 1;
    }
    if(num != sum){
    	cerr << "Error: Total gallons in goal state must be equal to the capacity of jug C." << endl;
    	return 1;
    }

    cout << waterjug(nums[1], nums[2], nums[3],nums[4], nums[5], nums[6]);
    return 0;
}




