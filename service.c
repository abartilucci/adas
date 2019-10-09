#include <service.h>

void writeLog(const char *FILEPATH, char *info){
	FILE* fp;
	char str[15];
	fp= fopen(FILEPATH,"a+");

	fputs(getDateTime(), fp);
	fputs(info, fp);
	fclose(fp);
}


char* getDateTime(){
	time_t t = time(NULL);
	struct tm tm = *localtime(&t);
	static char datetime[50];
	if(tm.tm_sec<10)
		sprintf(datetime, "%d-%d-%d %d:%d:0%d     ", tm.tm_mday, tm.tm_mon +1, tm.tm_year+1900, tm.tm_hour, tm.tm_min, tm.tm_sec);
	else
		sprintf(datetime, "%d-%d-%d %d:%d:%d     ", tm.tm_mday, tm.tm_mon +1, tm.tm_year+1900, tm.tm_hour, tm.tm_min, tm.tm_sec);	
	return datetime;
}


int getSpeed(){
	FILE* fp;
	int speed;
	fp= fopen("../data/speed.data","r");
	fscanf(fp, "%d", &speed);
	fclose(fp);
	return speed;
}

void setSpeed(int speed){
	FILE* fp;
	fp= fopen("../data/speed.data","w");
	fprintf(fp, "%d", speed);
	fclose(fp);
}



