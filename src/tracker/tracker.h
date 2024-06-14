#ifndef TRACKER_H
#define TRACKER_H

#include <stdio.h>
#include <arpa/inet.h>
#include "../peer/metainfo.h"


typedef struct peer_t{
  int id;
  metainfo files[128];
  struct sockaddr_storage client_addr;

}peer;


int add_peer(char* hash);

int remove_peer();

#endif
