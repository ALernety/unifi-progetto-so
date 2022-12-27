#include "../REGISTRO/REGISTRO.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "../common/alloc_macro.h"
#include "../common/socket.h"
#include "../common/string_handlers.h"

int create_socket_server(char *socket_path, unsigned int port,
			 int max_connected_clients)
{
	int sfd;
	socket_data socket_input;
	socket_input.socket_path = socket_path;
	socket_input.max_connected_clients = max_connected_clients;
	socket_input.port = port;
	socket_input.sfd = &sfd;
	socket_input.user = SERVER;
	return socket_open(socket_input, AF_INET);
}

void start_socket_server(int *sfd, char **itinerary_list)
{
	while (true) {
		int client_socket = -1;
		int *client_sfd = &client_socket;
		socket_accept(sfd, client_sfd);
		if (fork() == 0) {
			char *train_name = get_malloc_train(client_sfd);
			int train_index = get_integer_from(train_name) - 1;
			send_itinerary(client_sfd, itinerary_list[train_index]);
			socket_close(client_sfd, NULL);
			exit(EXIT_SUCCESS);
		} else {
			socket_close(client_sfd, NULL);
		}
	}
}

char *get_malloc_train(int *sfd)
{
	// Read from socket
	return socket_read_malloc(sfd, "\0");
}

bool send_itinerary(int *sfd, char *itinerary)
{
	size_t message_len = socket_write(sfd, itinerary, strlen(itinerary));
	return message_len == strlen(itinerary);
}
