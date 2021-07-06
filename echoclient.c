/*****************************************************************************

				echoclient.c: 

Dieses Client-Programm liest einen Text ein und sendet ihn zum Server. 
Anschliessend wird der modifizierte Text vom Server wieder empfangen.

*****************************************************************************/

#include "echo.h"

int main(int argc, char** argv) {

	char 	*server_name;     	    /* Namen des Servers */
	struct 	hostent		*host_info; /* Host-Struktur hier aufgebaut */
	struct 	sockaddr_in	server;     /* Server-Adresse hier aufgebaut */
	int	sock;			    /* Socket-Deskriptor */
	int	count;			    /* Zaehler fuer Zeichen */
	char 	line_in[MAXLEN];            /* Puffer: Client -> Server */
	char 	line_out[MAXLEN];           /* Puffer: Server -> Client */


	/* Server Namen entweder von der Kommandozeile, sonst localhost */

	server_name = (argc > 1) ? argv[1] : "localhost";
	
	/* Host-Struktur des Server-Rechners fuellen */

	host_info = gethostbyname(server_name);
	if (host_info == NULL) {
		fprintf(stderr, "%s: unknown host: %s\n", argv[0], server_name);
		exit(1);
	}
	
	/* Socket anlegen */

	sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock < 0) {
		perror("creating stream socket");
		exit(2);
	}

	/* Server-Struktur zusammensetzen */

	server.sin_family = host_info->h_addrtype; // bei IP auch fest AF_INET
	memcpy((char *)&server.sin_addr,host_info->h_addr,host_info->h_length);
	server.sin_port = htons(TCP_PORT);

	/* Mit dem Server-Prozess verbinden */

	if (connect(sock, (struct sockaddr *)&server, sizeof(server)) < 0) {
		perror("connecting to server");
		exit(3);
	}
	
	printf("connection to server %s established.\n", server_name);

	/* Einlesen von Tastatur, Senden zum Server, Empfangen vom Server, */
   	/* Ausgabe auf Bildschirm. Ende bei EOF-Zeichen ctrl-d (Tastatur)  */

	while ((count = read(0, line_out, MAXLEN)) > 0) {
		write(sock, line_out, count);
		count = read(sock, line_in, MAXLEN);
		write(1, line_in, count);
	}
	shutdown(sock, 2);
	close(sock);
	return 0;
}
