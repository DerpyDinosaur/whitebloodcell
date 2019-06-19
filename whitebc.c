#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>

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
	puts("\t-v\tVerbose mode.");
}

void menu(){
	char line[200];
	FILE * fp = fopen("./ascii.txt", "r");
	// If file cant be read replace title and continue
	if (fp == NULL){
		puts(RED"\t\t*Insert ascii title here*"RESET);
		puts(RED"\tSomething went wrong with the ascii file"RESET);
	}else{
		while(!feof(fp)){
			fgets(line, sizeof(line), fp);
			printf(RED"%s"RESET, line);
		}
		fclose(fp);
	}

	printf("\n/// MENU \\\\\\\n");
	printf("[ A ] Analise malware via upload.\n");
	printf("[ S ] Scan PC for malware.\n");
	printf("[ Q ] Query database for malware via hash.\n");
}

int main(int argc, char *argv[]){
	// Init variables
	int opt;
	bool verbose = false;

	// Argument handler
	if (argc > 3){
		colourText("too many arguments\n", 'r');
		exit(0);	
	}else if (argc < 2){
		colourText("an argument is required, type -h for help\n", 'y');
		exit(0);
	}

	while((opt = getopt(argc, argv, ":a:hMms:v")) != -1){
		switch(opt){
			case 'a':
				// Analise a file via upload
				printf("Analysing: %s\n", optarg);
				break;
			case 'm':
			case 'M':
				menu();
				break;
			case 's':
				// Scan computer for top 1000 malware hashes
				// Three different modes short, long
				// printf("Scanning: %s\n", optarg);
				if (strcmp(optarg, "short") == 0 || strcmp(optarg, "s") == 0){
					huntMalware(verbose, 's');
				}else if(strcmp(optarg, "long") == 0 || strcmp(optarg, "l") == 0){
					huntMalware(verbose, 'l');
				}else{
					colourText("Incorrect usage of arguments.\n", 'y');
					exit(0);
				}
				break;
			case 'q':
				// Query API for hash of file
				printf("Querying database: %s\n", optarg);
				break;
			case 'v':
				verbose = true;
			case ':':
				colourText("arguments were not met, type -h for help\n", 'y');
				break;
			case 'h':
			case '?':
				displayHelp();
			    exit(0);
			default:
				break;
		}
	}
	return 0;
}