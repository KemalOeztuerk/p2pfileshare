#ifndef TRACKER_H
#define TRACKER_H

#include <stdio.h>
#include "../peer/metainfo.h"


typedef struct peer_t{
  int id;
  metainfo files[128];
}peer;


int add_peer(char* hash);

int remove_peer();

#endif
