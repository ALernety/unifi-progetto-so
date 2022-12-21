
 
/**
 * @brief Iterates through all segments that make up the itinerary  
 *        and provides a log file with detailed access informations.            
 * 
 * @param itinerary_list Array of strings obtained by splitting the itinerary  
 * @param log_fd Log file descriptor
 */
void traverse_itinerary(char **itinerary_list, int log_fd);

/**
*@brief Create a client-side AF_INET socket 
*
*@param socket_path IP address 
*@param port_string Server socket port  
*@return int Socket file descriptor  
*/
int create_socket_client(char *socket_path, char* port_string);

/**
 * @brief Get the itinerary needed for the trains via AF-INET socket   
 * 
 * @param sfd Socket file descriptor 
 * @param train_name Name of train
 * @param itinerary_name Name of itinerary
 * @return char* String containing the itinerary
 */
char* get_itinerary(int sfd,char* train_name,char* itinerary_name);

