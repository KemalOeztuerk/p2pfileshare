#ifndef TALK_TRACKER_H
#define TALK_TRACKER_H

#include "../peer.h"
#include "../metainfo.h"

void *request_peer(int sockfd, void *mi);

void *announce_file(int sockfd, void *mi);


#endif
