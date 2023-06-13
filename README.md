# Web-Server-Over-Sockets
This repository develops client-server socket programming and builds a concurrent Web server over HTTP in C.
# Client-Server and Web Server

This project consists of two components: a client-server application using socket programming and a web server that handles HTTP requests and serves requested files.

## Client-Server

The client-server application allows communication between a client and a server using sockets. It follows a simple request-response model, where the client sends a request to the server, and the server processes the request and sends back a response. The communication between the client and server is established over a network using socket programming.

To run the client-server application:

1. Start the server application by running the server program.
2. Start the client application by running the client program.
3. The client can send requests to the server by specifying the desired operation or data.
4. The server processes the requests and sends back the appropriate response to the client.
5. The client displays the received response.

Make sure to customize and modify the client and server programs to suit your specific use case or desired functionality. The server handles multiple clients concurrently using pthreads.

## Web Server

The web server component is responsible for handling HTTP requests and serving requested files to clients. It acts as a basic HTTP server that can process GET requests and return the requested files as HTTP responses.

To run the web server:

1. Start the web server program by running the server script.
2. The server listens for incoming HTTP requests on a specified port.
3. Clients can send GET requests to the server, specifying the desired file in the request.
4. The server reads the requested file from the server's file system.
5. If the file exists, the server sends an HTTP response with the file content as the response body.
6. If the file does not exist or there is an error, the server sends an appropriate HTTP error response.

You can connect to the server from a web browser with the URL of http://<machine name>:<port number>/<html
file name>. For example, you can try http://localhost:8888/text.html or
http://server-ip-address:8888/text.html if your web server uses the port number of 8888 and
has a file called text.html in its root folder (www) for all the web contents. The server handles all types of bad requests.

Feel free to customize the web server code to handle additional HTTP methods, implement more advanced features, or add security measures as per your requirements.



## Usage
To run the client code in the client-server application use:
./client [ip_server_addr] [port#] [message1]
./client [ip_server_addr] [port#] [message2]
To run the server code use the following commands:
./server [port#]




