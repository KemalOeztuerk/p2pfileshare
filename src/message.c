
#include <stdio.h>
#include <stdlib.h>

#include "peer.h"
#include "metainfo.h"
#include "message.h"

message *init_message(metainfo *metainfo, peer *peer, int flag){
  message *new_msg = malloc(sizeof(message));
  new_msg->metainfo = metainfo;
  new_msg->peer = peer;
  new_msg->flag = flag;
  return new_msg;
}
