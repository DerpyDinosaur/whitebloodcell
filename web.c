#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>

#include "tools.h"

void checkWebsite(){
	clearScreen();
	char url[255];
	puts("-------------------------");
	puts("---- Website checker ----");
	puts("-------------------------");

	getInput("Enter a URL you would like to check.\n> ", url, sizeof(url));
	printf("%s\n", url);
}