#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include "service.h"
#include "const.h"

int main(int argc, char** argv){

	char* mode = argv[1];

	int fd_output, fd_input;
	
	
	mkfifo(PIPE_OUTPUT, 0666);
	fd_output = open(PIPE_OUTPUT, O_WRONLY);

	write(fd_output, "Avvio", 70);
	
	mkfifo(PIPE_INPUT, 0666);
	fd_input = open(PIPE_INPUT, O_WRONLY);
	
	write(fd_input, mode, 5);
	
	if ( mode == "0")
		mode = "NORMALE";
	else
		mode = "ARTIFICIALE";

	printf("Attivata modalità %s\n\n", mode);
	printf("			- HMI input -\n\n");

	printf("Digitare un comando:\n - INIZIO\n - PARCHEGGIO\n\n");
	
	char command[10];
	int end=0;

	while(end == 0){	
		printf(" - ");
		scanf("%s",command);
		if(strcmp("INIZIO", command) == 0){
			write(fd_input, command, 15);
		}else if(strcmp("PARCHEGGIO", command) == 0){
			write(fd_input, command, 15);
			end=1;
		}else 
			printf("\nErrore: digitato comando errato. Riprovare.\n");

	}	
	
	printf("\n		 	- - - - - -\n\n");

	close(fd_output);
	close(fd_input);
	
}
