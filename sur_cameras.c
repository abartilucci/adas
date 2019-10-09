#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include "service.h"
#include "const.h"

int main(int argc, char const *argv[]){

    	const char *PATH = argv[1];
	
	int fd;
	int timeout = 30;
	char str_log[30];	
	unsigned short data[2];	// Dati letti da urandom (hex)

	FILE* fp;
	fp = fopen(PATH, "r");
	
	if (fp == NULL){
		perror("Error opening random data file");
	}

	mkfifo(PIPE_PARK, 0666);
	fd = open(PIPE_CAMERAS, O_WRONLY);

	while(timeout > 0){
		int i;
		i = fread( data, sizeof(data), 1, fp);
		if ( i <= 0)
			break;
		for ( int k = 0; k < 8; k++) {
			sprintf(str_log, "Valore letto da random : [%x]\n", data[k]);
			writeLog(SUR_CAMERAS_LOG, str_log);
			write(fd, &data[k], 16);
		}	
		sleep(1);
		timeout--;
	} 
	fclose(fp);

}
