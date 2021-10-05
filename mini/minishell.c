/*******************************************************************************
 * Name        : minishell.c
 * Author      : Zane ThummBorst and Faraz Pathan
 * Date        :
 * Description : robust minishell that handlse SIGINT and SIGTERM, implements exit and CD command, and forkexecs anything else.
 * Pledge      : I pledge my honor that I have abided by the Stevens Honor System.
 ******************************************************************************/
#include <errno.h>
#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <linux/limits.h>
#include <pwd.h>
#include <wait.h>
#include <setjmp.h>

#define BRIGHTBLUE "\x1b[34;1m"
#define DEFAULT    "\x1b[0m"
//jmp_buf jump;
sigjmp_buf jump;

int all_spaces(char* s){
	for(int i = 0; i< sizeof(s); i++){
		if(s[i] == '\0'){
			return 1;
		}
		else if(s[i] != ' '){
			return 0;
		}
	}
	return 1;
}
int str_cmp(const void *a, const void *b) {
	return strcmp(*(const char **)a, *(const char **)b);
}

volatile sig_atomic_t signal_val = 0;

void catch_signal(int sig) {
		signal_val = sig;
		if(signal_val == SIGTERM){
			signal_val = 0;
		}
		else{
			printf("\n");
		}
        siglongjmp(jump, 1);
}

int main(){
	struct passwd *p;
	if((p = getpwuid(getuid())) == NULL){
		fprintf(stderr,"Error: Cannot get passwd entry. %s.\n", strerror(errno));
	}
	char dir[PATH_MAX];
	char term[128];
	const char *homedir = p->pw_dir;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	struct sigaction action;
	memset(&action, 0, sizeof(struct sigaction));
	action.sa_handler = catch_signal;
	//action.sa_flags = SA_RESTART; /* Restart syscalls if possible */
	  if (sigaction(SIGINT, &action, NULL) == -1){
		  fprintf(stderr,"Error: Cannot register signal handler. %s.\n" , strerror(errno));
		  return EXIT_FAILURE;
	  }
	  if (sigaction(SIGTERM, &action, NULL) == -1){
		  fprintf(stderr,"Error: Cannot register signal handler. %s.\n" , strerror(errno));
		  return EXIT_FAILURE;
	  }
	  sigsetjmp(jump, 1);

/////////////////////////////////////GET CURRENT DIRECTORY AND GET INPUT////////////////////////////////////////////////////////////////////////////////////
	while(true){
	   if (getcwd(dir, sizeof(dir)) != NULL) {
	       printf("[%s%s",BRIGHTBLUE, dir);
	       printf("%s]$ ", DEFAULT);
	   } else {
		   fprintf(stderr,"Error: Cannot get current working directory. %s.\n", strerror(errno));
		   return EXIT_FAILURE;
	   }

       fflush(stdout);
	   char command[PATH_MAX];
	   //command[0] = '\0';
//	   if(fgets(command, PATH_MAX, stdin) == NULL){
//		   fprintf(stderr,"Error: Failed to read from stdin. %s.\n" , strerror(errno));
//	   }
       ssize_t bytes = read(STDIN_FILENO, command, PATH_MAX - 1);
       if(bytes == -1){
    	   fprintf(stderr,"Error: Failed to read from stdin. %s.\n", strerror(errno));
    	   return EXIT_FAILURE;
       }
       if (bytes > 0) {
           command[bytes - 1] = '\0';
       }
//////////////////////////////////////////////////BREAK DOWN INPUT INTO CHAR**//////////////////////////////////////////////////////////////////////////
	   if(strcmp(command,"") == 0 || all_spaces(command) == 1){
		   //printf("\n");
	   }
	   else{
		//command[strcspn(command, "\n")] = 0;
		char *arg[PATH_MAX];
		memset(arg, 0, sizeof(arg));
		char *token;
		token = strtok(command," ");
		strcpy(term, token);
		int count = 0;
		while(token != NULL) {
		      arg[count] = token;
		      count++;
		      token = strtok(NULL, " ");
		      if(token == NULL){
		    	  break;
		      }
		   }
/////////////////////////////////////////////////EXIT AND CD///////////////////////////////////////////////////////////////////////////////
		if(strcmp(command,"exit") == 0){
			   break;
		   }
		else if(strcmp(term,"cd") == 0){
			if(count > 2){
				fprintf(stderr,"Error: Too many arguments to cd.\n");
			}
			else if(count == 1 || strcmp(arg[1], "~") == 0){
				if(chdir(homedir) != 0){
					fprintf(stderr,"Error: Cannot change directory to '%s'. %s.\n", homedir,strerror(errno));
				}
			}
			else{
				if(chdir(arg[1]) != 0){
					fprintf(stderr,"Error: Cannot change directory to '%s'. %s.\n", arg[1],strerror(errno));
				}
			}
		}
////////////////////////////////////////////////FORK AND EXEC///////////////////////////////////////////////////////////////////////////////////
		else{
			  	pid_t pid;
			  	if ((pid = fork()) < 0)
			  	{
			  		fprintf(stderr, "Error: fork() failed. %s.\n", strerror(errno));
			  		return EXIT_FAILURE;
			  	} else if (pid > 0){
			  	int status;
//			  	do {
//			  		// Wait for the child to complete and get the status of how it
//			  		 // terminated.
			  		 pid_t w = waitpid(pid, &status, WUNTRACED | WCONTINUED);
			  		 if (w == -1){
			  			fprintf(stderr, "Error: wait() failed. %s.\n", strerror(errno));
			  			return EXIT_FAILURE;
			  		 }
//			  		if (WIFEXITED(status)){
//			  			continue;
//			  		}
//			  		else if (WIFSIGNALED(status)){
//			  			continue;
//			  		}
//			  		else if (WIFSTOPPED(status)){
//			  			continue;
//			  		}
//			  		else if (WIFCONTINUED(status)){
//			  			continue;
//			  		}
//			  	} while (!WIFEXITED(status) && !WIFSIGNALED(status));
			  }
			  else
			  {
					if(execvp(arg[0],arg)==-1){
						fprintf(stderr, "Error: exec() failed. %s.\n", strerror(errno));
						 return EXIT_FAILURE;
					}
			  }
		}
	}
	}
	   return EXIT_SUCCESS;
}



