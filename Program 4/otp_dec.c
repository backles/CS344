#define h_addr h_addr_list[0]
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<strings.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<netdb.h>

void error(char*);

int main(int argc, char * argv[]){
	int portNo, textLength, keyLength, sockfd, nText, nKey, checkSockSending = 2, checkSockRecieving;
	struct sockaddr_in serverAddress;
	struct hostent *server;
	char textArray[2048];			//array holding the text array
	char keyArray[2048];			//array holding the key array
	FILE *textfp = NULL;			//the text file
	FILE *keyfp = NULL;				//the key file

	if(argc != 4){					//if there are not 4 arguments then error
		error("You entered an incorrect format try: \"otp_dec <plaintextfile> <keyfile> <port>\"\n");
	}
	textfp = fopen(argv[1], "r");	//open the text file in a read only session
	if(textfp == NULL){				//if its null then it didn't open successfully
		error("Unable to open the text file.\n");
	}
	keyfp = fopen(argv[2], "r");	//open the key file in a read only session
	if(keyfp == NULL){				//if its null then it didn't open successfully
		error("Unable to open the key file.\n");
	}
	while(fgets(textArray, sizeof(textArray), textfp)){
		textLength = strlen(textArray);			//get the length of the text file
	}
	while(fgets(keyArray, sizeof(keyArray), keyfp)){
		keyLength = strlen(keyArray);			//get the length of the key file
	}
	fclose(textfp);								//close the text file
	fclose(keyfp);								//close the key file`
	if(textLength > keyLength){					//if the text file is not longer than the key error out
		error("ERROR the key length id less than the text length");
	}
	portNo = atoi(argv[3]);						//set the port number
	sockfd = socket(AF_INET, SOCK_STREAM, 0);	//set the socket
	if(sockfd < 0){								//if the socket is less than 0 it was not set
		error("Could not open the socket.");
	}	   
	server = gethostbyname("localhost");		//connect to self
	if(server == NULL){							//if the server is null then it was not connected
		error("ERROR couldn't connect to the server");
	}	   
	bzero((char*) &serverAddress, sizeof(serverAddress));	//zero out the server address
	serverAddress.sin_family = AF_INET;						//set the server address info
	serverAddress.sin_port = htons(portNo);					//set the server address info
	bcopy((char*) server->h_addr, (char*) &serverAddress.sin_addr.s_addr, server->h_length);	//copy the sever information
	if(connect(sockfd, (struct sockaddr *) &serverAddress, sizeof(serverAddress)) < 0){		//check connection to server
		printf("Error: could not contact the daemon at port: %d\n", portNo);				//error if could not contact daemon
		exit(2);																			//exit 2
	}
	write(sockfd, &checkSockSending, sizeof(checkSockSending));
	read(sockfd, &checkSockRecieving, sizeof(checkSockRecieving));
	if(checkSockRecieving != 2){
		error("Tried connecting on the wrong socket");
	}
	nText = write(sockfd, textArray, 2047);		//write the information to the daemon
	if(nText < 0){								//if its less than 0 then the information was not sent
		error("Error writing the text array to socket");
	}
	nKey = write(sockfd, keyArray, 2047);		//send the key file to the daemon
	if(nKey < 0){								//if it is less than 0 it was not sent
		error("Error writing the key array to socket");
	}
	bzero(textArray, 2047);						//set the buffer size
	nText = read(sockfd, textArray, 2047);		//read the unciphered text
	if(nText < 0){								//if its less than 0 it was not read
		error("Error reading from the socket");
	}
	printf("%s\n", textArray);					//print the unciphered text
	close(sockfd);								//close the socket
	return 0;									//return 0
}	

void error(char * error){
	printf("%s\n", error);						//print error message
	exit(1);									//exit 1
}
