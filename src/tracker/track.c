#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <sqlite3.h>

#include "../tcp_server.h"
#include "../metainfo.h"
#include "../message.h"
#include "../peer.h"
#include "track.h"

sqlite3 *init_database(){
  sqlite3 *db;
  // create database
  int rv = sqlite3_open("trackerdb.sqlite3",&db);
  if(rv){
    fprintf(stderr,"cannot open the database: %s\n",sqlite3_errmsg(db));
    return NULL;
  }

  char *err_msg = 0;
  
  char *create_peertable_sql =
    "CREATE TABLE IF NOT EXISTS peer("
    "id INTEGER PRIMARY KEY AUTOINCREMENT,"
    "ip TEXT NOT NULL,"
    "port INTEGER NOT NULL"
    ");";

  char *create_filetable_sql =
    "CREATE TABLE IF NOT EXISTS files("
    "id INTEGER PRIMARY KEY AUTOINCREMENT,"
    "file_name TEXT NOT NULL,"
    "file_hash TEXT NOT NULL,"
    "file_size INTEGER NOT NULL,"
    "tracker_host TEXT NOT NULL,"
    //"flag INTEGER NOT NULL,"
    "peer_id INTEGER NOT NULL"
    ");";

  char *create_file_peer_table_sql =
    "CREATE TABLE IF NOT EXISTS file_peer ("
    "file_id INTEGER NOT NULL, "
    "peer_id INTEGER NOT NULL, "
    "FOREIGN KEY (file_id) REFERENCES files(id), "
    "FOREIGN KEY (peer_id) REFERENCES peer(id)"
    ");";

  // create tables
  if(sqlite3_exec(db,create_peertable_sql,0,0,&err_msg) != SQLITE_OK){
    fprintf(stderr, "Peer SQL error: %s\n", err_msg);
    sqlite3_free(err_msg);
    sqlite3_close(db);
    return NULL;
  }

  if(sqlite3_exec(db, create_filetable_sql, 0, 0, &err_msg) != SQLITE_OK){
    fprintf(stderr, "File SQL error: %s\n", err_msg);
    sqlite3_free(err_msg);
    sqlite3_close(db);
    return NULL;
  }

  if (sqlite3_exec(db, create_file_peer_table_sql, 0, 0, &err_msg) != SQLITE_OK) {
    fprintf(stderr, "SQL FilePeer SQL error %s\n", err_msg);
    sqlite3_free(err_msg);
    sqlite3_close(db);
    return NULL;
  }
  
  return db;
}


void *tracker_handler(int client_fd,void *args ){

  sqlite3* db = init_database();
  message *msg = malloc(sizeof(message));
  metainfo *mi = malloc(sizeof(metainfo));
  
  if(recv(client_fd, msg, sizeof(message), 0) == -1){
    perror("tracker: recv");
    exit(-1);
  }

  // TO DO: Handle hash table here
  //  hash_table *ht = malloc(sizeof(hash_table));

  // peer *p = (peer*)malloc(sizeof(peer));
  
  //get peer address and port
  struct sockaddr_storage client_addr;
  socklen_t client_addr_size = sizeof(client_addr);
  if(getpeername(client_fd, (struct sockaddr*)&client_addr, &client_addr_size)){
    perror("tracker: getpeername");
    close(client_fd);
    exit(-1);
  }

  char client_ip[INET6_ADDRSTRLEN];
  int client_port;

  if (client_addr.ss_family == AF_INET) { // ipv4
    struct sockaddr_in *s = (struct sockaddr_in *)&client_addr;
    client_port = ntohs(s->sin_port);
    inet_ntop(AF_INET, &s->sin_addr, client_ip, sizeof(client_ip));
  } else { // ipv6
    struct sockaddr_in6 *s = (struct sockaddr_in6 *)&client_addr;
    client_port = ntohs(s->sin6_port);
    inet_ntop(AF_INET6, &s->sin6_addr, client_ip, sizeof(client_ip));
  }
  printf("Client connected: %s:%d\n", client_ip, client_port);


  char sql[512];
  sqlite3_stmt *stmt;
  int peer_id;

  switch(msg->flag){
  case PUBLISH:
    // Insert peer info into the database
    snprintf(sql, sizeof(sql),
	     "INSERT INTO peer (ip, port) VALUES (?, ?)");
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, 0) == SQLITE_OK) {
      sqlite3_bind_text(stmt, 1, client_ip, -1, SQLITE_STATIC);
      sqlite3_bind_int(stmt, 2, client_port);

      if (sqlite3_step(stmt) != SQLITE_DONE) {
	fprintf(stderr, "SQL error: %s\n", sqlite3_errmsg(db));
      }
      sqlite3_finalize(stmt);
    }

    peer_id = sqlite3_last_insert_rowid(db);

    snprintf(sql, sizeof(sql),
	     "INSERT INTO files (file_name, file_hash, file_size, tracker_host,peer_id) "
	     "VALUES (?, ?, ?, ?, ?)");
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, 0) == SQLITE_OK) {
      sqlite3_bind_text(stmt, 1, mi->file_name, -1, SQLITE_STATIC);
      sqlite3_bind_text(stmt, 2, mi->sha1_info, -1, SQLITE_STATIC);
      sqlite3_bind_int(stmt, 3, mi->file_size_in_bytes);
      sqlite3_bind_text(stmt, 4, mi->tracker_host, -1, SQLITE_STATIC);
      sqlite3_bind_int(stmt, 5, peer_id);


      if (sqlite3_step(stmt) != SQLITE_DONE) {
	fprintf(stderr, "SQL error: %s\n", sqlite3_errmsg(db));
      }
      sqlite3_finalize(stmt);
    }

    int file_id = sqlite3_last_insert_rowid(db);

    snprintf(sql, sizeof(sql),
	     "INSERT INTO file_peer (file_id, peer_id) VALUES (?, ?)");
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, 0) == SQLITE_OK) {
      sqlite3_bind_int(stmt, 1, file_id);
      sqlite3_bind_int(stmt, 2, peer_id);

      if (sqlite3_step(stmt) != SQLITE_DONE) {
	fprintf(stderr, "SQL error: %s\n", sqlite3_errmsg(db));
      }
      sqlite3_finalize(stmt);
    }

    //send()...
    send(client_fd, "success", 10, 0);
    break;
  case DOWNLOAD:
    // Retrieve and send peer info
    snprintf(sql, sizeof(sql),
	     "SELECT peer.ip, peer.port FROM peer "
	     "JOIN file_peer ON peer.id = file_peer.peer_id "
	     "JOIN files ON file_peer.file_id = files.id "
	     "WHERE files.file_hash = ?");
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, 0) == SQLITE_OK) {
      sqlite3_bind_text(stmt, 1, mi->sha1_info, -1, SQLITE_STATIC);

      while (sqlite3_step(stmt) == SQLITE_ROW) {
	peer p;
	strncpy(p.ip, (const char *)sqlite3_column_text(stmt, 0), sizeof(p.ip) - 1);
	p.port = sqlite3_column_int(stmt, 1);
	if (send(client_fd, &p, sizeof(peer), 0) == -1) {
	  perror("tracker: send");
	}
      }
      sqlite3_finalize(stmt);
    }
    //send()...
    break;
  }
  //load db here somehow
  // ...

  return (void*)0;
  
}


