
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "peer.h"
#include "metainfo.h"
#include "message.h"

#define PUBLISH 1
#define DOWNLOAD 2

message *init_message(metainfo *mi, peer *p, int flag) {
  message *new_msg = malloc(sizeof(message));
  if (!new_msg) {
    perror("init_message: malloc");
    return NULL;
  }

  new_msg->metainfo = malloc(sizeof(metainfo));
  if (!new_msg->metainfo) {
    perror("init_message: malloc metainfo");
    free(new_msg);
    return NULL;
  }
  memcpy(new_msg->metainfo, mi, sizeof(metainfo));

  new_msg->peer = malloc(sizeof(peer));
  if (!new_msg->peer) {
    perror("init_message: malloc peer");
    free(new_msg->metainfo);
    free(new_msg);
    return NULL;
  }
  memcpy(new_msg->peer, p, sizeof(peer));

  new_msg->flag = flag;

  return new_msg;
}

void free_message(message *msg) {
  if (msg) {
    if (msg->metainfo) {
      free(msg->metainfo);
    }
    if (msg->peer) {
      free(msg->peer);
    }
    free(msg);
  }
}

