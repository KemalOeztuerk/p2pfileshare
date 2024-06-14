#ifndef TCP_SERVER_H
#define TCP_SERVER_H


typedef void (*handler_t)(int sockfd,void *args);

int start_tcp_server(char *port, handler_t handler,void *args);

#endif 
