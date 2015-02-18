#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <time.h>

#define BACKLOG 10

int main (int argc, char *argv[]) {
	int listenfd, connfd, sin_size;
	struct sockaddr_in servaddr;
	char buff[1000];
	time_t ticks;
	// Server admin provides the servers IP address and port number
	if (argc != 2) {
		printf ("Error with arguments!!! Usage:%s <portno>\n", argv[0]);
		return 0;
	}
	//Step 1: Socket function
	listenfd = socket (AF_INET, SOCK_STREAM, 0);
	printf ("SERVER: Listening socket opened with listenfd = %d\n", listenfd);
	//Step 2: Initialize address structure
	bzero (&servaddr, sizeof (servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl (INADDR_ANY);
	// servaddr.sin_port = htons (atoi (argv[1]));
	servaddr.sin_port = htons (13);
	// Step 3: Bind
	bind (listenfd, (struct sockaddr *)&servaddr, sizeof(servaddr));
	// Step 4: Start listening to client resquests
	listen (listenfd, BACKLOG);
	for (;;) {
		// Accept client request
		connfd = accept (listenfd, (struct sockaddr *)NULL, NULL);
		if (fork () == 0) {
			// In Child Process
			close (listenfd);
			printf ("SERVER: Connection established with connfd = %d\n", connfd);
			// Prepare data to be sent to connected client
			ticks = time (NULL);
			snprintf (buff, sizeof (buff), "%.24s\r\n--- \r\n", ctime (&ticks));
			write (connfd, buff, strlen (buff));			// Send data to client
			close (connfd);						// Close client connection
			return 0;						// End child process
		}
		close (connfd);							// Close client connection with original server process
	}
	close (listenfd);							// Close listening socket
	return 0;
}
