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
	
	printf("creato steer_by_wire\n");

	char str[10];
	char line[30];
	int fd;	
	FILE* fp;
	mkfifo(PIPE_STEER, 0666);
	fd = open(PIPE_STEER, O_RDONLY | O_NONBLOCK);

	fp = fopen(STEER_PID, "w");
	chmod(STEER_PID, 0777);
	fprintf(fp, "%d\n", getpid());
	fclose(fp);
	
	while(1){
		
		if(read(fd, str, 15) > 1){
			for(int i=0; i<4; i++){
				sprintf(line, "STO GIRANDO A %s", str);
				writeLog(STEER_LOG, line);
				sleep(1);
			}
		}else{
			writeLog(STEER_LOG, "NO ACTION\n");
			sleep(1);
		}
	
	}


}
