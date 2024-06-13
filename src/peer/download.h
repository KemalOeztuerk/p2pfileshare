#ifndef DOWNLOAD_H
#define DOWNLOAD_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include "utils.h"
#define PORT "5555" // client to be connected
#define MAXDATASIZE 1024

int download(char *hostname, char *filename);

#endif
