// Client side C/C++ program to demonstrate Socket
// programming
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#define PORT 12000

int main(int argc, char const* argv[])
{
	int sock = 0, valread, client_fd;
	struct sockaddr_in serv_addr;

	char sentence[256];
	strcpy(sentence, argv[3]);
	char modifiedSentence[1024] = { 0 };
	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		printf("\n Socket creation error \n");
		return -1;
	}

	serv_addr.sin_family = AF_INET;
	//long int port = atoi(argv[2]);
	serv_addr.sin_port = htons(atoi(argv[2]));

	// Convert IPv4 and IPv6 addresses from text to binary
	// form
	if (inet_pton(AF_INET, argv[1], &serv_addr.sin_addr)
		<= 0) {
		printf(
			"\nInvalid address/ Address not supported \n");
		return -1;
	}

	if ((client_fd
		= connect(sock, (struct sockaddr*)&serv_addr,
				sizeof(serv_addr)))
		< 0) {
		printf("\nConnection Failed \n");
		return -1;
	}
	send(sock, sentence, strlen(sentence), 0);
	printf("Modified sentence received from server:\n");
	valread = read(sock, modifiedSentence, 1024);
	printf("%s\n", modifiedSentence);

	// closing the connected socket
	sleep(60);
	close(client_fd);
	return 0;
}
