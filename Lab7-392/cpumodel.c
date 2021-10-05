/*******************************************************************************
 * Name        : cpumodel.c
 * Author      : Zane ThummBorst
 * Date        :
 * Description :
 * Pledge      : I pledge my honor that I have abided by the Stevens Honor System.
 ******************************************************************************/

#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

bool starts_with(const char *str, const char *prefix) {
    /* TODO:
       Return true if the string starts with prefix, false otherwise.
       Note that prefix might be longer than the string itself.
     */
    size_t lenp = strlen(prefix);
    size_t lens = strlen(str);
    return lens < lenp ? false : memcmp(prefix, str, lenp) == 0;

}

int main() {
    /* TODO:
       Open "cat /proc/cpuinfo" for reading, redirecting errors to /dev/null.
       If it fails, print the string "Error: popen() failed. %s.\n", where
       %s is strerror(errno) and return EXIT_FAILURE.
     */
	FILE *fp;
	if((fp = popen("pd 2>/dev/null", "r")) == NULL){
		fprintf(stderr,"Error: popen() failed. %s.\n", strerror(errno));
		return EXIT_FAILURE;
	}
	else{
		fp = popen("cat /proc/cpuinfo", "r");
	}

    /* TODO:
       Allocate an array of 256 characters on the stack.
       Use fgets to read line by line.
       If the line begins with "model name", print everything that comes after
       ": ".
       For example, with the line:
       model name      : AMD Ryzen 9 3900X 12-Core Processor
       print
       AMD Ryzen 9 3900X 12-Core Processor
       including the new line character.
       After you've printed it once, break the loop.
     */
	char a[256];
	while(fgets(a,sizeof(a),fp)){
		if(starts_with(a, "model name")){
			char *eoln = strchr(a, ':');
			if (eoln != NULL) {
			    eoln += 2;
			    printf("%s",eoln);
			}
			break;
		}
	}

    /* TODO:
       Close the file descriptor and check the status.
       If closing the descriptor fails, print the string
       "Error: pclose() failed. %s.\n", where %s is strerror(errno) and return
       EXIT_FAILURE.
     */
    int status = pclose(fp);
    if (status == -1) {
    	fprintf(stderr, "Error: pclose() failed. %s.\n", strerror(errno));
    	exit(EXIT_FAILURE);
    }
    return !(WIFEXITED(status) && WEXITSTATUS(status) == EXIT_SUCCESS);
}
