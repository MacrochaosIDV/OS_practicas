#pragma once
class CFork
{
public:
  CFork();
  ~CFork() = default;

  void pickUp();
  void putDown();

  bool isInUse;
};

