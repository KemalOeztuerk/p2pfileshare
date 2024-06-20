
#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <string.h>

#include "../tcp_client.h"
#include "../peer.h"
#include "../metainfo.h"

void *announce_file(int sockfd, void *args) {
  metainfo *mi = (metainfo *)args;
  
  if (send(sockfd, mi, sizeof(metainfo), 0) == -1) {
    perror("talk_tracker: send");
    return NULL;
  }

  return (void*)1;
}


void *request_peer(int sockfd, void *args) {
  metainfo *mi = (metainfo*)args;
  peer *p = (peer*)malloc(sizeof(peer));
  if (send(sockfd, mi, sizeof(metainfo), 0) == -1) {
    perror("client: send");
    return NULL;
  }

  
  if(recv(sockfd, p, sizeof(peer), 0) == -1){
    perror("talk_tracker: recv");
    return NULL;
  }

  return (void*)p;
  
}
