
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

#define PORT "5555"
#define BACKLOG 10
#define MAXDATASIZE 1024

#include "upload.h"
#include "../tcp_server.h"
#include "metainfo.h"

typedef struct peer_t{
  int id;
  metainfo files[128];
  struct sockaddr_storage client_addr;
  
}peer;


void upload_handler(int client_fd, void *peer_info){
  struct sockaddr_storage client_addr = *(struct sockaddr_storage*)peer_info;
  
  char s[INET6_ADDRSTRLEN];
  char buf[MAXDATASIZE];
  FILE *fp;
  char filename[64];

  
    if(recv(client_fd, filename, sizeof(filename)-1, 0 ) == -1){ //send command
    perror("server: send");
    exit(1);
    }
    printf("%s \n ", filename);

 
    inet_ntop(client_addr.ss_family, get_in_addr((struct sockaddr*)&client_addr), s, sizeof(s));
    
    //printf("Server got connection from %s\n", s);
    
    fp = fopen(filename,"r");
    
    //send 
    size_t bytes_read;
    while ((bytes_read = fread(buf, 1, MAXDATASIZE, fp)) > 0) {
      if (send(client_fd, buf, bytes_read, 0) == -1) {
	perror("server: send");
	fclose(fp);
	close(client_fd);
	break;
      }
    }

    fclose(fp);
    close(client_fd);

    printf("file has been sent\n");
}




