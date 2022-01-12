#include<stdio.h>
#include<signal.h>
#include<unistd.h>
#include<stdlib.h>
#include<string.h>
#include<sys/ioctl.h>

#define COLOR_COUNT 8
char *color_names[] = {"black", "red", "green", "yellow", "blue",
		"purple", "cyan", "white"};


void sig_exit(int sig){
	printf("\033[0m");
	exit(0);
}//sigint_exit


int print_file_chars(FILE *fp, unsigned int msc){
	unsigned int lncnt = 0;
	char c = fgetc(fp);
	while(!feof(fp)){
		if(c == '\n') ++lncnt;
		printf("%c",c);
		fflush(stdout);
		usleep(msc*1000);
		c = fgetc(fp);
	}//while
	return(lncnt);
}//print file chars


int main(int argc, const char *argv[]){
	signal(SIGINT, sig_exit);
	signal(SIGQUIT, sig_exit);
	signal(SIGTERM, sig_exit);

	struct winsize win;
	ioctl(0, TIOCGWINSZ, &win);
	int term_height = win.ws_row;

	char *file_name = "/usr/include/stdio.h";
	unsigned int delay = 50;
	char color_code = 32;
	unsigned int line_count = 0;

	int i;
	for(i=1; i<argc; i++){
		if(!strcmp(argv[i], "--help") || !strcmp(argv[i], "-h")){
			FILE *helpfile = fopen("help.txt", "r");
			print_file_chars(helpfile, 0);
			fclose(helpfile);
			exit(0);
		}
		else if(!strcmp(argv[i], "-t")){
			++i;
			delay = (unsigned int)atoi(argv[i]);
		}//if change timing
		else if(!strcmp(argv[i], "-c")){
			++i;
			int k, ccode=-1;
			for(k=0; k<COLOR_COUNT; k++){
				if(!strcmp(color_names[k], argv[i])){
					ccode = k;
				}//match color name
			}
			if(ccode == -1){
				fprintf(stderr, "error. color name not recognized.\n");
				exit(-1);
			}
			color_code = 30+ccode;	
		}//if change color
		else{
			file_name = (char*)malloc(strlen(argv[i]));
			strcpy(file_name, argv[i]);
		}
		if(i == argc) break;
	}//for

	FILE *fp = fopen(file_name, "r");

	if(fp == NULL){
		fprintf(stderr, "Error opening a given file.  Please check all arguments.  For help, use \"telecode --help\".\n");
	}


	printf("\033[0;%dm", color_code);
	while(1){
		line_count += print_file_chars(fp, delay);
		rewind(fp);
		printf("\n");
		if(line_count > term_height){
			system("clear");
			line_count = 0;
		}
	}//while
	return(0);
}//main



