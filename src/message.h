#ifndef MESSAGE_H
#define MESSAGE_H

#include "metainfo.h"
#include "peer.h"

typedef struct {
  metainfo * metainfo;
  peer *peer;
  int flag;
}message;

message *init_message(metainfo *metainfo, peer *peer, int flag);
  
#endif
