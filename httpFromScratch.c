#include <stdio.h>
#import <sys/socket.h>
#include <sys/types.h>
#import <ifaddrs.h>
#include <netdb.h>
#import <SystemConfiguration/SCNetworkReachability.h>

int main(int argc, char const *argv[])  {

	//First step:
	//Create socket on port 8080, have it listen
	//Steps for this are initialize socket, bind it to port 8080, and beging listening for incoming signals
	//once a signal has been recieved, allow for new sockets to bind to new clients

	//initialize variables 
	int server_file_descriptor, new_socket; long socket_read;

	//create sockaddr_in struct
	struct sockaddr_in address;

	//get size of addres to use for bind() and accept()
	int addresslen = sizeof(address);

	//example message
	char* example_message = "HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: 12\n\nHello world!";

	//initialize socket, name it to file descriptor server_fd
	server_file_descriptor = socket(AF_INET, SOCK_STREAM,0);

	//check if socket was opened - if fd is -1, it was not
	if (server_file_descriptor < 0) {
		perror("socket creation failed exiting.");
		return 0;
	}

	//now to bind the socket to TCP port 80

	//clear the structure
	memset((char *)&address, 0, sizeof(address));

	//use port 8080
	const int PORT = 8080;

	//use AF_INET, same network family used for the port
	address.sin_family = AF_INET;
	//use INADDRR_ANY as the address, so the os can use the address it wants
	//use htonl to convert to an address representation
	address.sin_addr.s_addr = htonl(INADDR_ANY);
	//for port, use htons to conver to an address representation
	address.sin_port = htons(PORT);

	//bind the socket 
	int bind_return = bind(server_file_descriptor, (struct sockaddr *)&address, addresslen);

	//make sure the bind worked - if bind() returned -1, failed
	if (bind < 0) {
		perror("bind failed! fix it.");
	}

	//now we make the socket listen for incoming signals
	//original socket is only used to accept connection
	//function accept() will create new sockets when a new request is made

	//second parameter of listen() means we will accept 3 connections
	if (listen(server_file_descriptor, 3) < 0) {
		perror("Failure in listen!");
	}

	while(1) {
		//server is waiting for new message, print that out
		printf("Server is waiting for a request!\n");
		//create new socket when a new request comes in
		new_socket = accept(server_file_descriptor, (struct sockaddr *)&address, (socklen_t*)&addresslen);

		//check if accept worked, if its -1 it didn't
		if (new_socket<1) {
			perror("Failure in accept");
		}

		//now create buffer to read incoming requests
		char buffer[3000] = {0};
		//read from socket
		socket_read = read(new_socket, buffer, 3000);
		//give buffer new line
		printf("%s\n", buffer); 
		//write to socket
		write(new_socket, example_message, strlen(example_message));
		//message is now sent! 
		printf("message sent!\n");
	}
	return 0;

}