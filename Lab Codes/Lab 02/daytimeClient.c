#include <stdio.h>	//Basic I/O Routines
#include <stdlib.h>	//Exit Routine
#include <memory.h>	//Memset, Memcopy
#include <sys/types.h>	//Standard System Types
#include <sys/socket.h>	//Socket Interface Functions
#include <netinet/in.h>	//Internet Address Structures
#include <netdb.h>	//Host to IP Resolution

#define HOSTNAMELEN 40	//Maximum Host Name Length
#define BUFLEN 1024	//Maximum Response Size
#define PORT 13		//Port of daytime Server

int main (int argc, char *argv[]) {
	printf ("XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX\n");
	printf ("XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX     DayTime Client     XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX\n");
	printf ("XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX\n");
	int nSystemCallReturn;			// System Calls return value storage
	int nSockDesc;				// Socket Descriptor
	char cBuffer [BUFLEN + 1];		// Buffer Server Answer
	char *pc;				// Pointer into the buffer
	struct sockaddr_in sa;			// Internet address struct
	struct hostent * hen;			// Host to IP translation
	/* Check if there are enough parameters */
	if (argc < 2) {
		fprintf (stderr, "*******ERROR*******\nHost Name Missing\n");
		return 1;
	}
	/* Address Resolution Stage */
	hen = gethostbyname (argv[1]);
	if (!hen) {
		perror ("*******ERROR*******\nCouldn't Resolve Host Name\n");
	}
	/* Initiate machine's internet address structure, first clear out the struct, to avoid garbage */
	memset (&sa, 0, sizeof (sa));
	/* Using Internet address family */
	sa.sin_family = AF_INET;
	/* Copy port number in network byte order */
	sa.sin_port = htons (PORT);		// Host to netwotk short
	/* Copy IP address into address struct */
	memcpy (&sa.sin_addr.s_addr, hen->h_addr_list[0], hen->h_length);
	/* Allocate a free socket Internet address family, Stream socket */
	nSockDesc = socket (AF_INET, SOCK_STREAM, 0);
	if (nSockDesc < 0) {
		perror ("*******ERROR*******\nSocket Allocation Failed\n");
	}
	/* Now Connect to the remote server. The System will use the 4th binding method(See section 3). 
	* Note the cast to a struct sockaddr pointer of the address of variable sa */
	nSystemCallReturn = connect (nSockDesc, (struct sockaddr *)&sa, sizeof (sa));
	/* Check there was no error */
	if (nSystemCallReturn) {
		perror ("Connect");
	}
	/* Now that we are connected, start reading the socket till read() returns 0, meaning the server closed the connection */
	pc = cBuffer;
	while (nSystemCallReturn = read (nSockDesc, pc, BUFLEN - (pc - cBuffer))) {
		pc += nSystemCallReturn;
	}
	/* Close The Socket */
	close (nSockDesc);
	/* Pad a null character to the end of the result */
	*pc = '\0';
	/* Print the result */
	fprintf (stdout, "\nTime: %s\n", cBuffer);
	/* and terminate */
	printf ("XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX\n");
	return 0;
}
