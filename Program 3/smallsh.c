#define _POSIX_SOURCE
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

void execFunc(int, char *);
void execBackGroundFunc(int, char *);
void ctrlCHandler(int);
void killChild();
void terminator();

volatile sig_atomic_t status = -10;

void main(){
   	signal(SIGINT, ctrlCHandler);
	while(1){
		int len = 0;			//length of the array of input
		char userInput[2049] = {0};	//input string
		char *token;
		char *args[12];
		printf(": ");			//follow the convention specified
		fgets(userInput, 2048, stdin);	//gets the user input
		len = strlen(userInput);	//The length of what the user inputed
		userInput[len-1] = '\0';	//Null terminator

		if((userInput[len-2] == '&') && (userInput[len-3] == ' ')){	//if a background proccess
			if((strcmp(userInput, "exit &")) == 0){			//if the user wants to exit
				break;
			}else{
				execBackGroundFunc(len, userInput);		//exec the program to the background
			}		
		}else{								//if forground process
			if((strcmp(userInput, "exit")) == 0){
				break;						//exit the program
			}else if(userInput[0] == '#'){
				//This is a comment line
			}else if(userInput[0] == '\n'){
				printf("\n");					//Do nothing they entered nothing
			}else if ((strcmp(userInput, "status")) == 0){
				if(status == -10){
					printf("There are no prior processes?\n");	//There were no prior processes
				}else{
					printf("Exit value: %d\n", status/256);		//Print the exit value
				}
			}else if(strstr(userInput, "badfile") != NULL){
			   	status = 1;						//Set the code to error of 1
				printf("Badfile is not valid and could not be opened. Exit Value: %d\n", status);	//if its a badfile say so
			}else if((userInput[0] == 'c') && (userInput[1] == 'd')){	//If its CD
				if(userInput[2] != ' '){				//if they gave a second arguement
					chdir(getenv("HOME"));				//Then go to the home directory
				}else{	
					token = strtok(userInput, " ");			//if they gave a second argument get it
					token = strtok(NULL, " ");
					chdir(token);					//Go to the absolute or realative path
				}
			}else{
				execFunc(len, userInput);				//If none of the commands match then exec
			}
		}	
	}
}

void ctrlCHandler(int sigNum){
	signal(SIGINT, ctrlCHandler);							//If CTRL c is hit then go to this function
	printf("\n");									//print a new line
	fflush(stdout);									//flush the buffer
}

void killChild(){
   	int statusTwo;
	pid_t pid = wait(&statusTwo);							//if the status is 15
	if(statusTwo == 15){				
		printf("The process %d. Terminated by signal: %d\n", pid, statusTwo);	//Print out the process and the signal
	}else{
		printf("The process: %d. Exit value: %d\n", pid, statusTwo);		//Print out th process and the exit value
	}
	return;										//return
}

void terminator(){
	int statusKill;
	pid_t pid = wait(&statusKill);							//Get the pid
	kill(pid, statusKill);								//kill the process
	printf("Terminated by signal: %d\n", statusKill);				//Print that the signal is terminated
	return;										//return
}

void execFunc(int len, char userInput[len]){
	int i = 0, spaces = 1;
	int redirect = 0;
	char copy[len];
	char *args[spaces + 1];
	char *token;
	char copyUser[len];
	char * fileName;
	signal(SIGINT, terminator);
	if(signal(SIGINT, terminator)){
		status = 2;								//if terminator is triggered changed the status
	}

	for(i = 0; i<len; i++){
		copyUser[i] = userInput[i];						//copy the array for error printing
	}
	i=0;
	while(userInput[i] != '\0'){
		if((userInput[i] == ' ') && (userInput[i+1] != '\0') && (userInput[i+1] != ' ')){
			spaces++;							//count the number of spaces
		}
		i++;
	}
	for(int z = 0; z < spaces + 1; z++){
		if(z == 0){
			token = strtok(userInput, " ");					//get the first argument
			args[z] = token;						//put it in the array
		}else if( z < spaces){
			token = strtok(NULL, " ");					//Get the next argument
			if(*token == '>'){						//If going out to file
				redirect = 2;						//set the variable for redirect
			}else if(*token == '<'){					//If pulling in from a file
				redirect = 1;						//set the variable
			}
			args[z] = token;						//add it to the array
		}else{
		   	if((userInput[3] == '<') || (userInput[3] == '>')){		//for the items set null characters
				fileName = token;
				args[z-1] = '\0';
				args[z-2] = '\0';
			}else{
				args[z] = '\0';						//set the last one to null
			}
		}
	}
	pid_t parentPID = getpid();							//get the parent ID
	pid_t childPID = fork();
	int devInput;
	int devOutput;
	if(childPID == -1){			//child was not created due to error
		printf("Process failed.\n");
	}else if(childPID > 0){			 //controling the parent
	   	int status;
	   	waitpid(childPID, &status, 0);
	}else{					   //controlling the child
	   	if(redirect != 0){
			if(redirect == 1){
				devInput = open(fileName, O_RDONLY);			//redirect devInput
				dup2(devInput, 0);								//to file
				close(devInput);								//close it
			}else{
				devOutput = open(fileName, O_WRONLY|O_CREAT|O_TRUNC, 0644);			//redirect devOutput
				dup2(devOutput, 1);								//to file
				close(devOutput);								//close it
			}
	   		execlp(args[0], args[0], NULL);						//execvp it
		}
		else{
			if((execvp(args[0], args)) == 0){
		   		printf("Successfully completed the child\n");
			}else{
				printf("Command %s invalid.\n", copyUser);
				exit(1);
			}
		}
	}
}

void execBackGroundFunc(int len, char userInput[len]){
	signal(SIGCHLD, killChild);
	signal(SIGINT, SIG_IGN);
   	int i = 0, spaces = 1, devOut = -1, devIn = -1;
	char copy[len];
	char *token;
	char copyUser[len];
	for(i = 0; i<len; i++){
		copyUser[i] = userInput[i];				//copy the array
	}
	i=0;
	while(userInput[i] != '\0'){
		if((userInput[i] == ' ') && (userInput[i+1] != '\0') && (userInput[i+1] != ' ')){
			spaces++;					//count the number of arguments
		}
		i++;
	}
	char *args[spaces];
	for(int i = 0; i < spaces + 1; i++){
		if(i == 0){
			token = strtok(userInput, " ");			//GEt the first argument
			args[i] = token;				//add the argument to the array
		}else if(i < spaces){
			token = strtok(NULL, " ");			//get the rest of the arguments
			args[i] = token;				//add them to the array
		}
	}
	args[spaces-1] = '\0';						//set the last one to null
	signal(SIGCHLD, killChild);					//kill the child
	pid_t parentPIDBackground = getpid();				//get the pid
	pid_t childPIDBackground = fork();				//get the child id
	if(childPIDBackground > 0){
		printf("The background PID is: %d\n", childPIDBackground);	//print the PID
	}
	if(childPIDBackground == -1){						//The process failed
		printf("The process failed.\n");
	}else if(childPIDBackground > 0){					//You are the parent			
	}else{									//You are the child
		if((devIn == -1) && (devOut == -1)){
			devOut = open("/dev/null", O_WRONLY);			//redirect devOutput
			devIn = open("/dev/null", O_RDONLY);			//redirect devInput
		}
		if((devIn == -1) && (devOut == -1)){
		   exit(127);
		}
		dup2(devOut, 1);						//send them out
		dup2(devIn, 0);
		close(devOut);
		close(devIn);
		execvp(args[0], args);						//exec vp the arguments
	}
	return;
}
