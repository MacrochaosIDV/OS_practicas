#pragma once

class CFork;

class CPhilosopher
{
public:
  CPhilosopher();
  ~CPhilosopher() = default;

  void setForks(CFork* rf, CFork* lf);
  void setID(int _id);
  void live();

  CFork* rightFork;
  CFork* leftFork;
  float waittimer;
  float eatTimer;
  unsigned short id;
};

