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
#define PORT "5555" // client to be connected
#define MAXDATASIZE 1024

#include "../tcp_client.h"
#include "download.h"
#include "metainfo.h"



void download_handler(int sockfd, void *args){
  metainfo *mi = (metainfo*)args;
  // char *filename = (char*)args;
  long numbytes;
  char buf[MAXDATASIZE];

   if(send(sockfd, mi->file_name, sizeof(mi->file_name), 0 ) == -1){ //send command
    perror("client: send");
    exit(1);
  }

  //char *b = malloc(sizeof(char));


  FILE *fp = fopen("filefromserver","w");


  do {
    // Receive data into buffer
    numbytes = recv(sockfd, buf, MAXDATASIZE, 0);
    if (numbytes == -1) {
      perror("client: recv");
      exit(1);
    } else if (numbytes == 0) {
      // Connection closed by server
      break;
    }

    // Write received data to file
    long bytes_written = fwrite(buf, 1, numbytes, fp);
    if (bytes_written != numbytes) {
      if (ferror(fp)) {
	perror("fwrite");
	exit(1);
      }
    }
  } while (numbytes > 0);

  printf("recieved the file.\n");

  
  fclose(fp);
}

