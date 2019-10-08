#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/ioctl.h>
#include <errno.h>
#include <time.h>

#include "tools.h"
#include "scan.h"

#define RED 	"\x1b[31m"
#define GREEN 	"\x1b[32m"
#define YELLOW 	"\x1b[33m"
#define BLUE 	"\x1b[34m"
#define RESET 	"\x1b[0m"

void displayHelp(){
	puts("USAGE");
	puts("\twhitebc [OPTION...] [FILE...]");
	puts("\twhitebc -s [s, l] or [small, large]");
	puts("\twhitebc -q [FILE, HASH...]");
	puts("\twhitebc -v [ARGUMENT...]");

	puts("\nDESCRIPTION");
	puts("\t-v\tVerbose mode, enter as the first argument or it will be ignored.");
}

void signalHandler(int sig){
	// Re-enable cursor
	printf("\e[?25h");
	exit(EXIT_SUCCESS);
}

void feed(int update){
	// Signal handler
	signal(SIGINT, signalHandler);

	if (update){
		updateMalwareData();
	}

	// Get width of terminal
	struct winsize w;
	if (ioctl(STDIN_FILENO, TIOCGWINSZ, &w) == -1) {
    	fprintf(stderr, "Cannot find STDIN info: %s\n", strerror(errno));
    	exit(EXIT_FAILURE);
	}
	int width = w.ws_col;

	// Blocked Unbuffered
	setvbuf(stdout, NULL, _IOFBF, 0);

	// Blank screen ready for printing
	clearScreen();

	// Remove cursor
	cursorManip(0);

	// Print menu
	int center = (width / 2) - (60 / 2);
	for (int i = 0; i < 10; ++i){
		char *line;
		switch(i){
			case 0:
				line = "▄▄▌ ▐ ▄▌ ▄ .▄▪  ▄▄▄▄▄▄▄▄ .    ▄▄▄▄· ▄▄▌              ·▄▄▄▄  \n";
				break;
			case 1:
				line = "██· █▌▐███▪▐███ •██  ▀▄.▀·    ▐█ ▀█▪██•  ▪     ▪     ██▪ ██ \n";
				break;
			case 2:
				line = "██▪▐█▐▐▌██▀▐█▐█· ▐█.▪▐▀▀▪▄    ▐█▀▀█▄██▪   ▄█▀▄  ▄█▀▄ ▐█· ▐█▌\n";
				break;		
			case 3:
				line = "▐█▌██▐█▌██▌▐▀▐█▌ ▐█▌·▐█▄▄▌    ██▄▪▐█▐█▌▐▌▐█▌.▐▌▐█▌.▐▌██. ██ \n";
				break;
			case 4:
				line = " ▀▀▀▀ ▀▪▀▀▀ ·▀▀▀ ▀▀▀  ▀▀▀     ·▀▀▀▀ .▀▀▀  ▀█▄▀▪ ▀█▄▀▪▀▀▀▀▀• \n";
				break;
			case 5:
				line = "                     ▄▄· ▄▄▄ .▄▄▌  ▄▄▌                      \n";
				break;
			case 6:
				line = "                    ▐█ ▌▪▀▄.▀·██•  ██•                      \n";
				break;
			case 7:
				line = "                    ██ ▄▄▐▀▀▪▄██▪  ██▪                      \n";
				break;
			case 8:
				line = "                    ▐███▌▐█▄▄▌▐█▌▐▌▐█▌▐▌                    \n";
				break;
			case 9:
				line = "                    ·▀▀▀  ▀▀▀ .▀▀▀ .▀▀▀                     \n\n\n";
				break;
		}

		for (int i = 0; i < center; ++i){
			printf(" ");
		}

		for (int i = 0; i < strlen(line); ++i){
			printf("%c", line[i]);
		}
	}

	// Main loop
	char line[100];
	FILE * fp = fopen("custodia/most-wanted.wbc", "r");
	// If file cant be read replace title and continue
	if (fp == NULL){
		colourText("Error opening file: most-wanted.wbc", 'r');
	}else{
		while(!feof(fp)){
			fgets(line, 100, fp);
			typeWriter(line, width, false);
		}
		fclose(fp);
	}printf("\n");

	// Re-enable cursor
	cursorManip(1);
	feed(0);
}

void menu(){
	char line[200];
	FILE * fp = fopen("custodia/ascii.wbc", "r");
	// If file cant be read replace title and continue
	if (fp == NULL){
		puts(RED"\t\t*Insert ascii title here*"RESET);
		puts(RED"\tSomething went wrong with the ascii file"RESET);
	}else{
		while(!feof(fp)){
			fgets(line, sizeof(line), fp);
			colourText(line, 'r');
		}
		fclose(fp);
	}

	printf(RESET);
	printf("\n/// MENU \\\\\\\n");
	printf("[ A ] Analise malware via upload.\n");
	printf("[ F ] Animated malware feed.\n");
	printf("[ S ] Scan PC for malware.\n");
	printf("[ Q ] Query database via hash or string.\n");
	printf("[ W ] Check website integrity.\n");
}

int main(int argc, char *argv[]){
	// Init variables
	int opt;
	bool verbose = false;

	// Load config file
	// TODO: config loader

	// Argument handler
	if (argc == 1){
		feed(0);
		exit(EXIT_SUCCESS);
	}

	while((opt = getopt(argc, argv, ":a:hMms:q:Uv")) != -1){
		switch(opt){
			case 'a':
				// Analise a file via upload
				printf("Analysing: %s\n", optarg);
				exit(EXIT_SUCCESS);
			case 'm':
			case 'M':
				menu();
				exit(EXIT_SUCCESS);
			case 's':
				// Scan computer for top 1000 malware hashes
				// Three different modes short, long
				if (strcmp(optarg, "short") == 0 || strcmp(optarg, "s") == 0){
					huntMalware(verbose, 's');
				}else if(strcmp(optarg, "long") == 0 || strcmp(optarg, "l") == 0){
					huntMalware(verbose, 'l');
				}else{
					colourText("Incorrect usage of argument options.\n", 'y');
					exit(EXIT_SUCCESS);
				}
				exit(EXIT_SUCCESS);
			case 'q':
				// Query API for hash of file
				printf("Querying database: %s\n", optarg);
				exit(EXIT_SUCCESS);
			case 'U':
				// Initiate feed function and update data
				feed(1);
				exit(EXIT_SUCCESS);
			case 'v':
				verbose = true;
				break;
			case ':':
				colourText("Arguments were not met, type -h for help\n", 'y');
				exit(EXIT_SUCCESS);
			case 'h':
			case '?':
				displayHelp();
			    exit(EXIT_SUCCESS);
			default:
				break;
		}
	}
	return 0;
}