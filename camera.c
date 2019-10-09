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


int main(int argc, char** argv){
	
	printf("creato front_W_camera\n");

	char line[15];
	FILE *fp;

	fp = fopen(CAMERA_PID, "w");
	chmod(CAMERA_PID, 0777);
	fprintf(fp, "%d\n", getpid());
	fclose(fp);

	fp = fopen(INPUT_DATA , "r");
	
	int fd;
	mkfifo(PIPE_FRONT_W_CAMERA, 0666);
	fd = open(PIPE_FRONT_W_CAMERA, O_RDWR);
	
	read(fd, line, 10); //AVVIA LETTURA DATI SOLO DOPO IL COMANDO AVVIO

	while(fgets(line, 10, fp)){
	
		writeLog(CAMERA_LOG, line);

		write(fd, line, 10);//COMUNICA CON ECU
	
		sleep(10);
	}

	fclose(fp);
	close(fd);

}

