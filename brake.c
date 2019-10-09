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

void warning_handler(int signum);
void parking_handler(int signum);

int main(int argc, char** argv){
	
	printf("creato brake_by_wire\n");

	char str[10];
	int fd;
	FILE *fp;
	int speed_delta = 0;
	mkfifo(PIPE_BRAKE, 0666);
	fd = open(PIPE_BRAKE, O_RDONLY | O_NONBLOCK);

	fp = fopen(BRAKE_PID, "w");
	chmod(BRAKE_PID, 0777);
	fprintf(fp, "%d\n", getpid());
	fclose(fp);

	signal(SIGUSR1, warning_handler);
	signal(SIGUSR2, parking_handler);

	while(1){
		
		if(read(fd, str, 15) > 1){
			printf("BRAKE:\n");
			sscanf(str, "FRENO %d", &speed_delta);
		}
		if(speed_delta > 0){
			setSpeed(getSpeed()-5);
			speed_delta = speed_delta -5;
			printf("%dKm/h\n",getSpeed());	
			writeLog(BRAKE_LOG, "DECREMENTO 5\n");
		}else
			writeLog(BRAKE_LOG, "NO ACTION\n");

		sleep(1);
	}

	close(fd);
}

void warning_handler(int signum){

	printf("\n - - - RILEVATO PERICOLO - - -\n");
	writeLog(BRAKE_LOG, "ARRESTO AUTO\n");
	setSpeed(0);
	sleep(1);

}

void parking_handler(int signum){

	writeLog(BRAKE_LOG, "ARRESTO AUTO\n");
	setSpeed(0);
	kill(getpid(), SIGSTOP);

}
