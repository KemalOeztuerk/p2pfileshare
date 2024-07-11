#ifndef TRACK_H
#define TRACK_H

#include <stdio.h>
#include <arpa/inet.h>
#include <sqlite3.h>
#include "metainfo.h"

sqlite3 *init_database();
void *tracker_handler(int client_fd,void *args);

#endif
