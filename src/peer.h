#ifndef PEER_H
#define PEER_H

#include <arpa/inet.h>
#include "metainfo.h"
typedef struct peer_t{
  char ip[INET_ADDRSTRLEN];
  int port;
}peer;

peer *init_peer(char *ip, int port);
#endif
