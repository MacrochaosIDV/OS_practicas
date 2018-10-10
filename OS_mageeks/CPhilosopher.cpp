#include "CPhilosopher.h"
#include <iostream>
#include <mutex>
#include <condition_variable>

#include "CFork.h"


#ifdef RAND_MAX
#  undef RAND_MAX
#  define  RAND_MAX 20
#elif 
#  define  RAND_MAX 20
#endif


CPhilosopher::CPhilosopher() {
  rightFork = nullptr;
  leftFork = nullptr;
  waittimer = 0;
  eatTimer = 0;
  id = 0;
}

void CPhilosopher::setForks(CFork* rf, CFork* lf) {
  rightFork = rf;
  leftFork = lf;
}

void CPhilosopher::setID(int _id) {
  id = _id;
}

void CPhilosopher::live() {
  std::condition_variable cv;
  std::mutex mtx;
  std::unique_lock<std::mutex> lock(mtx);
  float i = 0.0f;
  waittimer = std::rand();
  eatTimer = 5.0f;
  while (1) {
    
    cv.wait(lock, [this]() { return(waittimer <=0 )? true : false; });
    if (!leftFork->isInUse) { // Maybe he eats
      leftFork->pickUp();
      if (rightFork->isInUse) {
        i = 2.0f;
        while (i >= 0) {
          i -= 0.016;
        }
      }
      if (!rightFork->isInUse) { // Dinner time
        rightFork->pickUp();
        cv.notify_all();
        std::cout << "Philosopher " << id << "started eating\n";
        while (eatTimer >= 0) {
          i -= 0.016;
        }
        std::cout << "Philosopher " << id << "finished eating\n";
      }
      leftFork->putDown();
    }
    // Not eating right now
    else {
      cv.notify_all();
      while (waittimer >= 0) {
        i -= 0.016;
      }
    }
    // Reset for next loop
    waittimer = std::rand();
    eatTimer = 5.0f;
  }
}
