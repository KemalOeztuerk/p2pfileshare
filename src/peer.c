
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h> 
#include "peer.h"

peer *init_peer(char *ip, int port) {
  peer *new_peer = (peer *)malloc(sizeof(peer));
  if (!new_peer) {
    perror("init_peer: malloc");
    return NULL;
  }

  strncpy(new_peer->ip, ip, INET_ADDRSTRLEN - 1);
  new_peer->ip[INET_ADDRSTRLEN - 1] = '\0';  // Ensure null-termination
  new_peer->port = port;

  return new_peer;
}
