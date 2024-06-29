#ifndef UTILS_H
#define UTILS_H

#include <sys/socket.h>
#include <netinet/in.h>  // For sockaddr_in, sockaddr_in6
#include <arpa/inet.h>   // For inet_ntoa, inet_ntop
#include "message.h"
#include "metainfo.h"
void *get_in_addr(struct sockaddr *sa);

size_t get_file_size(char *file_path);


char* message_to_xml(message *msg);
void xml_to_message(const char *xml_str, message *msg);

#endif
