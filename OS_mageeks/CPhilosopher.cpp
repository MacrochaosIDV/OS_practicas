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

CPhilosopher::CPhilosopher(const CPhilosopher & other) {
  mtx = other.mtx;
  rightFork = other.rightFork;
  leftFork = other.leftFork;
  waittimer = other.waittimer;
  eatTimer = other.eatTimer;
  id = other.id;
}



void CPhilosopher::setForks(CFork* rf, CFork* lf) {
  rightFork = rf;
  leftFork = lf;
}

void CPhilosopher::setID(int _id) {
  id = _id;
}

float f_min(float n1, float n2) {
  return(n1 < n2) ? n1 : n2;
}
float f_min3(float n1, float n2, float n3) {
  if (n1 < n2) {
    if (n1 < n3)
      return n1;
    else
      return n3;
  }
  else {
    if (n2 < n3)
      return n2;
    else
      return n3;
  }
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
    srand(time(NULL)*(id + 1));
    //rrT is the one that counts down & is compared against RRTime
    float rrT = RRTime = 3000;
    eatTimer = 5000.0f;
    sleep_for(milliseconds(1000));
    //////// while starts ////////
    while (true) {

      hadDinner = false;
      think_timer = ((std::rand() % 5) + 1) * 500;

      if (RRTime > 0) {
        {/////// lock the threads /////// 
          std::unique_lock<std::mutex> lock(*mtx);
          if (!rightFork->isInUse) {
            pickUpRightFork();
            if (!leftFork->isInUse) {
              pickUpLeftFork();
              hadDinner = true;
              
            }
            else {
              putDownRightFork();
            }
          }
        }////// unlock the threads ////// 
        if (hadDinner) {
          eat();
          rrT -= eatTimer;
        }
      }
      ////// while end //////
      printf("Philosopher %d is now thinking\n", id);
      sleep_for(milliseconds(static_cast<int>(think_timer)));
      if (rrT <= 0) {
        printf("Philosopher %d is now resting \n", id);
        sleep_for(milliseconds(static_cast<int>(think_timer * 9)));
        rrT = RRTime;
      }
    }
  }
  
  void CPhilosopher::eat() {
    
    {
      printf("Philosopher %d eats\n", id);
      sleep_for(milliseconds(static_cast<int>(eatTimer)));
      {
        std::unique_lock<std::mutex> lock(*mtx);
        putDownLeftFork();
        putDownRightFork();
      }
      printf("Philosopher %d finished eating\n", id);
      
    }
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
  void CPhilosopher::live() {
    // a philosopher must do its thing while his round robin time hasn't run out 
    // and when it runs out he stop then waits some time and continues
    //
    printf("Philosopher %d sits down at the table\n", id);
    srand(time(NULL)*(id + 1));
    //rrT is the one that counts down & is compared against RRTime
    eatTimer = ((rand() % 40) + 30) * 100;
    waittimer = ((rand() % 40) + 30) * 100;
    think_timer = ((rand() % 40) + 30) * 100;
    hadDinner = false;
    bool didTheThinking = false;
    bool didTheWaiting = false;
    sleep_for(milliseconds(1000));
    //////// while starts ////////
    while (true) {
      while (!hadDinner || !didTheThinking || !didTheWaiting) {
        if (eatTimer == f_min3(eatTimer, think_timer, waittimer)) {
          {
            std::unique_lock<std::mutex> lock(*mtx);
            if (!rightFork->isInUse) {
              pickUpRightFork();
              if (!leftFork->isInUse) {
                pickUpLeftFork();
                hadDinner = true;
              }
              else {
                putDownRightFork();
              }
            }
          }
        }
        else if (waittimer == f_min3(eatTimer, think_timer, waittimer)) {
          printf("Philosopher %d is resting\n", id);
          sleep_for(milliseconds(static_cast<int>(waittimer)));
          didTheWaiting = true;
          waittimer = 8000;
        }
        else if(think_timer == f_min3(eatTimer, think_timer, waittimer)) {
          printf("Philosopher %d is thinking\n", id);
          sleep_for(milliseconds(static_cast<int>(think_timer)));
          didTheThinking = true;
          think_timer = 8000;
        }
        else {
          sleep_for(milliseconds(1500));
        }

        if (hadDinner) {
          eat();
          eatTimer = 8000;
        }
      }
      printf("Philosopher %d did all his tasks, now reseting\n", id);
      // reset for loop
      hadDinner = didTheThinking = didTheWaiting = false;
      eatTimer = ((rand() % 40) + 80) * 100;
      waittimer = ((rand() % 40) + 80) * 100;
      think_timer = ((rand() % 40) + 80) * 100;
    }
  }

  void CPhilosopher::eat() {

    {
      printf("Philosopher %d eats\n", id);
      sleep_for(milliseconds(static_cast<int>(eatTimer)));
      {
        std::unique_lock<std::mutex> lock(*mtx);
        putDownLeftFork();
        putDownRightFork();
      }
      printf("Philosopher %d finished eating\n", id);

    }
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