/*******************************************************************************
 * Name        : mtsieve.c
 * Author      : Zane ThummBorst and Faraz Pathan
 * Date        :
 * Description : Multithreaded algorithm using Sieve to calculate the number of primes in a given range.
 * Pledge      : I pledge my honor that I have abided by the Stevens Honor System.
 ******************************************************************************/
#include <errno.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <ctype.h>
#include <stdbool.h>
#include <math.h>
#include <pthread.h>
#include <sys/sysinfo.h>


typedef struct arg_struct {
    int start;
    int end;
} thread_args;

pthread_mutex_t lock;
int sum = 0;



void *sieve(void *ptr){
    thread_args* targs = (thread_args *)ptr;
    int count = 0;
    int a = targs->start;
    int b = targs->end;
    int sqrt = (int)sqrtf(b);
    bool *low = (bool *) malloc(sizeof (bool)*(sqrt + 1));

    for (int i = 0; i <= sqrt; i++){
    	low[i] = true;
    }

    for (int p = 2; p*p < sqrt; p++){
        if (low[p] == true){
            for (int j = p*p; j <= sqrt; j+=p){
                low[j] = false;
            }
        }
    }
    int len = b - a + 1;
    bool *high = (bool *) malloc(sizeof (bool)*len + 1);

    for (int i = 0; i <= len; i++){
        high[i] = true;
    }

    for (int p = 2; p < sqrt; p++){
        if (low[p]){
            int i = (int)ceil((double)a/p) * p - a;
            if (a <= p){
                i = i +  p;
            }
			for(int j = i; j < len; j =j+p ){
				high[j] = false;
			}
        }
    }

  	for (int p = 2; p < len; p++){
    	        if (high[p] == true){
    	        	int threecount = 0;
    	        	int prim = p + a;
    	        	while(prim != 0){
    	        		if(prim%10 == 3){
    	        			threecount++;
    	        			if(threecount == 2){
    	        	        	count++;
    	        			}
    	        		}
    	        		prim = prim/10;
    	        	}
    	        }
    	}

    int retval;
    if ((retval = pthread_mutex_lock(&lock)) != 0) {
        fprintf(stderr, "Warning: Cannot lock mutex. %s.\n",
        		strerror(retval));
    }
    sum += count;
    if ((retval = pthread_mutex_unlock(&lock)) != 0) {
        fprintf(stderr, "Warning: Cannot unlock mutex. %s.\n",
        		strerror(retval));
    }
    free(low);
    free(high);
    pthread_exit(NULL);
}

int main(int argc, char *argv[]){
	if( argc == 1){
	        printf("Usage: ./mtsieve -s <starting value> -e <ending value> -t <num threads>\n");
	        return EXIT_FAILURE;
		}
		int opt;
		int s_flag = 0, e_flag = 0, t_flag = 0, lower = 0, upper = 0, number = 0;
		long long long_long_i;
		while((opt = getopt(argc,argv, ":s:e:t:")) != -1){
			switch(opt){

			case 's':
				s_flag++;
				lower = atoi(optarg);
				if(lower==0)
				{
					fprintf(stderr, "Error: Invalid input '%s' received for parameter '-%c'.\n", optarg, 's');
					return EXIT_FAILURE;
				}
				if (sscanf(optarg, "%lld", &long_long_i) == 1)
				{
					int value = (int)long_long_i;
					if (long_long_i != (long long) value)
					{
						fprintf(stderr, "Error: Integer overflow with '-%c'.\n",'s');
						return EXIT_FAILURE;
					}
				}
				break;
			case 'e':
				e_flag++;
				upper = atoi(optarg);
				if(upper==0)
				{
					fprintf(stderr, "Error: Invalid input '%s' received for parameter '-%c'.\n", optarg, 'e');
					return EXIT_FAILURE;
				}
				if (sscanf(optarg, "%lld", &long_long_i) == 1)
				{
					int value = (int)long_long_i;
					if (long_long_i != (long long) value)
					{
						fprintf(stderr, "Error: Integer overflow with '-%c'.\n", 'e');
						return EXIT_FAILURE;
					}
				}
				break;
			case 't':
				t_flag++;
				number = atoi(optarg);
				if(number==0)
				{
					fprintf(stderr, "Error: Invalid input '%s' received for parameter '-%c'.\n", optarg, 't');
					return EXIT_FAILURE;
				}
				if (sscanf(optarg, "%lld", &long_long_i) == 1)
				{
					int value = (int)long_long_i;
					if (long_long_i != (long long) value)
					{
						fprintf(stderr, "Error: Integer overflow with '-%c'.\n",'c');
						return EXIT_FAILURE;
					}
				}
				break;
		case '?':
			//FARAZ idk why this part is not working as intended; if you forget to put in an arguement for SET
			// it doesn't catch it here like it says it will, maybe wanna bring it up to a TA Tomorrow, but if you can't figure it out
			// we can leave for later.
		 if (optopt == 'e' || optopt == 's' || optopt == 't') {
			 fprintf(stderr, "Error: Option -%c requires an argument.\n", optopt);
		 } else if (isprint(optopt)) {
			 fprintf(stderr, "Error: Unknown option '-%c'.\n", optopt);
		 } else {
			 fprintf(stderr, "Error: Unknown option character '\\x%x'.\n", optopt);
		 }
		 return EXIT_FAILURE;
		case ':':
			if (optopt == 'e' || optopt == 's' || optopt == 't') {
				fprintf(stderr, "Error: Option -%c requires an argument.\n", optopt);
			} else if (isprint(optopt)) {
				fprintf(stderr, "Error: Unknown option '-%c'.\n", optopt);
			} else {
				fprintf(stderr, "Error: Unknown option character '\\x%x'.\n", optopt);
			}
			return EXIT_FAILURE;
		}
	}
	if(argc > 4){
		fprintf(stderr,"Error: Non-option argument '%s' supplied.\n", argv[4]);
		return EXIT_FAILURE;
	}
	else if(s_flag < 1){
		fprintf(stderr,"Error: Required argument <starting value> is missing.\n");
		return EXIT_FAILURE;
	}
	else if(lower < 2){
		fprintf(stderr,"Error: Starting value must be >= 2.\n");
		return EXIT_FAILURE;
	}
	else if(e_flag <1){
		fprintf(stderr,"Error: Required argument <ending value> is missing.\n");
		return EXIT_FAILURE;
	}
	else if(lower < 2){
		fprintf(stderr,"Error: Ending value must be >= 2.\n");
		return EXIT_FAILURE;
	}
	else if(upper < lower){
		fprintf(stderr,"Error: Ending value must be >= starting value.\n");
		return EXIT_FAILURE;
	}
	else if(t_flag != 1){
		fprintf(stderr,"Error: Required argument <num threads> is missing.\n");
		return EXIT_FAILURE;
	}
	else if(number < 1){
		fprintf(stderr,"Error: Number of threads cannot be less than 1.\n");
		return EXIT_FAILURE;
	}
	else if (number > get_nprocs()){
        printf("Error: Number of threads cannot exceed twice the number of processors(%d).\n", get_nprocs());
        return EXIT_FAILURE;
    }
	int retval;
    if ((retval = pthread_mutex_init(&lock, NULL)) != 0) {
        fprintf(stderr, "Error: Cannot create mutex. %s.\n", strerror(retval));
        return EXIT_FAILURE;
    }
    pthread_t threads[number];
    thread_args targs[number];
		//FOR THE REMAINDER CALCULATE HOW MUCH IS LEFT OVER AND ADD IT ON TO SUBSEQUENT ONES BARRED FROM THE FIRST.
    	if(number > upper - lower + 1){
    		number = upper - lower + 1;
    	}
    	if(lower == upper){
    		number = 1;
    	}
		int segments = (upper - lower)/number;
		int remainder = (upper -lower)%number;
		int start = lower + segments;

		for(int i = 0; i < number; i++){
			if(i == 0){
				targs[0].start = lower;
				targs[0].end = lower + segments;
			}
			else{
				targs[i].start = start + 1;
				if(remainder > 0){
					targs[i].end = start + segments + 1;
					start = start + segments + 1;
					remainder--;
				}
				else{
					targs[i].end = start + segments;
					start = start + segments;
				}
			}
	    	if ((retval = pthread_create(&threads[i], NULL, sieve,(void *)(&targs[i]))) != 0) {
	    	    fprintf(stderr, "Error: Cannot create thread %d. %s.\n",
	    	    i + 1, strerror(retval));
	    	    break;
	    	}
		}
		printf("Finding all prime numbers between %d and %d.\n", lower, upper);
		printf("%d segments:\n", number);
		for(int i = 0; i < number; i++){
			printf("   [%d, %d]\n", targs[i].start, targs[i].end);
		}
	    for (int i = 0; i < number; i++) {
	        if (pthread_join(threads[i], NULL) != 0) {
	            fprintf(stderr, "Warning: Thread %d did not join properly.\n", i + 1);
	        }
	    }
	    if ((retval = pthread_mutex_destroy(&lock)) != 0) {
	        fprintf(stderr, "Error: Cannot destroy mutex. %s.\n", strerror(retval));
	        return EXIT_FAILURE;
	    }
		printf("Total primes between %d and %d with two or more '3' digits: %d\n",lower,upper, sum);



	return EXIT_SUCCESS;
}



