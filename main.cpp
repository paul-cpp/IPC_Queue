#include "Queue.h"
#include <chrono>
#include <thread>

using namespace std;

static const int KEY = 1234;

void wd(Queue &q) {
  while (1) {
    q.PrintStatus();
    this_thread::sleep_for(chrono::seconds(2));
  }
}
int main() {
  Queue q;
  q.Create(KEY);

  //  thread t(wd, std::ref(q));
  //  t.join();

  return EXIT_SUCCESS;
}
