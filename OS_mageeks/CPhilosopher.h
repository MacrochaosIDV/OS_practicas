#pragma once

#include <mutex>
#include <thread>
#include <iostream>
#include <condition_variable>

using std::this_thread::sleep_for;
using std::chrono::milliseconds;

class CFork;

class CPhilosopher
{
public:
  CPhilosopher();
  ~CPhilosopher() = default;

  void setForks(CFork* rf, CFork* lf);
  void setID(int _id);
  void live();
  void eat();
  void pickUpRightFork();
  void pickUpLeftFork();
  void putDownRightFork();
  void putDownLeftFork();

  CFork* rightFork;
  CFork* leftFork;
  float waittimer;
  float eatTimer;
  bool gotRightFork;
  bool gotLeftFork;
  bool hadDinner;
  unsigned int id;
  std::condition_variable cv;
  std::mutex mtx;
};

