#pragma once
#include <stdbool.h>
#include <sys/socket.h>

enum SOCKET_USER { SERVER, CLIENT };
typedef enum SOCKET_USER socket_user;

/**
 * @brief All fields are mandatory except "port" on AF_UNIX socket and
 *        "max_connected_clients" with user CLIENT.
 *
 * @param sfd Is a socket file descriptor, mandatory in all cases.
 * @param user Is a socket_user ENUM that indicates usage purpose.
 * @param socket_path Is a path to socket file with AF_UNIX socket,
 *        and IP address with AF_INET socket.
 * @param max_connected_clients Is a maximum number of clients, that
 *        this host will able to serve.
 * @param port Is a port number required only by AF_INET socket.
 *        (mandatory with AF_INET)
 */
struct SOCKET_DATA {
	int *sfd;
	socket_user user;
	char *socket_path;
	int max_connected_clients;
	unsigned int port;
};
typedef struct SOCKET_DATA socket_data;

/**
 * @brief Open socket from specified "sa_family_t"
 *        and with input in "socket_input".
 *
 * @param socket_input socket_data type instance.
 * @param socket_domain Is a selector of socket domain,
 *        can be AF_INET or AF_UNIX
 * @return int Returns system file descriptor of the created socket.
 */
int socket_open(socket_data socket_input, sa_family_t socket_domain);

/**
 * @brief Accepts the connection from client.
 *
 * @param sfd File descriptor of server socket.
 * @param client_sfd File descriptor of the client socket.
 * @param allowed_num Number of errno consts in allowed_errno.
 * @param allowed_errno Array of errno consts, which should not be interpreted
 *        as an error.
 * @return int Client socket file descriptor.
 */
int socket_accept_errno(int *sfd, int *client_sfd, size_t allowed_num,
			int allowed_errno[]);

#define socket_accept(sfd, client_sfd) \
	socket_accept_errno(sfd, client_sfd, 0, NULL)

/**
 * @brief Close socket and unlink of the socket_path.
 *
 * @param sfd File descriptor of socket.
 * @param socket_path Path to the socket file, which will be unlinked. Set to
 *        NULL if "user" is CLIENT.
 * @return true Error on close.
 * @return false Closed with success.
 */
bool socket_close(int *sfd, char *socket_path);

/**
 * @brief Read message of length msg_len from socket to variable msg
 *
 * @param sfd File descriptor of socket.
 * @param msg Variable where will be stored read message.
 * @param msg_len Number of characters to read.
 * @param allowed_num Number of errno consts in allowed_errno.
 * @param allowed_errno Array of errno consts, which should not be interpreted
 *        as an error.
 * @return int Length of read message.
 */
int socket_read_length_errno(int *sfd, char *msg, ssize_t msg_len,
			     size_t allowed_num, int allowed_errno[]);

#define socket_read_length(sfd, msg, msg_len) \
	socket_read_length_errno(sfd, msh, msg_len, 0, NULL)

/**
 * @brief Read message from socket to allocated space with malloc,
 *        until end.
 *
 * @param sfd File descriptor of socket.
 * @param end End of message.
 * @param allowed_num Number of errno consts in allowed_errno.
 * @param allowed_errno Array of errno consts, which should not be interpreted
 *        as an error.
 * @return int Length of read message.
 */
char *socket_read_malloc_errno(int *sfd, const char *end, size_t allowed_num,
			       int allowed_errno[]);

#define socket_read_malloc(sfd, end) socket_read_malloc_errno(sfd, end, 0, NULL)

/**
 * @brief Write first msg_len characters from msg to socket.
 *
 * @param sfd File descriptor of socket.
 * @param msg Message to write.
 * @param msg_len Number of characters of msg to write.
 * @return int Number of characters to write without terminator.
 */
int socket_write(int *sfd, const char *msg, ssize_t msg_len);
