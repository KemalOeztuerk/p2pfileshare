#ifndef UTILS_H
#define UTILS_H

#include <sys/socket.h>
#include <netinet/in.h>  // For sockaddr_in, sockaddr_in6
#include <arpa/inet.h>   // For inet_ntoa, inet_ntop

void *get_in_addr(struct sockaddr *sa);

size_t get_file_size(char *file_path);

#endif
