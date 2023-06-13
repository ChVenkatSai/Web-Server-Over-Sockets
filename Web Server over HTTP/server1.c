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
#define BUFFER_SIZE 1024

void *connection_handler(void *);

static struct sockaddr_in client_addr;
static int client_addrlen = sizeof(client_addr);

int main(int argc, char const* argv[])
{
	int server_fd, new_socket, valread;
	struct sockaddr_in address;
	int opt = 1;
	int addrlen = sizeof(address);
	pthread_t sniffer_thread;
	
	char buffer[BUFFER_SIZE];
    	char resp[] = "HTTP/1.0 200 OK\r\n"
        	          "Server: webserver-c\r\n"
                	  "Content-type: text/html\r\n\r\n"
                  	"<html>hello, world</html>\r\n";
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
			= accept(server_fd, (struct sockaddr*)&client_addr,&client_addrlen)) < 0){
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
		char buffer[1024] = { 0 };
		
		printf("%s\n",buffer);
		char reply[40000] = { 0 };
		int sock = *(int*)socket_desc;
		int read_size;
		read_size = read(sock, buffer, 1024);
		if(read_size == 0)
		{
			puts("Client disconnected");
			fflush(stdout);
			goto h;
		}
		else if(read_size == -1)
		{
			perror("rv failed");
			goto h;
		}
		// Read the request
        	char method[1024], uri[1024], version[1024];
        	sscanf(buffer, "%s %s %s", method, uri, version);
        	printf("message from client: [%s:%u]\n%s %s %s\n", inet_ntoa(client_addr.sin_addr),
               		ntohs(client_addr.sin_port), method, uri, version);	

		

		if (strcmp(version, "HTTP/1.1")) 
		{strcpy(reply, "HTTP/1.1 505 HTTP version not supported\r\n");
		send(sock, reply, strlen(reply), 0);
		printf("message to client: [%s:%u]\n", inet_ntoa(client_addr.sin_addr),
               	ntohs(client_addr.sin_port));
		printf("HTTP/1.1 505 HTTP version not supported\r\n");
		goto mark;
		}
		if (strlen(uri)>30)
		{strcpy(reply, "HTTP/1.1 400 bad url\r\n");
		send(sock, reply, strlen(reply), 0);
		printf("message to client: [%s:%u]\n", inet_ntoa(client_addr.sin_addr),
               	ntohs(client_addr.sin_port));
		printf("HTTP/1.1 400 bad url\r\n");
		goto mark;
		}

		char filename[1024] = {0};
;
		strncpy(filename, uri + 1, strlen(uri) - 1);
		filename[strlen(uri) - 1] = '\0';


	
		if (filename)
		{

			FILE *fp = fopen(filename,"rb");

			char *buff = 0;
			long length;
			//FILE * f = fopen (filename, "rb");
	
			if (fp)
			{
			  fseek (fp, 0, SEEK_END);
			  length = ftell (fp);
			  fseek (fp, 0, SEEK_SET);		
			  buff = malloc (length);
			  if (buff)
			  {
			    fread (buff, 1, length, fp);
			  }
			  fclose (fp);
			}
			else 
			{
				strcpy(reply, "HTTP/1.0 404 Not Found\r\n"
					"Server: webserver-c\r\n");
				send(sock, reply, strlen(reply), 0);
				printf("message to client: [%s:%u]\n", inet_ntoa(client_addr.sin_addr),
               		ntohs(client_addr.sin_port));
				printf("HTTP/1.1 404 Not Found\r\n");
				goto mark;
			}

			if (!strcmp((char *)&filename[strlen(filename)-1],"l")){
				strcpy(reply, "HTTP/1.1 200 OK\r\n"
        					"Server: webserver-c\r\n"
                			"Content-type: text/html\r\n\r\n");
			}
			else if (!strcmp((char *)&filename[strlen(filename)-1],"g")){
				strcpy(reply, "HTTP/1.1 200 OK\r\n"
        					"Server: webserver-c\r\n"
                			"Content-type: image/jpeg\r\n\r\n");
				send(sock, reply, strlen(reply), 0);
				send(sock, buff, length, 0);
				goto dark;
			}
			strcat(reply, buff);
	

		}


		send(sock, reply, strlen(reply), 0);
		dark:
        printf("message to client: [%s:%u]\n", inet_ntoa(client_addr.sin_addr),
               	ntohs(client_addr.sin_port));	
		printf("HTTP/1.1 200 OK\r\n");
		mark:

			
		//Free the socket pointer
		h:close(sock);


}
