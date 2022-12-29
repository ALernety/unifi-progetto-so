#include <arpa/inet.h>
#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "REGISTRO/REGISTRO.h"
#include "common/alloc_macro.h"
#include "common/parent_dir.h"
#include "common/string_handlers.h"

static void sigusr2_handler(int sig);

int main(int argc, char const *argv[])
{
	const char *format_string =
		"\033[31mNot enough arguments! Example of use:\033[0m\n"
		"\n"
		"Usage: %s <MAP> <MAP_DELIMITER> <AF_INET_ADDRESS> <AF_INET_PORT>\n\n"
		"\033[36m<MAP>\033[0m possible values are:\n"
		"    \033[36mpath/to/map\033[0m - manage trains with map two\n"
		"\n"
		"\033[36m<MAP_DELIMITER>\033[0m possible values are:\n"
		"    \033[36m\\n\033[0m          - Is default value\n"
		"\n"
		"\033[36m<AF_INET_ADDRESS>\033[0m possible values are:\n"
		"    \033[36m127.0.0.1\033[0m   - Is default value\n"
		"\n"
		"\033[36m<AF_INET_PORT>\033[0m possible values are:\n"
		"    \033[36m43210\033[0m       - Is default value\n";
	size_t help_length = strlen(format_string) + strlen(argv[0]) + 1;
	malloc_macro_def(char *, help_str, help_length);
	snprintf(help_str, help_length, format_string, argv[0]);

	const char *map_name;
	const char *map_delimiter = "\n";
	char *ip_address = strdup("127.0.0.1");
	int port = 43210;

	switch (argc) {
	case 5:
		port = atoi(argv[4]);
		__attribute__((fallthrough));
	case 4:
		ip_address = strdup(argv[3]);
		__attribute__((fallthrough));
	case 3:
		map_delimiter = argv[2];
		__attribute__((fallthrough));
	case 2: {
		map_name = argv[1];
		// MAP must be set
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
		if (access(map_name, R_OK) == -1) {
			printf("\033[31mMap file not found or can't read!\033[0m\n");
			printf("%s", help_str);
			exit(EXIT_FAILURE);
		}
		break;
	}
	default:
		printf("%s", help_str);
		exit(EXIT_FAILURE);
	}

	signal(SIGUSR2, sigusr2_handler);

	free(help_str);

	char *map = get_malloc_string_from(map_name);
	char **itinerary_list = get_malloc_token_list(map, map_delimiter);
	int sfd = create_socket_server(ip_address, port, 3);
	// Remove timeout for socket read
	struct timeval timeout;
	timeout.tv_sec = 0;
	timeout.tv_usec = 0;
	if (setsockopt(sfd, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof timeout) <
	    0) {
		perror("socket timeout receive");
		abort();
	}
	printf("Started REGISTRO, to stop it send interrupt signal (CTRL-C)\n");
	start_socket_server(&sfd, itinerary_list);
	free(ip_address);

	return EXIT_SUCCESS;
}

static void sigusr2_handler(int sig)
{
	printf("Terminate REGISTRO with signal %d.\n", sig);
	exit(EXIT_SUCCESS);
	return;
}
