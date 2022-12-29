#include <arpa/inet.h>
#include <fcntl.h>
#include <linux/limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include "PADRE_TRENI/PADRE_TRENI.h"
#include "common/alloc_macro.h"
#include "common/parent_dir.h"

#define SEGMENTS_NUM 16

static void sigusr1_handler(int signo, siginfo_t *info, void *extra);
size_t trains_arrived = 0;

int main(int argc, char *argv[])
{
	const char *format_string =
		"\033[31mNot enough arguments! Example of use:\033[0m\n"
		"\n"
		"Usage: %s <TRAIN_NUMBER> <AF_UNIX_SOCKET> <SIGUSR2_PARENT> "
		"<AF_INET_ADDRESS> <AF_INET_PORT> <REQUEST_DELIMITER> "
		"<ITINERARY_DELIMITER>\n\n"
		"\033[36m<TRAIN_NUMBER>\033[0m possible values are:\n"
		"    \033[36m[number]\033[0m       - Create number times train "
		"process. Default is 0.\n"
		"\n"
		"\033[36m<AF_UNIX_SOCKET>\033[0m possible values are:\n"
		"    \033[36mtmp/rbc\033[0m        - Path to RBC socket. To use "
		"without RBC set to empty string (default).\n"
		"\n"
		"\033[36m<SIGUSR2_PARENT>\033[0m possible values are:\n"
		"    \033[36m0\033[0m              - Don't send signal SIGUSR2 to "
		"parent process.\n"
		"    \033[36m1\033[0m              - Send signal SIGUSR2 to parent "
		"process.\n"
		"\n"
		"\033[36m<AF_INET_ADDRESS>\033[0m possible values are:\n"
		"    \033[36m127.0.0.1\033[0m      - Is default value\n"
		"\n"
		"\033[36m<AF_INET_PORT>\033[0m possible values are:\n"
		"    \033[36m43210\033[0m          - Is default value\n"
		"\n"
		"\033[36m<REQUEST_DELIMITER>\033[0m possible values are:\n"
		"    \033[36m,\033[0m              - Is default value\n"
		"\n"
		"\033[36m<ITINERARY_DELIMITER>\033[0m possible values are:\n"
		"    \033[36m', '\033[0m           - Is default value\n";
	size_t help_length = strlen(format_string) + strlen(argv[0]) + 1;
	malloc_macro_def(char *, help_str, help_length);
	size_t trains_number = 0;
	char *RBC_socket_file = strdup("");
	bool sigusr2_parent = false;
	char *ip_address = strdup("127.0.0.1");
	size_t port = 43210;
	const char *request_delim = ",";
	const char *itinerary_delim = ", ";

	snprintf(help_str, help_length, format_string, argv[0]);

	switch (argc) {
	case 8:
		itinerary_delim = argv[7];
		__attribute__((fallthrough));
	case 7:
		request_delim = argv[6];
		__attribute__((fallthrough));
	case 6:
		port = atoi(argv[5]);
		__attribute__((fallthrough));
	case 5:
		ip_address = strdup(argv[4]);
		__attribute__((fallthrough));
	case 4:
		sigusr2_parent = atoi(argv[3]);
		__attribute__((fallthrough));
	case 3:
		RBC_socket_file = strdup(argv[2]);
		__attribute__((fallthrough));
	case 2: {
		trains_number = atoi(argv[1]);
		struct sockaddr_in sa;
		bool is_wrong_ip =
			inet_pton(AF_INET, ip_address, &(sa.sin_addr)) == 0;
		if (is_wrong_ip) {
			printf("\033[31mWrong ip!\033[0m\n");
			printf("%s", help_str);
			exit(EXIT_FAILURE);
		}
		if (port == 0) {
			printf("\033[31mWrong port!\033[0m\n");
			printf("%s", help_str);
			exit(EXIT_FAILURE);
		}
		bool is_rbc = strcmp(RBC_socket_file, "") &&
			      access(RBC_socket_file, F_OK) == -1;
		if (is_rbc) {
			printf("\033[31mRBC socket file not reachable!\033[0m\n");
			usleep(1000000);
			if (access(RBC_socket_file, F_OK) == -1) {
				printf("\033[31mRBC socket file not found!\033[0m\n");
				printf("%s", help_str);
				exit(EXIT_FAILURE);
			}
		}
		break;
	}
	default:
		printf("%s", help_str);
		exit(EXIT_FAILURE);
	}
	free(help_str);

	struct sigaction action;
	action.sa_flags = SA_SIGINFO;
	action.sa_sigaction = sigusr1_handler;
	sigemptyset(&action.sa_mask);
	sigaction(SIGUSR1, &action, NULL);

	umask(000);
	// Get the size of the formatted string to allocate needed memory
	char file[PATH_MAX];
	for (int i = 0; i < SEGMENTS_NUM; i++) {
		segment_create(file, i);
	}
	// create and execute train processes
	for (size_t train_index = 0; train_index < trains_number;
	     train_index++) {
		create_train_process(train_index, ip_address, port,
				     RBC_socket_file, itinerary_delim,
				     request_delim);
	}
	while (trains_arrived < trains_number) {
		// Use pause instead of wait to not wait until child die
		pause();
	}
	// All trains are terminated, so PADRE_TRENI can be terminated.
	if (sigusr2_parent) {
		kill(getppid(), SIGUSR2);
	}
	printf("Terminate PADRE_TRENI.\n");
	free(RBC_socket_file);
	free(ip_address);
	exit(EXIT_SUCCESS);
}

static void sigusr1_handler(int signo, siginfo_t *info, void *extra)
{
	(void)signo, (void)extra;
	printf("TRENO number %d at the last station.\n",
	       info->si_value.sival_int);
	trains_arrived++;
	return;
}
