#ifndef QUEUE_H
#define QUEUE_H

#include <cstdio>
#include <iostream>

#include <cstring>
#include <errno.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/types.h>

// static const int KEY = 1234;
static const int MSGSZ = 512;

class Queue {
public:
  Queue();
  Queue(key_t key);
  ~Queue();

  int Create(key_t key);
  int Open(key_t key);
  int Send(const char *buf);
  std::string Receive();
  //  void Delete();
  //  void Clear();
  void PrintStatus();

private:
  key_t key;
  int qid;
  struct msgbuf s_buf, r_buf;
  struct msqid_ds qstatus;
};

#endif // QUEUE_H
