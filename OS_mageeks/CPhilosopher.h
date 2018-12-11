#pragma once

#include <mutex>
#include <thread>
#include <iostream>
#include <condition_variable>

//#define normalPhilosophers
//#define RRPhilosophers
#define SJFPhilosophers

using std::mutex;
using std::this_thread::sleep_for;
using std::chrono::milliseconds;

class CFork;

class CPhilosopher
{
public:
  CPhilosopher();
  CPhilosopher(std::mutex* _pMtx);
  CPhilosopher(const CPhilosopher & other);
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
  float think_timer;
  bool gotRightFork;
  bool gotLeftFork;
  bool hadDinner;
  int nDinnersHad;

  float RRTime;
  float timeSlice;
  unsigned int id;
  std::condition_variable cv;

  std::mutex* mtx;
  //std::unique_lock<std::mutex> mtxLock;
  
};

