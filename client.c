#include <stdlib.h>
#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/wait.h>


/**
*	This program is the client end of a remote shell through socket.
*	The client program must run after the server program.
*	Usage: <Program> <IP address> <Port>
*/

// maximum lenth of arguments which should be big enough to make sure the socket never fails of big amount of data
#define MAX_LENGTH 1000000
// maximum lenth of each command and each argument
#define MAX_WORD_LENGTH 40

char* myRead(char *s, int fd);


int main(int argc, char* argv[]) {
	char message[MAX_LENGTH];
	int server, portNumber, pid, n, status;
	socklen_t len;
	struct sockaddr_in servAdd;

	if (argc != 3) {
		fprintf(stderr, "Call model:%s <IP> <Port>\n", argv[0]);
		exit(0);
	}

	if ((server = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		fprintf(stderr, "Cannot create a socket\n");
		exit(1);
	}

	//config the server socket
	servAdd.sin_family = AF_INET;
	sscanf(argv[2], "%d", &portNumber);
	servAdd.sin_port = htons((uint16_t)portNumber);
	if (inet_pton(AF_INET, argv[1], &servAdd.sin_addr) < 0) {
		fprintf(stderr, " inet_pton() has failed\n");
		exit(2);
	}
	//connect to the server
	if (connect(server, (struct sockaddr*) &servAdd, sizeof(servAdd)) < 0) {
		fprintf(stderr, "Connection refused\n");
		exit(3);
	}

	while (1) {
		//pid = fork();
		//if (!pid) {
			//prompt
			write(STDOUT_FILENO, ">>", 2);
			n = read(STDIN_FILENO, message, MAX_LENGTH);
			message[n] = '\0';
			//send commands to the server using space" " to seperate arguments
			write(server, message, strlen(message) + 1);
			//user types 'quit' to close the connection
			if (!strcasecmp(message, "quit\n")) {
				close(server);
				kill(getppid(), SIGTERM);
				exit(0);
			}
			//printf("waiting for server...\n");
			do {
			if (n = read(server, message, MAX_LENGTH)) {
				message[n] = '\0';
				write(STDOUT_FILENO, message, n + 1);
				//printf("server returned %s\n", message);
			}
			} while (strncmp(message, "DONE", 4));
			//exit(0);
		//}
		//else {
		//	wait(&status);
		//}
	}
}


