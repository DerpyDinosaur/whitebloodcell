#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <json-c/json.h>

#include "tools.h"

<<<<<<< Updated upstream
void huntMalware(){
	// Variables
	FILE * fp = fopen("apisample.json", "r+");
	if (fp == NULL){
		colourText("ERROR: JSON file could not be read\nProgram terminating...", 'r');
		return;
	}
	char line[1711];
	// int numOfMalware;

	// JSON document
	struct json_object *json;
	// JSON fields
	// struct json_object *md5;
	// struct json_object *threat_name;

	fseek(fp, 1, SEEK_END);
	fputs("", fp);
	fseek(fp, 1, SEEK_SET);
	fputs("", fp);
	fread(line, 1711, 1, fp);
	fclose(fp);

	json = json_tokener_parse(line);

	json_object_object_foreach(json, key, val){
		printf("%s\n", json_object_get_string(val));
	}

	// json_object_object_get_ex(json, "md5", &md5);
	// json_object_object_get_ex(json, "threat_name", &threat_name);

	// printf("Name: %s\n", json_object_get_string(threat_name));
=======
bool updateMalwareDatabase(){
	// Variables
	FILE * fp = fopen("apisample.json", "r");
	if (fp == NULL){
		colourText("ERROR: JSON file could not be read\nProgram terminating...", 'r');
		exit(0);
	}
	bool dataUpdated = false;
	char line[1024];
	const char delim[2] = ":";
	char *token;

	// JSON PARSER
	fgets(line, sizeof(line), fp);
	fgets(line, sizeof(line), fp);
	fgets(line, sizeof(line), fp);
	token = strtok(line, delim);
	token = strtok(NULL, delim);
	printf("%s\n", token);

	fgets(line, sizeof(line), fp);
	token = strtok(line, delim);
	token = strtok(NULL, delim);
	printf("%s\n", token);

	fgets(line, sizeof(line), fp);
	token = strtok(line, delim);
	token = strtok(NULL, delim);
	printf("%s\n", token);

	// while(token != NULL){
	// 	printf("%s\n", token);
	// 	fgets(line, sizeof(line), fp);
	// 	token = strtok(NULL, delim);
	// }

	fclose(fp);
	return dataUpdated;
}

void huntMalware(bool verbose, char searchTime){
	bool isUpdated = updateMalwareDatabase();
	/*DEBUG LINE*/verbose = true;
	if (verbose){
		if (isUpdated == true){
			colourText("Malware database was updated...\n", 'y');
		}else if(isUpdated == false){
			colourText("Malware database did not update...\n", 'y');
		}
	}

>>>>>>> Stashed changes
}