
#include <stdio.h>
#include <stdlib.h>

#include "peer.h"

peer *init_peer(char *ip, int port){
  peer *new_peer;
  if((new_peer = malloc(sizeof(peer)))==NULL){
    perror("peer: malloc");
  }

  new_peer->ip = ip;
  new_peer->port = port;

  return new_peer;
}
