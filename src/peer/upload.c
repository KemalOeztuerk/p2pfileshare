
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
#include "../metainfo.h"
#include "../peer.h"
#include "../utils.h"
/*typedef struct peer_t{
  int id;
  metainfo files[128];
  struct sockaddr_storage client_addr;
  
  }peer;*/


void *upload_handler(int client_fd, void *args){
  // attemp forward port with UPNP
  /*if(attempt_forward_port()!=0){
    printf("file sharing is only possible in local network\n");
    }*/
  
  //struct sockaddr_storage client_addr = *(struct sockaddr_storage*)peer_info;
  printf("uploader started to listen\n");
  //char s[INET6_ADDRSTRLEN];
  char buf[MAXDATASIZE];
  FILE *fp;
  // char filename[64];
  //char hash[SHA1_STR_SIZE]; // hash value of the file to be uploaded
  size_t xml_buffer_size = 10000;
  char *xml_buffer = malloc(xml_buffer_size);
  
  if(recv(client_fd, xml_buffer, xml_buffer_size, 0 ) == -1){ //send command
    perror("server: send");
    free(xml_buffer);
    exit(1);
  }

  message msg;
  xml_to_message(xml_buffer, &msg);
  
  // inet_ntop(client_addr.ss_family, get_in_addr((struct sockaddr*)&client_addr), s, sizeof(s));
    
  //printf("Server got connection from %s\n", s);

  //check if hash values are the same
  /* char *path = "files/";
  if((path=strcat(path, msg.metainfo->file_name))!=0){
    perror("upload: stcat:");
    }
  printf("%s\n",path);
  char *hash = hash_file(path);
  printf("%s %s\n ",hash, msg.metainfo->sha1_info);
  printf("path:%s\n",path);*/
  
  if((fp = fopen(msg.metainfo->file_name,"rb"))==NULL){
    perror("upload_handler: cannot find file");
    return NULL;
  }

  //send 
  size_t bytes_read;
  while ((bytes_read = fread(buf, 1, MAXDATASIZE, fp)) > 0) {
    if (send(client_fd, buf, bytes_read, 0) == -1) {
      perror("server: send");
      fclose(fp);
      close(client_fd);
      break;
    }
    printf("%lu\n",bytes_read);
  }

  fclose(fp);
  close(client_fd);
  printf("file has been sent\n");

  return (void*)1;
}




