#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <syslog.h>
#include <signal.h>
#include <errno.h>

#define MAXLEN 		80 /* maximale Laenge eines Strings */
#define TCP_PORT	1067


