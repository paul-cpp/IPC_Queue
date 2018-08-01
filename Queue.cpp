#include "Queue.h"

Queue::Queue() : key(0), qid(-1), s_buf{}, r_buf{}, qstatus{} {}

Queue::Queue(key_t key) : Queue() { Create(key); }

Queue::~Queue() {}

int Queue::Create(key_t key) {
  this->key = key;
  qid = msgget(key, IPC_CREAT | 0666);
  if (qid == -1) {
    perror("Queue::Create error");
    return -1;
  }
  printf("Queue with key %d, id %d created\n", this->key, this->qid);
  return qid;
}

int Queue::Open(key_t key) {
  if (qid == -1)
    this->key = key;
  qid = msgget(key, 0666);
  if (qid == -1) {
    perror("Queue::Open error");
    return -1;
  }
  printf("Queue with key %d, id %d opened\n", this->key, this->qid);

  return qid;
}

int Queue::Send(const char *buf) {
  s_buf.mtype = 1;
  strcpy(s_buf.mtext, buf);
  size_t buf_len = strlen(s_buf.mtext) + 1;
  int rc = msgsnd(qid, &s_buf, buf_len, IPC_NOWAIT);
  if (rc < 0) {
    perror("msgsnd failed");
    return rc;
  }
  printf("Message %s sended, length %d bytes\n", s_buf.mtext, buf_len);

  return rc;
}

std::string Queue::Receive() {
  r_buf.mtype = 1;
  int rc = msgrcv(qid, &r_buf, MSGSZ, r_buf.mtype, 0);
  if (rc < 0) {
    perror("msgrcv failed");
    return std::string();
  }
  printf("Message %s received, length %d bytes\n", r_buf.mtext, rc);
  std::string str = r_buf.mtext;
  return str;
}

void Queue::PrintStatus() {

  if (msgctl(qid, IPC_STAT, &qstatus) < 0) {
    perror("msgctl failed");
    return;
  }
  printf("--STATUS--\n");
  printf("Real user id of the queue creator: %d\n", qstatus.msg_perm.cuid);
  printf("Real group id of the queue creator: %d\n", qstatus.msg_perm.cgid);

  printf("Effective user id of the queue creator: %d\n", qstatus.msg_perm.uid);
  printf("Effective group id of the queue creator: %d\n", qstatus.msg_perm.gid);
  printf("Permissions: %d\n", qstatus.msg_perm.mode);
  printf("Message queue id: %d\n", qid);
  printf("%d message(s) on queue\n", qstatus.msg_qnum);
  printf("Last message sent by process :%3d at %s \n", qstatus.msg_lspid,
         ctime(&(qstatus.msg_stime)));

  printf("Last message received by process :%3d at %s \n", qstatus.msg_lrpid,
         ctime(&(qstatus.msg_rtime)));
  printf("Current number of bytes on queue %d\n", qstatus.msg_cbytes);
  printf("Maximum number of bytes allowed on the queue%d\n",
         qstatus.msg_qbytes);
}
