#include "CPhilosopher.h"
#include <iostream>
#include <mutex>
#include <condition_variable>

#include "CFork.h"

using std::this_thread::sleep_for;
using std::chrono::milliseconds;




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
  //std::cout << "Philosopher " << id << " sits down at the table\n";
  printf("Philosopher %d sits down at the table\n", id);
  srand(time(NULL)-id);
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
