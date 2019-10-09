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

	sleep(1);
	
	printf("\n			- HMI output -\n\n\n\n");
	
	FILE* fp;

	fp = fopen(HMI_OUTPUT_PID, "w");
	chmod(HMI_OUTPUT_PID, 0777);
	fprintf(fp, "%d\n", getpid());
	fclose(fp);

	int fd;
	char str[70];

	mkfifo(PIPE_OUTPUT, 0666);
	
	while(1){
		fd = open(PIPE_OUTPUT, O_RDONLY); //bloccante
		read(fd, str, 70);
		printf("%s\n", str);
		close(fd);
	}
	
	
}

