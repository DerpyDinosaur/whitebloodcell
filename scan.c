#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <openssl/sha.h>
#include <openssl/md5.h>

#include "tools.h"

struct tmp{
	// Char length should match length of hash size
	char md5[100];
	char sha1[100];
	char sha256[100];
	char name[100];
};

void hash(){
	/* 
		TODO: Write your own hashing algorithm
		https://www.ietf.org/rfc/rfc1321.txt
	*/

	// execl("/usr/bin/md5sum", "md5sum", "custodia/ascii.wbc", NULL);
	// find -type f \( -not -name "md5sum.txt" \) -exec md5sum '{}' \; > md5sum.txt
}

bool updateMalwareData(){
	// Variables
	FILE * jsonfp = fopen("custodia/apisample.json", "r");
	if (jsonfp == NULL){
		colourText("ERROR: JSON file could not be read\nProgram terminating...\n", 'r');
		exit(0);
	}

	FILE * wantedfp = fopen("custodia/most-wanted.wbc", "w");
	if (wantedfp == NULL){
		colourText("ERROR: Wanted file could not be created\nProgram terminating...\n", 'r');
		exit(0);
	}
	bool dataUpdated = true;
	char line[1024];
	const char delim[2] = ":";
	char *token;
	char badChars[] = "\", ";
	struct tmp tmp;

	// JSON PARSER
	/*
		11 * 1000 + 2 = 11002
	*/

	// NOM '['
	fgets(line, sizeof(line), jsonfp);
	for (int i = 0; i < 3; ++i){
		// NOM '{'
		fgets(line, sizeof(line), jsonfp);
		// Get MD5 line
		fgets(line, sizeof(line), jsonfp);
		token = strtok(line, delim);
		token = strtok(NULL, delim);
		strcpy(tmp.md5, token);
		nomChars(badChars, tmp.md5);

		// Get SHA1 line
		fgets(line, sizeof(line), jsonfp);
		token = strtok(line, delim);
		token = strtok(NULL, delim);
		strcpy(tmp.sha1, token);
		nomChars(badChars, tmp.sha1);

		// Get SHA256 line
		fgets(line, sizeof(line), jsonfp);
		token = strtok(line, delim);
		token = strtok(NULL, delim);
		strcpy(tmp.sha256, token);
		nomChars(badChars, tmp.sha256);

		// Get Name line
		fgets(line, sizeof(line), jsonfp);
		fgets(line, sizeof(line), jsonfp);
		fgets(line, sizeof(line), jsonfp);
		token = strtok(line, delim);
		token = strtok(NULL, delim);
		strcpy(tmp.name, token);
		nomChars("\",", tmp.name);
		tmp.name[0] = '#';

		fgets(line, sizeof(line), jsonfp);
		fgets(line, sizeof(line), jsonfp);
		fgets(line, sizeof(line), jsonfp);
		fgets(line, sizeof(line), jsonfp);

		fprintf(wantedfp, "%s\n", tmp.name);
		fprintf(wantedfp, "%s\n", tmp.md5);
		fprintf(wantedfp, "%s\n", tmp.sha1);
		fprintf(wantedfp, "%s\n", tmp.sha256);

		// puts(tmp.name);
		// puts(tmp.md5);
		// puts(tmp.sha1);
		// puts(tmp.sha256);
	}
	fclose(wantedfp);
	fclose(jsonfp);

	return dataUpdated;
}

void huntMalware(bool verbose, char searchTime){
	/*
		~ LEADS ~
		ftw and nftw / Searching for files
	*/
	/*DEBUG LINE*/verbose = true;
	if (updateMalwareData() && verbose == true){
		colourText("Malware database was updated...\n", 'g');
	}
}