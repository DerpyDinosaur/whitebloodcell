/*
	Adam John Brickhill
	Student Number: 10445080
	Unit: CSP2308

	References:
	https://www.thecodingforums.com/threads/newbie-turn-off-echo-in-c.315753/
	https://c-program-example.com/2012/04/c-program-to-encrypt-and-decrypt-a-password.html
*/
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <termios.h>
#include <openssl/sha.h>

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
void getInput(char *text, char *output){
	int eater;

	// Print custom message for input
	printf("%s", text);
	fgets(output, sizeof(output), stdin);

	if (output[sizeof(output)-1] != '\n'){
		while(((eater = getchar()) != '\n') && (eater != EOF));
	}

	output[sizeof(output)-1] = '\0';
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

	newString[i-1] = '\0';
	strcpy(output, newString);
}

// Clears screen both windows and linux
void clearScreen(){
	system("@cls||clear");
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