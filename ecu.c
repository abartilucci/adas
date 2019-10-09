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


void inizio();
pid_t getPid(const char *name);
void inizio(int fd_output,int fd_camera);

int main(int argc, char** argv){

	printf("creato ECU\n");
	setSpeed(0);
	
	int fd_output, fd_input, fd_camera, fd_park, fd_sur_cameras;
	char str[10];
	int mode;
	int end=0;
	int first=1;
	int parked = 1;
	int park[6] = {0x172A, 0xD639, 0x0000, 0xBDD8, 0xFAEE, 0x4300}; // Valori significativi di parcheggio
	
	FILE *park_pipe;
	FILE *sur_cameras;
	
	mkfifo(PIPE_OUTPUT, 0666);
	fd_output = open(PIPE_OUTPUT, O_WRONLY);

	mkfifo(PIPE_INPUT, 0666);
	fd_input = open(PIPE_INPUT, O_RDONLY);

	mkfifo(PIPE_FRONT_W_CAMERA, 0666);
	fd_camera = open(PIPE_FRONT_W_CAMERA, O_RDWR);
	
	read(fd_input, str, 5);
	sscanf(str, "%d", &mode);

	while(1){		
		read(fd_input, str, 15);
		if(strcmp("INIZIO", str) == 0){
		
			if(first == 1){
				printf("\nINIZIO\n\n");
				write(fd_camera, "start", 10); //avvia lettura dati input
				first = 0;
			}else
				kill(getPid(CAMERA_PID), SIGCONT);

			if( fork() == 0 ) {
				inizio(fd_output,fd_camera);
			}

		}else {

			kill(getPid(INIZIO_PID), SIGSTOP);

			kill(getPid(CAMERA_PID), SIGSTOP);	
			kill(getPid(STEER_PID), SIGSTOP);

			kill(getPid(THROTTLE_PID), SIGUSR2);
			kill(getPid(BRAKE_PID), SIGUSR2);

			writeLog(ECU_LOG, "PARCHEGGIO AVVIATO\n");
			printf("\nPARCHEGGIO AVVIATO\n");	
				
			while (parked != 0) {
				printf("\ncreati park_assist e surround_view_cameras\n");

				if ( mode == 0 ) {
					printf("in modalita normale\n\n");
					park_pipe = popen (U_PATH_P, "r");
					sur_cameras = popen (U_PATH_C, "r");
				} else {
					printf("in modalita artificiale\n\n");
					park_pipe = popen (U_BIN_P, "r");
					sur_cameras = popen (U_BIN_C, "r");
				}

				mkfifo(PIPE_PARK, 0666);
				fd_park = open(PIPE_PARK, O_RDONLY);

				mkfifo(PIPE_CAMERAS, 0666);
				fd_sur_cameras = open(PIPE_CAMERAS, O_RDONLY);
				
				unsigned short hex_p;
				unsigned short hex_c;
				ssize_t n;
				ssize_t t;

				while ( (n = read(fd_park, &hex_p, 4)) && (t = read(fd_sur_cameras, &hex_c, 16)) ) {
					//printf("park assist ultrasonic -> %x | ", hex_p);	
					//printf("surround view camera -> %x \n", hex_c);	
					for ( int i = 0; i < 6; i++ ) {
						if ( hex_p == park[i] || hex_c == park[i] ) {
							printf("\n Errore : nuovo tentativo di parcheggio tra 5 secondi.\n\n");
							writeLog(ECU_LOG, "ERRORE PARCHEGGIO\n");
							sleep(3);
							parked = 1;
						}
					}
				//printf("Return value di read di park (%ld) e di surround cameras (%ld) \n\n", n , t );
				}
				parked = 0;
			}

			writeLog(ECU_LOG, "PARCHEGGIO COMPLETATO\n"); 
			printf("\nPARCHEGGIO COMPLETATO\n\n");
			pclose(park_pipe);
			pclose(sur_cameras);

			//printf("\n ECU terminata.\n\n");
			
			kill(getPid(INIZIO_PID), SIGTERM);
			kill(getPid(CAMERA_PID), SIGTERM);	
			kill(getPid(STEER_PID), SIGTERM);
			kill(getPid(THROTTLE_PID), SIGTERM);
			kill(getPid(BRAKE_PID), SIGTERM);
			kill(getPid(HMI_OUTPUT_PID), SIGTERM);
			kill(getpid(),SIGTERM);
		} 
		
	} 

}	

void inizio(int fd_output, int fd_camera){

	int fd_throttle, fd_brake, fd_steer;
	FILE* fp;

	mkfifo(PIPE_THROTTLE, 0666);
	fd_throttle = open(PIPE_THROTTLE, O_WRONLY);
	mkfifo(PIPE_BRAKE, 0666);
	fd_brake = open(PIPE_BRAKE, O_WRONLY);
	mkfifo(PIPE_STEER, 0666);
	fd_steer = open(PIPE_STEER, O_WRONLY);

	fp = fopen(INIZIO_PID, "w");
	chmod(INIZIO_PID, 0777);
	fprintf(fp, "%d\n", getpid());
	fclose(fp);

	while(1){	//TODO si distinguono 3 casi
		
		int log = 1;
		char input[10];
		char msg[15];
		char line[70];
		char str [50];
		char P[2] = "P";		
		
		read(fd_camera, input, 10);
		
		int speed_act = getSpeed();
		int speed_req;
		if(sscanf(input, "%d", &speed_req)){			//1) L'INPUT È UNA VELOCITÀ
			
			sprintf(str, "\nVelocità: Attuale %dKm/h | Richiesta %dKm/h ->", speed_act, speed_req);	
		
			int speed_delta;
			if(speed_req > speed_act){	
				speed_delta = speed_req - speed_act;
				sprintf(msg, "INCREMENTO %d\n", speed_delta);
				write(fd_throttle, msg, 15);// comunica a throttle
			}else if(speed_act > speed_req){
				speed_delta = speed_act - speed_req;
				sprintf(msg, "FRENO %d\n", speed_delta);
				write(fd_brake, msg, 15);// comunica a brake
			}
			else{
				log=0;
				strcpy(msg, "OK\n");
			}

			sprintf(line, "%s %s\n", str, msg);
		
		}else if(input[0] == P[0]){				//2) L'INPUT È IL SEGNALE DI PERICOLO

			sprintf(msg, "%s", input);

			kill(getPid(BRAKE_PID), SIGUSR1); //TODO KILL to brake
			kill(getPid(CAMERA_PID), SIGSTOP);			

			setSpeed(0);
			writeLog(ECU_LOG, msg);
			exit(0);

		}else if(strcmp("DESTRA", input)){			//3) L'INPUT È UNA DIREZIONE

			sprintf(line, "\nDirezione -> %s\n", input);
			write(fd_steer, input, 15);
			sprintf(msg, "%s", input);

		}else if(strcmp("SINISTRA", input)){

			sprintf(line, "\nDirezione -> %s\n", input);
			write(fd_steer, input, 15);
			sprintf(msg, "%s", input);

		}
		
		if(log)
			writeLog(ECU_LOG, msg);

		write(fd_output, line, 70);
	}

}


pid_t getPid(const char *name){
	FILE *fp;
	fp = fopen(name, "r");
	pid_t pid;
	fscanf(fp, "%d", &pid);
	fclose(fp);
	return pid;
}


