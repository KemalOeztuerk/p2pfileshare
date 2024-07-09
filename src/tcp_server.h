#ifndef TCP_SERVER_H
#define TCP_SERVER_H

typedef void *(*handler_t)(int sockfd,void *args);

typedef struct tcp_server_args_t{
  char *port;
  handler_t handler;
  void *args; // extra arguements
  
}tcp_server_args;

void *start_tcp_server(void *args);

#endif 
