#ifndef UPLOAD_H
#define UPLOAD_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>
#include "utils.h"

#define PORT "5555"
#define BACKLOG 10
#define MAXDATASIZE 1024

int upload();

#endif
