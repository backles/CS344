#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<string.h>
#include<unistd.h>
#include<sys/stat.h>
#include<sys/types.h>
int roomName[2][10]={	//Names of the rooms to use
	{1,2,3,4,5,6,7,8,9,10},
	{0,0,0,0,0,0,0,0,0,0}
};
int nameUsed[7];	//names actually used for rooms 
int type[7];		//type of room for each room
int connections[7][7];	//each room and their connections
int pathHistory[100];	//the path taken to get to the ending room

void roomMaker(); 	//make room information
void dirMaker();	//write it to the file and read it back
void gamePlay();	//play the game

int main(){
   	srand(time(NULL));	//set SRAND
	roomMaker();		//make rooms
	dirMaker();		//put them in and out of a file
	printf("The ending room is: %d\n", nameUsed[6]);
	gamePlay();		//play the game
	return 0;
};

void gamePlay(){
   	int curPos = 0, x, userPos = nameUsed[0], curRmType=0;
	int userInput, temp, count = 1;
	printf("Hi welcome to Braden's version of adventure!\n");		//Welcome the user
	printf("The rules are simple get to the ending room and you win!\n");	//Rules
	pathHistory[count]=userPos;						//Start the history path with the first room
	count++;							
	while(curRmType != 2){							//While the room the user is in is not he ending room
		printf("Currently Location: %d\n", userPos);			//Tell them whch room they are in
		printf("Possible connections: ");				//Tell them their options for where to go
		for(x=0; x<=6;x++){						
		   	if(connections[curPos][x] !=0 ){
				printf("%d, ",nameUsed[connections[curPos][x]]);	//print out the connections
			} 
		}
		printf(".\nWhere to?>");						//Where does the user want to go
		scanf("%d", &userInput);						//user input
		//printf("%d",connections[curPos][userInput-1]);
		if(userInput==userPos || userInput==connections[curPos][userInput]){
			printf("HUH? I DON’T UNDERSTAND THAT ROOM. TRY AGAIN.\n");	//they had an invalid entry
		}else{
			userPos = userInput;						//The user position is now what the user wanted
	        	for(temp=0;temp<=6;temp++){
				if(nameUsed[temp]==userPos){
					curPos=temp;					//name check and exchange
					curRmType=type[temp];
					break;
				};
			};
			pathHistory[count] = userPos;					//update the path the user has taken
			count++;		
		};
	};
	printf("You have reached the end room\n");					//The user has won the game
	printf("The path you took to get to the end was: ");				//Tell them the path they took
	for(x=0; x<20; x++){
	   	if(pathHistory[x]!=0){
			printf("%d ", pathHistory[x]);
		}
	};
	printf("\n");
};

void dirMaker(){
   	pid_t pid;
	pid=getpid();
   	int x, j, count=1, first=1, temp=1, lines=0, ch=0;
	char *token;
	char *beginning = "./acklesb.rooms.";
	char directoryName[20];
	char fileName[5];
	char path[25];
	char info[20];
	char *line=NULL;
	size_t length = 0;
	ssize_t read;
	FILE *fp;
	sprintf(directoryName, "%s%d", beginning, pid);
	int folder = mkdir(directoryName, 0777);				//make the directory
	for(x=0;x<=6;x++){
	   	strcpy(path,"");
		sprintf(fileName, "%s%d", "room", x+1);
		sprintf(path, "%s/%s", directoryName, fileName); 		//make the directory name with file name
		fp=fopen(path, "w+");						//
		fprintf(fp, "ROOM NAME: %d\n", nameUsed[x]);			//print out the room name to the file
		count = 1;
		for(j=0;j<=6;j++){
		   	if(connections[x][j]!=0){
				fprintf(fp, "CONNECTION %d: %d\n", count, connections[x][j]);	//print out the connections
				count++;
			};
		};
		fprintf(fp, "ROOM TYPE: %d\n", type[x]);			//print out the room type
		fclose(fp);							//close the file
		fp = fopen(path, "r"); 						//open file
		//count lines
		while(!feof(fp)){
			ch = fgetc(fp);
			if(ch == '\n'){
				lines++;
			};
		};
		temp=0;
		while((read = getline(&line, &length, fp)) != -1){
			if(temp==1){					//get the value out of the file of the room name
				token=strtok(line, " ");
				token=strtok(NULL, " ");
				token=strtok(NULL, " ");
				nameUsed[temp-1]=atoi(token);
			}else if(temp!=1 && temp != lines){		//get the value out of the file of the connections
				token=strtok(line, " ");
				token=strtok(NULL, " ");
				token=strtok(NULL, " ");
				connections[x][temp-1]=atoi(token);
			}else if (temp == lines){			//get the value out of the file of the type
				token=strtok(line, " ");
				token=strtok(NULL, " ");
				type[temp-1] = atoi(token);
			}
			temp++;
		};
	};
};

void roomMaker(){
	int i, rando, x, y, numConnect, temp;
	for(i=0; i <= 6;i++){//for every Room Created
		temp=0;
		do{
		     	rando= rand() % 10 + 1; 		//Random place for name
			if(roomName[2][rando]==1){		//if that name is used already
			   	rando = rand() % 10 + 1;	//get new random
			}else{					//otherwise
				temp=1;				//Get out of loop
				nameUsed[i]=rando;		//set the name to place
				roomName[2][rando]=1;		//Say you are using that name now
			}
		}while(temp==0);
		do{					
			numConnect = rand() % 6 + 1;	//Set the number of connections for that room
		}while(numConnect < 3);			//If the number of connections is less than 3 go again
		for(x=0; x<=numConnect-1; x++){
		   	temp=1;
			rando= rand() % 7 + 1;
	   		do{
				//see if already exists
				if(connections[i][rando-1]!=0){
					rando= rand() % 7 + 1;
				}else{
					connections[i][rando-1]=rando;
					connections[rando-1][i]=i+1;
					temp=1;		//be able to exit loop
				}
			}while(temp==0);
		};
		connections[i][i]=0;	//Make sure a room doesn't connect to its self
		if(i==0){
			type[i]=0;	//if its the first room make it the starter
		}else if(i==6){
			type[i]=2;	//if its the last room make it the ender
		}else{
			type[i]=1;	//anything else its a middle room
		}

	};
};
