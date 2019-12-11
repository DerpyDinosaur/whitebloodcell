#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>

#include <curl/curl.h>

#include "tools.h"

#define strSize 200
#define RED 	"\x1b[31m"
#define GREEN 	"\x1b[32m"
#define YELLOW 	"\x1b[33m"
#define BLUE 	"\x1b[34m"
#define RESET 	"\x1b[0m"

struct tmp{
	// TODO: Char length should match length of hash size
	char name[strSize];
	char size[strSize];
	char md5[strSize];
	char sha1[strSize];
	char sha256[strSize];
	char analysisTime[strSize];
	// Score 0 out of 100
	char threatScore[strSize];
	char threatLevel[strSize];
	char malwareFamily[strSize];
	char fileType[strSize];
	char enviroment[strSize];
	char reportUrl[strSize];
};

static size_t write_data(void *ptr, size_t size, size_t nmemb, void *stream){
	size_t written = fwrite(ptr, size, nmemb, (FILE *)stream);
	return written;
}

bool updateMalwareData(){
	// Download and format json
	CURL *curl_handle;
	CURLcode res;
	static const char *pagefilename = "custodia/feed.json";
  	FILE *pagefile;

  	curl_global_init(CURL_GLOBAL_ALL);
	curl_handle = curl_easy_init();

	if(curl_handle) {
		struct curl_slist *chunk = NULL;

		/* Modify a header curl otherwise adds differently */
		// TODO: Create config file to hide api key
		chunk = curl_slist_append(chunk, "accept: application/json");
		chunk = curl_slist_append(chunk, "user-agent: Falcon Sandbox");
		chunk = curl_slist_append(chunk, "api-key: xy8avvt479d075cbtp519gq69c00447f1j48ccrl94b5bf8cj74mtca8edce7bdd");

		/* set our custom set of headers */ 
		curl_easy_setopt(curl_handle, CURLOPT_HTTPHEADER, chunk);
		curl_easy_setopt(curl_handle, CURLOPT_URL, "https://www.hybrid-analysis.com/api/v2/feed/latest?_timestamp=1573429856283");

		/* Check for errors */ 
		res = curl_easy_perform(curl_handle);
		if(res != CURLE_OK){
			fprintf(stderr, RED"%s\n"RESET, curl_easy_strerror(res));
			return false;
		}

		curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, write_data);

		pagefile = fopen(pagefilename, "wb");
		if(pagefile) {
			/* write the page body to this file handle */ 
			curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, pagefile);

			/* get it! */ 
			curl_easy_perform(curl_handle);

			/* close the header file */ 
			fclose(pagefile);
		}

		/* cleanup curl stuff */ 
		curl_easy_cleanup(curl_handle);

		curl_global_cleanup();
	}

	// Variables
	FILE * jsonfp = fopen("custodia/feed.json", "r");
	if (jsonfp == NULL){
		colourText("ERROR: JSON file could not be read\nProgram terminating...\n", 'r');
		return false;
	}

	FILE * wantedfp = fopen("custodia/most-wanted.wbc", "w");
	if (wantedfp == NULL){
		colourText("ERROR: File 'most-wanted.wbc' failed to open\n", 'r');
		return false;
	}

	char line[strSize];
	const char delim[2] = ":";
	char *token;
	char badChars[] = "\",";
	struct tmp tmp;
	int currentJob = 1;
	int jobItem = 0;
	bool writeJob = false;

	while(!feof(jsonfp)){
		fgets(line, sizeof(line), jsonfp);
		token = strtok(line, delim);
		nomChars(" ", token);

		if (strcmp(token, "\"job_id\"") == 0){
			if (writeJob == true && currentJob != 1){
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
				writeJob = false;
				jobItem = 0;
			}
			fprintf(wantedfp, "#%d\n", currentJob);
			currentJob++;

		}else if(strcmp(token, "\"md5\"") == 0 && jobItem < 1){
			token = strtok(NULL, delim);
			nomChars(badChars, token);
			strcpy(tmp.md5, "");
			strcpy(tmp.md5, "MD5: ");
			strcat(tmp.md5, token);
			jobItem++;

		}else if(strcmp(token, "\"sha1\"") == 0 && jobItem < 2){
			token = strtok(NULL, delim);
			nomChars(badChars, token);
			strcpy(tmp.sha1, "");
			strcpy(tmp.sha1, "SHA1: ");
			strcat(tmp.sha1, token);
			jobItem++;

		}else if(strcmp(token, "\"sha256\"") == 0 && jobItem < 3){
			token = strtok(NULL, delim);
			nomChars(badChars, token);
			strcpy(tmp.sha256, "");
			strcpy(tmp.sha256, "SHA256: ");
			strcat(tmp.sha256, token);
			jobItem++;

		}else if(strcmp(token, "\"threat_score\"") == 0 && jobItem < 4){
			token = strtok(NULL, delim);
			nomChars(badChars, token);
			strcpy(tmp.threatScore, "");
			strcpy(tmp.threatScore, "Threat Score: ");
			strcat(tmp.threatScore, token);
			jobItem++;

		}else if(strcmp(token, "\"threat_level_human\"") == 0 && jobItem < 5){
			token = strtok(NULL, delim);
			nomChars(badChars, token);
			strcpy(tmp.threatLevel, "");
			strcpy(tmp.threatLevel, "Threat Level: ");
			strcat(tmp.threatLevel, token);
			jobItem++;
			
		}else if(strcmp(token, "\"vx_family\"") == 0 && jobItem < 6){
			token = strtok(NULL, delim);
			nomChars(badChars, token);
			strcpy(tmp.malwareFamily, "");
			strcpy(tmp.malwareFamily, "Malware Family: ");
			strcat(tmp.malwareFamily, token);
			jobItem++;
			
		}else if(strcmp(token, "\"submit_name\"") == 0 && jobItem < 7){
			token = strtok(NULL, delim);
			nomChars(badChars, token);
			strcpy(tmp.name, "");
			strcpy(tmp.name, "Name: ");
			strcat(tmp.name, token);
			jobItem++;
			
		}else if(strcmp(token, "\"size\"") == 0 && jobItem < 8){
			token = strtok(NULL, delim);
			nomChars(badChars, token);
			strcpy(tmp.size, "");
			strcpy(tmp.size, "Size: ");
			strcat(tmp.size, token);
			jobItem++;
			
		}else if(strcmp(token, "\"type\"") == 0 && jobItem < 9){
			token = strtok(NULL, delim);
			nomChars(badChars, token);
			strcpy(tmp.fileType, "");
			strcpy(tmp.fileType, "File Type: ");
			strcat(tmp.fileType, token);
			jobItem++;

		}else if(strcmp(token, "\"environment_description\"") == 0 && jobItem < 10){
			token = strtok(NULL, delim);
			nomChars(badChars, token);
			strcpy(tmp.enviroment, "");
			strcpy(tmp.enviroment, "Enviroment: ");
			strcat(tmp.enviroment, token);
			jobItem++;
			
		}else if(strcmp(token, "\"report_url\"") == 0 && jobItem < 11){
			token = strtok(NULL, delim);
			nomChars(badChars, token);
			strcpy(tmp.reportUrl, "");
			strcpy(tmp.reportUrl, "Report Url: ");
			strcat(tmp.reportUrl, token);
			jobItem++;
		}else{
			writeJob = true;
		}
	}
	fclose(jsonfp);
	fclose(wantedfp);

	return true;
}