/*****************************************************************************

                                echoserver_parallel.c:

Dieses Server-Programm liest einen Text vom Client ein und sendet ihn um den
Server-Rechner-Namen erweitert an den Client zurueck.

Der Server kann parallel mehrere Client-Verbindungen bearbeiten.

*****************************************************************************/

#include "echo.h"
#include <signal.h>
/*
void waiter() {
        wait(0);
        signal(SIGCHLD,waiter); // Reinstall signal handler
}
*/

void worker(int in,int out) {
        char inbuf[MAXLEN], outbuf[MAXLEN], hostname[MAXLEN];
	int count;

	gethostname(hostname, MAXLEN); /* eigenen Rechnernamen ermitteln */
        while ((count=read(in, inbuf, MAXLEN)) > 0) {
		inbuf[count]='\0';
		sprintf(outbuf,"%s: %s", hostname, inbuf);
		write(out, outbuf, strlen(outbuf));
	}
}



int main (void) {
	int sock, fd;
	socklen_t client_len;
	struct sockaddr_in server, client;

	/* Socket anlegen */

	sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock < 0) {
		perror("creating stream socket");
		exit(1);
	}

	/* Server-Struktur zusammensetzen */

	server.sin_family 	= AF_INET;
	server.sin_addr.s_addr	= htonl(INADDR_ANY);
	server.sin_port		= htons(TCP_PORT);

	/* Server-Prozess an Port TCP_PORT binden, wobei Client-Requests von */
        /* jeder Netzwerkkarte entgegen genommen werden.		     */

	if (bind(sock, (struct sockaddr *)&server, sizeof(server)) < 0) {
		perror("binding socket");
		exit(2);
	}

	/* Warteschlange fuer mehrere connect-Requests einrichten */

	if (listen(sock, 5) < 0) perror("listen");
	signal(SIGCHLD, SIG_IGN);
	// signal(SIGCHLD,waiter);

	while (1) {
		client_len = sizeof(client);
		if ((fd = accept(sock, (struct sockaddr *) &client, (socklen_t *) &client_len)) < 0) {
			perror("accepting connection");
			exit(3);
		}

		/* Kommunikation mit dem Client in worker-Funktion gekapselt */
        	if (fork() == 0) {
                        /* child: process the request */
                        worker(fd,fd);
                        exit(0);
                } else close(fd);
	}
	//shutdown(fd,SHUT_RDWR);
	//close(fd);
	return 0;
}
