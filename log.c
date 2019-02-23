
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>

#include "log.h"

#ifndef LOG
#define LOG false
#endif

log_type _log_type;
char _log_filename[128];

char* time_string() {
  time_t rawtime;
  time(&rawtime);
  struct tm t = *gmtime(&rawtime);
  char* timestr = malloc(20 * sizeof(char));
  sprintf(timestr, "%i-%02i-%02i %02i:%02i:%02i",t.tm_year + 1900, t.tm_mon+1, t.tm_mday, t.tm_hour, t.tm_min, t.tm_sec);
  return timestr;
}

void append_file(char* message) {
  if(LOG) {
    FILE* f = fopen(_log_filename, "aw");
    fwrite(message, sizeof(char), strlen(message), f);
    fwrite("\n", sizeof(char), strlen("\n"), f);
    fclose(f);
  }
}

void append_stdout(char* message) {
  if(LOG)
    printf("%s\n", message);
}

void init_log(log_type type) {
  _log_type = type;
}

void init_file_log(char* filename) {
  init_log(file);
  if(filename == NULL) {
    strcpy(_log_filename, DEFAULT_LOG_FILE);
  }
  else {
    strcpy(_log_filename, filename);
  }
}

void info(char* message) {
  char *msg = malloc(sizeof(char) * (strlen(message) + 31));
  sprintf(msg, "%s [info]    %s", time_string(), message);
  if(_log_type == file) {
    append_file(msg);
  }
  else {
    append_stdout(msg);
  }
}

void warning(char* message) {
  char *msg = malloc(sizeof(char) * (strlen(message) + 31));
  sprintf(msg, "%s [warning] %s", time_string(), message);
  if(_log_type == file) {
    append_file(msg);
  }
  else {
    append_stdout(msg);
  }
}

void error(char* message) {
  char *msg = malloc(sizeof(char) * (strlen(message) + 31));
  sprintf(msg, "%s [error]   %s", time_string(), message);
  if(_log_type == file) {
    append_file(msg);
  }
  else {
    append_stdout(msg);
  }
}



