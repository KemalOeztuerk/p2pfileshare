#ifndef PEER_H
#define PEER_H

#include "metainfo.h"
typedef struct peer_t{
  char *ip;
  int port;
}peer;

peer *init_peer(char *ip, int port);
#endif
