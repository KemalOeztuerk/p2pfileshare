#ifndef TCP_CLIENT_H
#define TCP_CLIENT_H

typedef void *(*handler_t)(int sockfd,void *args);


void *start_tcp_client(char *hostname, char *port, handler_t handler, void *args);

#endif
