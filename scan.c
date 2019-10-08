#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <openssl/sha.h>
#include <openssl/md5.h>

#include "tools.h"

struct tmp{
	// TODO: Char length should match length of hash size
	char name[100];
	char size[100];
	char md5[100];
	char sha1[100];
	char sha256[100];
	char analysisTime[100];
	// Score 0 out of 100
	char threatScore[100];
	char threatLevel[100];
	char malwareFamily[100];
	char fileType[100];
	char enviroment[100];
	char reportUrl[200];
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
	FILE * jsonfp = fopen("custodia/feed.json", "r");
	if (jsonfp == NULL){
		colourText("ERROR: JSON file could not be read\nProgram terminating...\n", 'r');
		exit(EXIT_FAILURE);
	}

	FILE * wantedfp = fopen("custodia/most-wanted.wbc", "w");
	if (wantedfp == NULL){
		colourText("ERROR: File 'most-wanted.wbc' failed to open\n", 'r');
		exit(EXIT_FAILURE);
	}

	bool dataUpdated = true;
	char line[200];
	const char delim[2] = ":";
	char *token;
	char badChars[] = "\",";
	struct tmp tmp;
	int entryNumber = 1;

	while(!feof(jsonfp)){
		fgets(line, sizeof(line), jsonfp);
		token = strtok(line, delim);
		nomChars(" ", token);

		if (strcmp(token, "\"job_id\"") == 0){
			if (entryNumber != 1){
				fprintf(wantedfp, "%s", tmp.name);
				fprintf(wantedfp, "%s", tmp.md5);
				fprintf(wantedfp, "%s", tmp.sha1);
				fprintf(wantedfp, "%s", tmp.sha256);
				fprintf(wantedfp, "%s", tmp.analysisTime);
				fprintf(wantedfp, "%s", tmp.threatScore);
				fprintf(wantedfp, "%s", tmp.threatLevel);
				fprintf(wantedfp, "%s", tmp.malwareFamily);
				fprintf(wantedfp, "%s", tmp.fileType);
				fprintf(wantedfp, "%s", tmp.enviroment);
				fprintf(wantedfp, "%s\n", tmp.reportUrl);
			}
			fprintf(wantedfp, "#%d\n", entryNumber);
			entryNumber++;

		}else if (strcmp(token, "\"md5\"") == 0){
			token = strtok(NULL, delim);
			nomChars(badChars, token);
			strcpy(tmp.md5, "MD5: ");
			strcat(tmp.md5, token);

		}else if(strcmp(token, "\"sha1\"") == 0){
			token = strtok(NULL, delim);
			nomChars(badChars, token);
			strcpy(tmp.sha1, "SHA1: ");
			strcat(tmp.sha1, token);

		}else if(strcmp(token, "\"sha256\"") == 0){
			token = strtok(NULL, delim);
			nomChars(badChars, token);
			strcpy(tmp.sha256, "SHA256: ");
			strcat(tmp.sha256, token);

		}else if(strcmp(token, "\"threat_score\"") == 0){
			token = strtok(NULL, delim);
			nomChars(badChars, token);
			strcpy(tmp.threatScore, "Threat Score: ");
			strcat(tmp.threatScore, token);

		}else if(strcmp(token, "\"threat_level_human\"") == 0){
			token = strtok(NULL, delim);
			nomChars(badChars, token);
			strcpy(tmp.threatLevel, "Threat Level: ");
			strcat(tmp.threatLevel, token);
			
		}else if(strcmp(token, "\"vx_family\"") == 0){
			token = strtok(NULL, delim);
			nomChars(badChars, token);
			strcpy(tmp.malwareFamily, "Malware Family: ");
			strcat(tmp.malwareFamily, token);
			
		}else if(strcmp(token, "\"submit_name\"") == 0){
			token = strtok(NULL, delim);
			nomChars(badChars, token);
			strcpy(tmp.name, "Name: ");
			strcat(tmp.name, token);
			
		}else if(strcmp(token, "\"size\"") == 0){
			token = strtok(NULL, delim);
			nomChars(badChars, token);
			strcpy(tmp.size, "Size: ");
			strcat(tmp.size, token);
			
		}else if(strcmp(token, "\"type\"") == 0){
			token = strtok(NULL, delim);
			nomChars(badChars, token);
			strcpy(tmp.fileType, "File Type: ");
			strcat(tmp.fileType, token);

		}else if(strcmp(token, "\"environment_description\"") == 0){
			token = strtok(NULL, delim);
			nomChars(badChars, token);
			strcpy(tmp.enviroment, "Enviroment: ");
			strcat(tmp.enviroment, token);
			
		}else if(strcmp(token, "\"report_url\"") == 0){
			token = strtok(NULL, delim);
			nomChars(badChars, token);
			strcpy(tmp.reportUrl, "Report Url: ");
			strcat(tmp.reportUrl, token);
		}
	}
	fclose(jsonfp);
	fclose(wantedfp);

	return dataUpdated;
}

void huntMalware(bool verbose, char searchTime){
	/*
		~ LEADS ~
		ftw and nftw / Searching for files
	*/
	if (updateMalwareData() && verbose == true){
		colourText("Malware database was updated...\n", 'g');
	}
}