#include "CPhilosopher.h"
#include <iostream>
#include <mutex>
#include <condition_variable>

#include "CFork.h"

using std::this_thread::sleep_for;
using std::chrono::milliseconds;
using std::chrono::system_clock;
using std::cout;
using std::endl;

CPhilosopher::CPhilosopher() {
  rightFork = nullptr;
  leftFork = nullptr;
  waittimer = 0;
  eatTimer = 0;
  id = 0;
  RRTime = 7;
}

CPhilosopher::CPhilosopher(mutex * _pMtx) {
  mtx = _pMtx;
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


#ifdef normalPhilosophers
void CPhilosopher::live() {
  //std::cout << "Philosopher " << id << " sits down at the table\n";
  printf("Philosopher %d sits down at the table\n", id);
  srand(time(NULL)*id);
  waittimer = std::rand() % 5;
  eatTimer = 3.0f;
  std::unique_lock<std::mutex> lock(mtx);
  hadDinner = false;

  while (1) {
    sleep_for(milliseconds(static_cast<unsigned int>(waittimer * 1000)));
    cv.wait(lock, [this]() { return(!rightFork->isInUse || !leftFork->isInUse); });
    if (!leftFork->isInUse) { // Maybe he eats
      pickUpLeftFork();
      if (rightFork->isInUse) {
        sleep_for(milliseconds(1000));
        if (!rightFork->isInUse) {
          pickUpRightFork();
          eat();
        }
      }
      else {
        pickUpRightFork();
        eat();
      }
      putDownLeftFork();
    }
    if (gotRightFork)
      putDownRightFork();
    if (gotLeftFork)
      putDownLeftFork();

    // Not eating right now
    
    // Reset for next loop
    srand(time(NULL) * id);
    if (hadDinner)
      waittimer = (rand() % 5) + 10;
    hadDinner = false;
  }
}

void CPhilosopher::live(int _milisecs) {}

void CPhilosopher::eat() {
  //std::cout << "Philosopher " << id << " started eating\n";
  printf("Philosopher %d started eating\n", id);
  sleep_for(milliseconds(static_cast<unsigned int>(eatTimer * 1000)));
  printf("Philosopher %d finished eating\n", id);
  //std::cout << "Philosopher " << id << " finished eating\n";
  putDownLeftFork();
  putDownRightFork();
  hadDinner = true;
}

void CPhilosopher::pickUpRightFork() {
  rightFork->pickUp();
  gotRightFork = true;
}

void CPhilosopher::pickUpLeftFork() {
  leftFork->pickUp();
  gotLeftFork = true;
}

void CPhilosopher::putDownRightFork() {
  rightFork->putDown();
  gotRightFork = false;
  sleep_for(milliseconds(500));

}

void CPhilosopher::putDownLeftFork() {
  leftFork->putDown();
  gotLeftFork = false;
  sleep_for(milliseconds(500));
}
#endif

#ifdef RRPhilosophers
void CPhilosopher::live() {
  // a philosopher must do its thing while his round robin time hasn't run out 
  // and when it runs out he stop then waits some time and continues
  //
  printf("Philosopher %d sits down at the table\n", id);
  srand(time(NULL)*id);
  
  RRTime = 3000;
  float rrT = RRTime;
  think_timer = (std::rand() % 5) * 500;
  eatTimer = 5000.0f;
  mtxLock = std::unique_lock<std::mutex>(*mtx, std::defer_lock);
  std::unique_lock<std::mutex> lock(*mtx);
  //std::unique_lock<std::mutex> lock_(mtx, std::defer_lock);
  //std::atomic_flag a_lock = ATOMIC_FLAG_INIT;
  hadDinner = false;
  bool didThinking = false;
  while (true) {
    /*
    if (RRTime > 0) {
      if (mtx->try_lock()) {
        mtx->lock();
        if (!rightFork->isInUse) {

          pickUpRightFork();
          if (!leftFork->isInUse) {
            if (RRTime >= eatTimer) {
              eat();
            }
            else {
              mtx->unlock();
            }
          }
        }
        else {
          sleep_for(milliseconds(10000));
          RRTime = 5000;
        }
      }
    }
    sleep_for(milliseconds(static_cast<int>(think_timer * 3)));
    RRTime -= think_timer;
    printf("Philosopher %d thinks for %g  seconds\n", id, think_timer * 0.001);
    think_timer = ((std::rand() % 5) * 500);
    */
    
  }
}

  
 

  void CPhilosopher::eat() {
    //std::unique_lock<std::mutex> lock_(*mtx, std::defer_lock);
    //hadDinner = true;
    //RRTime -= eatTimer;

    mtx->unlock();
    printf("Philosopher %d started eating\n", id);
    sleep_for(milliseconds(static_cast<int>(eatTimer)));
    printf("Philosopher %d finished eating\n", id);
    mtx->lock();
    putDownLeftFork();
    putDownRightFork();
    mtx->unlock();
  }

  void CPhilosopher::pickUpRightFork() {
    rightFork->pickUp();
    gotRightFork = true;
  }

  void CPhilosopher::pickUpLeftFork() {
    leftFork->pickUp();
    gotLeftFork = true;
  }

  void CPhilosopher::putDownRightFork() {
    rightFork->putDown();
    gotRightFork = false;

  }

  void CPhilosopher::putDownLeftFork() {
    leftFork->putDown();
    gotLeftFork = false;
  }
#endif

#ifdef SJFPhilosophers
#endif