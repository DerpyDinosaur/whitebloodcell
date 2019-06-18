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

void menu(){
	char line[200];
	FILE * fp = fopen("./ascii.txt", "r");
	while(!feof(fp)){
		fgets(line, sizeof(line), fp);
		printf(RED"%s"RESET, line);
	}
	fclose(fp);
	printf("\n/// MENU \\\\\\\n");
	printf("[ A ] Analise malware via upload.\n");
	printf("[ S ] Scan PC for malware.\n");
	printf("[ Q ] Query database for malware via hash.\n");
}

int main(int argc, char *argv[]){
	// Init variables
	int opt;
	char optString[] = ":a:hMms";

	// Argument handler
	if (argc > 3){
		colourText("too many arguments\n", 'r');
		exit(0);	
	}else if (argc < 2){
		colourText("an argument is required, type -h for help\n", 'y');
		exit(0);
	}

	while((opt = getopt(argc, argv, optString)) != -1){
		switch(opt){
			case 'a':
				// Analise a file via upload
				printf("Analysing: %s\n", optarg);
				break;
			case 'h':
				printf("SYNOPSIS\n");
				printf("\twhitebc [OPTION...] [FILE...]\n");
				printf("\twhitebc -s [small, medium, large]\n");
				printf("\twhitebc -q [FILE, HASH...]\n");
				break;
			case 'm':
			case 'M':
				menu();
				break;
			case 's':
				// Scan computer for top 1000 malware hashes
				// Three different modes short, medium, long
				// printf("Scanning: %s\n", optarg);
				huntMalware();
				break;
			case 'q':
				// Query API for hash of file
				printf("Querying database: %s\n", optarg);
				break;
			case ':':
				colourText("arguments were not met, type -h for help\n", 'y');
				break;
			case '?':
			    colourText("argument does not exist, type -h for help\n", 'y');
			    break;
		}
	}
	return 0;
}