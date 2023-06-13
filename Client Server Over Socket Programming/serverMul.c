// Server side C/C++ program to demonstrate Socket
// programming
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <ctype.h>
#include <pthread.h>
#define PORT 12000

void *connection_handler(void *);
static struct sockaddr_in client_address;
static int client_address_len = sizeof(client_address);

int main(int argc, char const* argv[])
{
	int server_fd, new_socket, valread, *new_sock;
	struct sockaddr_in address;
	
	int opt = 1;
	int addrlen = sizeof(address);
	pthread_t sniffer_thread;

	// Creating socket file descriptor
	if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		perror("socket failed");
		exit(EXIT_FAILURE);
	}

	// Forcefully attaching socket to the port 12000
	if (setsockopt(server_fd, SOL_SOCKET,
				SO_REUSEADDR, &opt,
				sizeof(opt))) {
		perror("setsockopt");
		exit(EXIT_FAILURE);
	}
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	long int port = atoi(argv[1]);
	address.sin_port = htons(port);

	// Forcefully attaching socket to the port 12000
	if (bind(server_fd, (struct sockaddr*)&address,
			sizeof(address))
		< 0) {
		perror("bind failed");
		exit(EXIT_FAILURE);
	}
	if (listen(server_fd, 3) < 0) {
		perror("listen");
		exit(EXIT_FAILURE);
	}
	printf("The server is ready to receive\n");
	while (1){
		if ((new_socket
			= accept(server_fd, (struct sockaddr*) &client_address, &client_address_len)) < 0){
			perror("accept");
			exit(EXIT_FAILURE);
		}

		
		if( pthread_create( &sniffer_thread , NULL ,  connection_handler , (void*) &new_socket) < 0)
		{
			perror("could not create thread");
			return 1;
		}
		
		//Now join the thread , so that we dont terminate before the thread
		pthread_join( sniffer_thread , NULL);
		//puts("Handler assigned");


		// closing the connected socket
		close(new_socket);
	}
	// closing the listening socket
	shutdown(server_fd, SHUT_RDWR);
	return 0;
}



void *connection_handler(void *socket_desc)
{
	//Get the socket descriptor
	int sock = *(int*)socket_desc;
	int read_size;
	char *message , client_message[1024] = { 0 };
	
	//Send some messages to the client
	//message = "Greetings! I am your connection handler\n";
	//send(sock , message , strlen(message), 0);
	
	
	//Receive a message from client
	//char client_message[1024] = { 0 };
	//printf("%d\n",sock);
	read_size = read(sock , client_message , 1024);
	
	//char client_message[1024] = { 0 };

	printf("Recevied sentence: [%s:%u]\n", inet_ntoa(client_address.sin_addr),                                               ntohs(client_address.sin_port));
	printf("%s\n", client_message);
	char modifiedSentence[1024] = { 0 };
	int j = 0;
	char ch;
	while (client_message[j]){
		ch = client_message[j];
		modifiedSentence[j] = toupper(ch);
		j++;
	}
	printf("Modified sentence:\n");
	printf("%s\n", modifiedSentence);
	send(sock , modifiedSentence, strlen(modifiedSentence), 0);

	close(sock);

}