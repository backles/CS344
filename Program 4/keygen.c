#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>

void main(int argc, char * argv[]){
	int length = atoi(argv[1]);
	int rando = 0;
	char *string[length];
	srand(time(NULL));
   	if(argc != 2){
		printf("You did not enter enough parameter");		//if no length was specified
		exit(1);											//quit
	}
	if(length < 1){											//if the length is not long enough then you can't do it
		printf("You did not enter a valid length");
		exit(1);
	}
	//get random characters for each length
	for(int x = 0; x <= length-1; x++){
		rando = rand() % 27;
		switch(rando){
		   	case 0:
				string[x] = "A";
				break;
			case 1:
			   	string[x] = "B";
				break;
			case 2:
				string[x] = "C";
				break;
			case 3:
				string[x] = "D";
				break;
			case 4:
				string[x] = "E";
				break;
			case 5:
				string[x] = "F";
				break;
			case 6:
				string[x] = "G";
				break;
			case 7:
				string[x] = "H";
				break;
			case 8:
				string[x] = "I";
				break;
			case 9:
				string[x] = "J";
				break;
			case 10:
				string[x] = "K";
				break;
			case 11:
				string[x] = "L";
				break;
			case 12:
				string[x] = "M";
				break;
			case 13:
				string[x] = "N";
				break;
			case 14:
				string[x] = "O";
				break;
			case 15:
				string[x] = "P";
				break;
			case 16:
				string[x] = "Q";
				break;
			case 17:
				string[x] = "R";
				break;
			case 18:
				string[x] = "S";
				break;
			case 19:
				string[x] = "T";
				break;
			case 20:
				string[x] = "U";
				break;
			case 21:
				string[x] = "V";
				break;
			case 22:
				string[x] = "W";
				break;
			case 23:
				string[x] = "X";
				break;
			case 24:
				string[x] = "Y";
				break;
			case 25:
				string[x] = "Z";
				break;
			case 26:
				string[x] = " ";
				break;
		}
     		printf("%s", string[x]);	//print the random characters	
	}
	printf("\n");						//add new line characters
}
