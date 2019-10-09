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

void parking_handler(int signum);

int speed_delta = 0;

int main(int argc, char** argv){
	
	printf("creato throttle_control\n");
	
	char str[10];
	int fd;	
	FILE* fp;


	fp = fopen(THROTTLE_PID, "w");
	chmod(THROTTLE_PID, 0777);
	fprintf(fp, "%d\n", getpid());
	fclose(fp);

	mkfifo(PIPE_THROTTLE, 0666);
	fd = open(PIPE_THROTTLE, O_RDONLY | O_NONBLOCK);

	signal(SIGUSR2, parking_handler);

	while(1){
		
		if(read(fd, str, 15) > 1){
			printf("THROTTLE:\n");
			sscanf(str, "INCREMENTO %d", &speed_delta);
		}
		if(speed_delta > 0){
			setSpeed(getSpeed()+5);
			speed_delta = speed_delta -5;
			printf("%dKm/h\n",getSpeed());	
			writeLog(THROTTLE_LOG, "AUMENTO 5\n");
		}else
			writeLog(THROTTLE_LOG, "NO ACTION\n");

		sleep(1);
	}

	close(fd);
}

void parking_handler(int signum){
	 speed_delta = 0;
	
}
