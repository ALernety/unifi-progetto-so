#include "common/log.h"

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

#include "common/alloc_macro.h"

static char *get_date(char *date_string, size_t *date_size);

static char *log_rbc_format_malloc(Mode mode);

void log_segment(int fd, const char *current, const char *next)
{
	size_t date_size = 30;
	const char *format_string = "[Attuale: %s], [Next: %s], %s\n";
	char log_message_length = strlen(format_string) + strlen(current) +
				  strlen(next) + date_size + 1;
	malloc_macro_def(char *, date, date_size);
	get_date(date, &date_size);
	malloc_macro_def(char *, log_message, log_message_length);

	snprintf(log_message, log_message_length, format_string, current, next,
		 date);
	int write_length = write(fd, log_message, strlen(log_message));
	if (write_length != (int)strlen(log_message)) {
		perror("log segment");
		abort();
	}
	free(date);
	free(log_message);
}

int log_create(char *log_file)
{
	int fd = open(log_file, O_CREAT | O_WRONLY, 0644);
	if (fd == -1) {
		perror("log create");
		abort();
	}
	lseek(fd, 0, SEEK_END);
	return fd;
}

void log_rbc(int log_fd, const char *train, const char *current_platform,
	     const char *request_platform, const bool permit, Mode mode)
{
	size_t date_size = 30;
	char *format_string = log_rbc_format_malloc(mode);
	size_t log_length = strlen(format_string) + strlen(train) +
			    strlen(current_platform) +
			    strlen(request_platform) +
			    /*authorize*/ 2 + date_size + 1;
	malloc_macro_def(char *, log_message, log_length);

	malloc_macro_def(char *, date, date_size);
	get_date(date, &date_size);

	snprintf(log_message, log_length, format_string, train,
		 current_platform, request_platform, permit ? "SI" : "NO",
		 date);
	int write_length = write(log_fd, log_message, strlen(log_message));
	if (write_length != (int)strlen(log_message)) {
		perror("log segment");
		abort();
	}
	free(date);
	free(format_string);
	free(log_message);
}

static char *log_rbc_format_malloc(Mode mode)
{
	char *action_type;
	switch (mode) {
	case PERMIT:
		action_type = strdup("AUTORIZZAZIONE");
		break;
	case MOVE:
		action_type = strdup("MOVIMENTO");
		break;
	default:
		perror("wrong mode");
		abort();
	}

	const char *format_string = "[TRENO RICHIEDENTE %s: %%s], "
				    "[SEGMENTO ATTUALE: %%s], "
				    "[SEGMENTO RICHIESTO: %%s], "
				    "[AUTORIZZATO: %%s], "
				    "[DATA: %%s]\n";

	size_t format_length = strlen(format_string) + strlen(action_type) + 1;
	malloc_macro_def(char *, format, format_length);

	snprintf(format, format_length, format_string, action_type);
	free(action_type);
	return format;
}

static char *get_date(char *date_string, size_t *date_size)
{
	struct tm current_time;
	time_t now;

	static char *date_format = "%d/%m/%Y; %H:%M:%S";
	now = time(NULL);
	localtime_r(&now, &current_time);
	*date_size =
		strftime(date_string, *date_size, date_format, &current_time);
	return date_string;
}
