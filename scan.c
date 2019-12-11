#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>

#include "tools.h"
#include "feed.h"

void hash(){
	/* 
		TODO: Write your own hashing algorithm
		https://www.ietf.org/rfc/rfc1321.txt
	*/

	// execl("/usr/bin/md5sum", "md5sum", "custodia/ascii.wbc", NULL);
	// find -type f \( -not -name "md5sum.txt" \) -exec md5sum '{}' \; > md5sum.txt
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