/*******************************************************************************
 * Name        : spfind.c
 * Author      : Zane ThummBorst and Faraz Pathan
 * Date        :
 * Description : finds matching file permissions with forks, and sorts them.
 * Pledge      : I pledge my honor that I have abided by the Stevens Honor System.
 ******************************************************************************/
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>
int matches_per_buf(char* buf, int n){
	int t = 0;
    for (int i = 0; i < n; i++){
    	if(buf[i] == '\0'){
    		return t;
    	}
    	if(buf[i]=='\n'){
        	t++;
        }
    }
    return t;
}
int main(int argc, char *argv[]) {

    int pipe_pf2s[2], pipe_s2p[2];
    pipe(pipe_pf2s);
    pipe(pipe_s2p);
    //int exit_status = 0;
    pid_t pid[2];
    if ((pid[0] = fork()) == 0) {
        // pfind
        close(pipe_pf2s[0]);
        dup2(pipe_pf2s[1], STDOUT_FILENO);

        // Close all unrelated file descriptors.
        close(pipe_s2p[0]);
        close(pipe_s2p[1]);
        int pfind_exit = (execv("pfind",argv));
        if(pfind_exit == -1)
        {
        	fprintf(stderr,"Error: pfind failed.\n");
        	return EXIT_FAILURE;
        }
/*
 * This won't work becuase execs only return if there was an error, not the actual exit
 * type of the program i.e. we dont get back EXIT_FAILURE
 */

//        else if(pfind_exit == 1){
//        	exit_status = 1;
//        }
    }
    else if(pid[0]<0)
    {
    	fprintf(stderr, "Error: fork() failed. %s.\n", strerror(errno));
    	return EXIT_FAILURE;
    }

    if ((pid[1] = fork()) == 0) {
        // sort
        close(pipe_pf2s[1]);
        dup2(pipe_pf2s[0], STDIN_FILENO);
        close(pipe_s2p[0]);
        dup2(pipe_s2p[1], STDOUT_FILENO);
        if ((execlp("sort", "sort", NULL))== -1)
        {
        	fprintf(stderr,"Error: sort failed.\n");
        	return EXIT_FAILURE;
        }
    }
    else if(pid[1]<0)
    {
    	fprintf(stderr, "Error: fork() failed. %s.\n", strerror(errno));
    	return EXIT_FAILURE;
    }
    close(pipe_s2p[1]);
    dup2(pipe_s2p[0], STDIN_FILENO);
    // Close all unrelated file descriptors.
    close(pipe_pf2s[0]);
    close(pipe_pf2s[1]);
    int status;
    int n = 0;
     do 
     {            
     	// Wait for the child to complete and get the status of how it            
     	// terminated.            
     	pid_t w = waitpid(pid[n], &status, WUNTRACED | WCONTINUED);
     	if (w == -1) 
     	{                
     		// waitpid failed.                
     		perror("waitpid()");                
     		exit(EXIT_FAILURE); 
     	}           
     	n++;
     }while (!WIFEXITED(status) && !WIFSIGNALED(status));
     if (WEXITSTATUS(status) == 1 && status != 0) {
    	 //Unsure why this works, we thought pfind would EXIT_SUCSESS on no input with help but it prints nothing and is in here.
    	 if(argc == 1){
    		 printf("Usage: ./pfind -d <directory> -p <permissions string> [-h]\n");
    	 }
         return EXIT_FAILURE;
     }
     int m = 0;
     char buffer[4096];
        while (1) {
            ssize_t count = read(STDIN_FILENO, buffer, sizeof(buffer));
            if (count == -1) {
                if (errno == EINTR) {
                    continue;
                } else {
                    perror("read()");
                    exit(EXIT_FAILURE);
                }
            } else if (count == 0) {
                break;
            } else {
                write(STDOUT_FILENO, buffer, count);
                //keeps track of matches per buffer. every \n will contribute to the total
                //if we have less than what could be stored in the buffer, we end with the delimitter \0
                m += matches_per_buf(buffer, sizeof(buffer));
            }
        }
        printf("Total matches: %d\n", m);
        close(pipe_pf2s[0]);


    return EXIT_SUCCESS;
}
