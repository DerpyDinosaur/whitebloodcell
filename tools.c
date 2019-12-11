/*
	Adam John Brickhill
	Email: adam.brickhill@lonetree.site
*/
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <termios.h>
#include <openssl/sha.h>
#include <sys/stat.h>
#include <time.h>
#include <sys/ioctl.h>

#define _stringSize 128
#define RED 	"\x1b[31m"
#define GREEN 	"\x1b[32m"
#define YELLOW 	"\x1b[33m"
#define BLUE 	"\x1b[34m"
#define RESET 	"\x1b[0m"

const char* colours[] = {RED, YELLOW, GREEN, BLUE};
static struct termios termSettings;

// Gets input from the user char by char and returns string
// Size can also be specified so a user cannot input past the desired length.
void getInput(char *text, char *output, int size){
	int eater;

	// Print custom message for input
	printf("%s", text);
	fgets(output, size, stdin);

	if (output[strlen(output)-1] != '\n'){
		while(((eater = getchar()) != '\n') && (eater != EOF));
	}

	output[strlen(output)] = '\0';
}

void nomChars(char *charArray, char *output){
	int badChars, i = 0;
	char newString[strlen(output)];

	for (int x = 0; x < strlen(output); ++x){
		badChars = 0;
		for (int y = 0; y < strlen(charArray); ++y){
			if (charArray[y] == output[x]){
				badChars++;
			}
		}

		if (badChars == 0){
			newString[i++] = output[x];
		}
	}

	newString[i] = '\0';
	strcpy(output, newString);
}

// Clears screen both windows and linux
void clearScreen(){
	printf("\033[H\033[J");
	// system("@cls||clear");
}

void cursorManip(int io){
	if (io){
		// Show cursor
		printf("\e[?25h");
	}else if(!io){
		// Hide cursor
		printf("\033[?25l");
	}
}

// Function will grab a specified variable from the config file
// This allows the program to locate one config rather than all three at once
// TODO: Allow user to change config path
void getConfig(char *output, char mode){
	char line[128];
	char *confName;
	bool configIsSet = false;

	switch(mode){
		case 'm':
			confName = "[Path to md5sum]\n";
			break;

		case 's':
			confName = "[Path to sha1sum]\n";
			break;

		case 'S':
			confName = "[Path to sha256sum]\n";
			break;

		default:
			puts(RED"Incorrect Function Mode: getConfig()");
			exit(0);
	}

	//Attempt to load config file
	FILE * configfp = fopen("custodia/config.wbc", "r");
	if (configfp == NULL){
		puts(RED"ERROR: Config file could not be opened\n");
		exit(0);
	}

	// Loop while not end of config file
	while(!feof(configfp)){
		// Grab line in config
		fgets(line, sizeof(line), configfp);
		// If line is equal to the config name and config is not set
		if (configIsSet == false && strcmp(line, confName) == 0){
			// Loop until line is not equal to a return and space character
			while (true){
				fgets(line, sizeof(line), configfp);
				if (line[0] != '\n' && line[0] != ' '){
					nomChars(" ", line);
					configIsSet = true;
					break;
				}
			}
		}// Break loop when config is set
		else if (configIsSet == true){
			break;
		}
	}
	fclose(configfp);
	strcpy(output, line);
}

// Colour functions
void colourText(char *text, char colour){
	if (colour == 'r'){
		printf(RED"%s"RESET, text);
	}else if(colour == 'g'){
		printf(GREEN"%s"RESET, text);
	}else if(colour == 'y'){
		printf(YELLOW"%s"RESET, text);
	}else if(colour == 'b'){
		printf(BLUE"%s"RESET, text);
	}
}

void rainbow(char *text) {                                                                                                                  
	int i = 0;
	while(*text) {
		if(*text == ' ' || *text == '\n') {
			printf(RESET "%c", *text);
		} else {
			printf("%s%c", colours[i], *text);
			i = (i + 1) % 4;
		}
		text++;
	}
	printf(RESET"\n");
}

// Turn echo in terminal on or off
void echoIO(bool echoIsOn){
	if (echoIsOn == true){
		tcsetattr(0, TCSANOW, &termSettings);
	}else{
		struct termios newSettings;
		tcgetattr(0, &termSettings);
		newSettings = termSettings;
		newSettings.c_lflag &= (~ECHO);
		tcsetattr(0, TCSANOW, &newSettings);		
	}
	return;
}

// Press enter to continue program
void enterToContinue(){
	int c;
   	while((c = getchar()) != '\n' && c != EOF);
}

void fileSize(char const filepath[]){
	struct stat file;

	if(stat(filepath,&file) == -1){
		printf("Error Occurred\n");
	}

	printf("\nFile size: %ld \n",file.st_size);
}

void typeWriter(char *string, int width, bool newline){
	// Set nanosleep time @ 1/4 second
	struct timespec tim, tim2;
	tim.tv_sec = 0;
	// tim.tv_nsec = 330000000L;
	tim.tv_nsec = 100000000L;

	int center = (width / 2) - (strlen(string) / 2);
	for(int i = 0; string[i] != '\0'; i ++){
		if (i==0){
			printf("%*c", center, string[i]);
		}else{
			printf("%c", string[i]);

			if (nanosleep(&tim, &tim2) > 0){
				printf("Nanosleep failed!");
				exit(EXIT_FAILURE);
			}
		}
		fflush(stdout);
	}if (newline){
		printf("\n");
	}
}