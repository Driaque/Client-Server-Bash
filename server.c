#include <stdlib.h>
#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

/**
*	This program is the server end of a remote shell through socket.
*	The client program must run firstly then the clients can connect to the machine.
*	Usage: <Program> <Port>
*/


// maximum lenth of arguments which should be big enough to make sure the socket never fails of big amount of data
#define MAX_LENGTH 1000000
// maximum lenth of each command and each argument
#define MAX_WORD_LENGTH 40
int run(char* buff, int length)
{
	int i = 0;
	int x = 0;
	int y = 0;
	int z = 0;
	//i = 0;
	
	int exitStatus = -1;
	//char buff[100]; //Buffer
	char *image;
	char *commands[40];
	char delimiter[40];
	char *arguments[10];
	char *tkn;
	
	image = strdup(buff);
	//printf("executing command(s)......\n");
	tkn = strtok(buff, "&;|\n"); 
	while (tkn != NULL)
	{
		delimiter[i] = image[tkn - buff + strlen(tkn)];
		commands[i] = tkn;
		i++;
		tkn = strtok(NULL, "&;|\n"); 
		
	}
	commands[i] = NULL;
	y = i;
	i = 0;
	while (i <= y)
	{
		i++;
	}
	i = 0;
	exitStatus = -1;
	while (i < y)
	{
		if (exitStatus > -1) {
			//if previous command fails
			if (delimiter[i - 1] == '&') {
				if (exitStatus != 0) {
					break;
				}
			} 
		}

		tkn = strtok(commands[i], " ");
		x = 0;
		while (tkn != NULL & x < 10)
		{
			arguments[x] = tkn;
			x++;
			tkn = strtok(NULL, " ");
		}
		arguments[x] = NULL;
		z = x;
		x = 0;
		if (strcmp(arguments[0], "exit") == 0)
		{
			exit(0);
		}
		while (x <= z)
		{
			x++;
		}
		//child Process
		if (fork()== 0)
		{
			//check if command is Valid
			execvp(arguments[0], arguments);
			fprintf(stderr, "Invalid Command detected!\n");
			exit(1);
		}
		else
		{
				wait(&exitStatus);
		//printf("\n");
		}
		i++;
	}
}

void serverClient(int s);
int mySplit(char** res, char* s, char* delim);

int main(int argc, char* argv[]) {
	int sd, client, portNumber, status;
	
	socklen_t len;
	struct sockaddr_in servAdd;

	if (argc != 2) {
		printf("Call model: %s <Port:#>\n", argv[0]);
		exit(0);
	}

	if ((sd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		perror("Wrong creating new socket");
		exit(1);
	}

	//config the server socket
	servAdd.sin_family = AF_INET;
	servAdd.sin_addr.s_addr = htonl(INADDR_ANY);
	sscanf(argv[1], "%d", &portNumber);
	servAdd.sin_port = htons((uint16_t)portNumber);
	bind(sd, (struct sockaddr*) &servAdd, sizeof(servAdd));
    int Count=0;
	// start listening to the assigned port for at most 5 clients
	
	listen(sd, 5);
	fprintf(stderr,"waiting for clients to connect");
	while (1) {
		// wait for clients from any IP address
		
		client = accept(sd, (struct sockaddr*) NULL, NULL);
       //// printf("Got a new client\n");
		printf("Client %d connected\n", ++Count);
		printf("Got a new client\n");
		// fork a process for each client
		if (fork()==0)
			serverClient(client);
		//close(client);
		//waitpid(0, &status, WNOHANG);
	}

}



/**
* int mySplit(char** res, char *s, char* delim);
* implement custom Split function to extract command and arguments from the given string.
* parameters:
*      char** res: point of a array of string where the array stores commands
*      and arguments.
*      char* s: point of the given string.
*      char* delim: point of the string as a delimiter.
*  return value:
*      return the number of command plus arguments.
*/
int mySplit(char** res, char* s, char* delim) {
	//change the last character of s from '\n' to '\0' otherwise execvp()
	//cannot be executed.
	s[strlen(s) - 1] = 0;
	char * token;
	token = strtok(s, delim);
	int i = 0;
	// use while loop to seperate string s iteratively.
	while (token != NULL) {
		res[i] = (char*)malloc(sizeof(char) * strlen(token));
		strcpy(res[i++], token);
		token = strtok(NULL, delim);
	}
	//return size of the array.
	return i;
}


/**
*	void serverClient(int sd);
*	function to deal with a client
*	parameters:
*		int sd: socket descriptor returned by socket().
*/
void serverClient(int sd) 
{
	char message[MAX_LENGTH];
	int n, pid;
	int status = -1;
	char **res;
	char *token;
    char *copy;
	char *cmd_array[40];
	char *args[10];
	char delim_array[40];
	int k = 0;
	int j = 0;
	int l = 0;
	int i=0;
	//make the screen descriptor designate the client socket
	dup2(sd, STDOUT_FILENO);
	dup2(sd, STDIN_FILENO);
	dup2(sd, STDERR_FILENO);
	while (1) 
	{
		//pid = fork();
		// fork a new process for every command
	  //if (!pid) 
	  //{
		 int i=0;
			//read from client
			//printf("reading from client....\n");
		 n = read(sd, message, MAX_LENGTH) ;
		 //printf("read done\n");
		 
				// quit if the client sends 'quit'
		 message[n] = '\0';
		 run(message, n);
		printf("DONE\n");
		
	 //}
		
    }
    
}
