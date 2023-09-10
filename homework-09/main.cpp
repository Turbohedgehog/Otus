
#include <iostream>
#include <thread>

#include "async.h"

using namespace std::chrono_literals;

void test1() {
  std::this_thread::sleep_for(1s);
  auto context_id = homework_09::connect(3);
  homework_09::receive("cmd1", 4, context_id);
  std::this_thread::sleep_for(100ms);
  homework_09::receive("cmd2", 4, context_id);
  std::this_thread::yield();
  homework_09::receive("{", 1, context_id);
  homework_09::receive("cmd3", 4, context_id);
  std::this_thread::sleep_for(100ms);
  homework_09::receive("cmd4", 4, context_id);
  homework_09::receive("{", 1, context_id);
  std::this_thread::yield();
  homework_09::receive("cmd5", 4, context_id);
  homework_09::receive("cmd6", 4, context_id);
  std::this_thread::sleep_for(100ms);
  homework_09::receive("}", 1, context_id);
  homework_09::receive("cmd7", 4, context_id);
  std::this_thread::yield();
  homework_09::receive("}", 1, context_id);
  homework_09::receive("cmd8", 4, context_id);
  std::this_thread::yield();
  homework_09::disconnect(context_id);
}

void test2() {
  std::this_thread::sleep_for(1s);
  auto context_id = homework_09::connect(1);
  homework_09::receive("dmc1", 4, context_id);
  std::this_thread::sleep_for(100ms);
  homework_09::receive("dmc2", 4, context_id);
  std::this_thread::yield();
  homework_09::receive("{", 1, context_id);
  homework_09::receive("dmc3", 4, context_id);
  std::this_thread::sleep_for(100ms);
  homework_09::receive("dmc4", 4, context_id);
  homework_09::receive("{", 1, context_id);
  std::this_thread::yield();
  homework_09::receive("dmc5", 4, context_id);
  homework_09::receive("dmc6", 4, context_id);
  std::this_thread::sleep_for(100ms);
  homework_09::receive("}", 1, context_id);
  homework_09::receive("dmc7", 4, context_id);
  std::this_thread::yield();
  homework_09::receive("}", 1, context_id);
  homework_09::receive("dmc8", 4, context_id);
  std::this_thread::yield();
  homework_09::disconnect(context_id);
}

void test3() {
  auto context_id = homework_09::connect();
  homework_09::receive("www1", 4, context_id);
  homework_09::receive("www2", 4, context_id);
  homework_09::receive("{", 1, context_id);
  homework_09::receive("www3", 4, context_id);
  std::this_thread::sleep_for(1s);
  homework_09::receive("www4", 4, context_id);
  homework_09::receive("}", 1, context_id);
  homework_09::receive("www5", 4, context_id);
  std::this_thread::sleep_for(100ms);
  homework_09::receive("{", 1, context_id);
  homework_09::receive("www6", 4, context_id);
  homework_09::receive("www7", 4, context_id);
  std::this_thread::sleep_for(100ms);
  homework_09::receive("www8", 4, context_id);
  homework_09::receive("}", 1, context_id);
  homework_09::receive("www9", 4, context_id);
  homework_09::receive("wwwA", 4, context_id);
  std::this_thread::sleep_for(100ms);
  homework_09::receive("wwwB", 4, context_id);
  homework_09::receive("wwwC", 4, context_id);
  std::this_thread::sleep_for(100ms);
  homework_09::disconnect(context_id);
}

int main () {
  auto t1 = std::thread(test1);
  auto t2 = std::thread(test2);
  auto t3 = std::thread(test3);
  t1.join();
  t2.join();
  t3.join();

  return 0;
}