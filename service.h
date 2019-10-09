#ifndef SERVICE_H
#define SERVICE_H

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <time.h>


void writeLog(const char *FILEPATH, char *info);
char* getDateTime();
int getSpeed();
void setSpeed(int speed);


#endif
