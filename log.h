#ifndef LOG_H
#define LOG_H


#define DEFAULT_LOG_FILE "log.txt"

typedef enum e_log_type {
			 file,
			 std_out
} log_type;

void init_log(log_type);

void init_file_log(char*);

void info(char* message);

void warning(char* message);

void error(char* message);

#endif
