#include "PADRE_TRENI/PADRE_TRENI.h"

#include <fcntl.h>
#include <linux/limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

#include "PADRE_TRENI/TRENO.h"
#include "common/alloc_macro.h"
#include "common/log.h"
#include "common/string_handlers.h"

static int file_create(char *filename);

int process_create()
{
	int pid = fork();
	if (pid < 0) {
		perror("Error creating a child process.");
		exit(EXIT_FAILURE);
	}
	return pid;
}

static int file_create(char *filename)
{
	int fd;
	if ((fd = open(filename, O_RDWR | O_CREAT | O_TRUNC, 0666)) < 0) {
		char err_string[30 + PATH_MAX];
		snprintf(err_string, sizeof(err_string),
			 "Error opening file %s", filename);
		perror(err_string);
		exit(EXIT_FAILURE);
	}
	return fd;
}

void segment_create(char *file, int seg_number)
{
	snprintf(file, PATH_MAX, "tmp/MA%d", seg_number + 1);
	int segment = file_create(file);
	file_write(segment, "0", 1);
	close(segment);
}

int file_write(int fd, const char *msg, ssize_t msg_len)
{
	int bytes_written = write(fd, msg, msg_len);
	if (bytes_written != msg_len) {
		perror("Error writing into a file.");
		exit(EXIT_FAILURE);
	}
	return bytes_written;
}

int64_t file_length(int fd)
{
	return lseek(fd, 0, SEEK_END);
}

void create_train_process(size_t train_index, char *REGISTRO_ip,
			  size_t REGISTRO_port, char *RBC_socket_file,
			  const char *itinerary_delim,
			  const char *request_delim)
{
	int parent_pid = getpid();
	int pid = fork();
	if (pid < 0) {
		perror("Error creating a child process.");
		abort();
	} else if (pid != 0) {
		// Return to parent process.
		return;
	}
	int train_name_size = snprintf(NULL, 0, "T%zu", train_index) + 1;
	malloc_macro_def(char *, train_name, train_name_size);
	snprintf(train_name, train_name_size, "T%zu", train_index + 1);
	char log_file[PATH_MAX];
	snprintf(log_file, PATH_MAX, "log/%s.log", train_name);

	int sfd = create_socket_client(REGISTRO_ip, REGISTRO_port);
	char *itinerary = get_itinerary(sfd, train_name);
	if (strcmp(itinerary, "")) {
		int log_fd = log_create(log_file);
		// Split the itinerary to get list of segments and stations
		char **itinerary_list =
			get_malloc_token_list(itinerary, itinerary_delim);
		traverse_itinerary(itinerary_list, log_fd, RBC_socket_file,
				   train_name, request_delim);
	}
	// Train already on last station, so we communicate to PADRE_TRENI.
	kill(parent_pid, SIGUSR1);
	exit(EXIT_SUCCESS);
}
