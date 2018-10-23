#include <iostream>
#include <thread>
#include <mutex>
#include <vector>
#include <condition_variable>
#include <functional>
#include <windows.h>

#include "CPhilosopher.h"
#include "CFork.h"

static unsigned int arrI[10];
std::condition_variable cv;
std::mutex mtx;
//std::unique_lock<std::mutex> lock(mtx);

bool green;

//void asdf();

class CCounter
{
 public:
   CCounter() = default;
   ~CCounter() = default;
   void add(int i) {
     std::unique_lock<std::mutex> lock(mtx);
     cv.wait(lock, [this]() {
       return v.size() < 10;
     });

     v.push_back(i);

     lock.unlock();
     cv.notify_all();
   }

   int remove() {
     std::unique_lock<std::mutex> lock(mtx);

     cv.wait(lock, [this]() {return !v.empty();});
     //mtx.lock();
     int i = v.back();
     v.pop_back();
     //mtx.unlock();
     lock.unlock();
     cv.notify_all();
     return i;
   }

   std::vector<unsigned int> v;
   static unsigned int m_Cntarr[10];
};
class CProducer
{
 public:
   CProducer() = default;
   ~CProducer() = default;
   CProducer(CCounter* _counter) {
     m_counter = _counter;
   }

   void run() {
     while (1) {
       val = std::rand() % 100;
       std::cout << "Produced: " << val << std::endl;
       m_counter->add(val);
     }
   }

   unsigned int val;
   CCounter* m_counter;
};
class CConsumer
{
 public:
   CConsumer() = default;
   ~CConsumer() = default;
   CConsumer(CCounter* _counter) {
     m_counter = _counter;
   }

   void run() {
     while (1) {
       std::cout << "Consumed: " << m_counter->remove() << std::endl;
     }
   }

   CCounter* m_counter;
};
void producerCV() {

}
void consumerCV() {

}
void producerMutex() {
  unsigned int a = 0;
  while (1) {
    if (green) {
      //lock.lock();
      for (int i = 0; i < 10; ++i) {
        arrI[i] = a;
        ++a;
      }
      //lock.unlock();
    }
  }
}
void consumerMutex() {
  while (1) {
  
    if (green) {
      //lock.lock();
      for (int i = 0; i < 10; ++i) {
        std::cout << arrI[i] << std::endl;
      }
      //lock.unlock();
    }
  }
}
void producer2() {
  unsigned int a = 0;
  while (1)  {
    if (green) {
      green = !green;
      for (int i = 0; i < 10; ++i) {
        arrI[i] = a;
        ++a;
      }
      green = !green;
    }
  }
}
void consumer2() {
  while (1) {
    if (green) {
      green = !green;
      for (int i = 0; i < 10; ++i) {
        std::cout << arrI[i] << std::endl;
      }
      green = !green;
    }
  }
}
void hello() {
  std::cout << "Hellow World!\n";
}

void philosophers() {
  CPhilosopher P_alpha;
  CPhilosopher P_bravo;
  CPhilosopher P_charlie;
  CPhilosopher P_delta;
  CPhilosopher P_epsilon;
  CFork fa;
  CFork fb;
  CFork fc;
  CFork fd;
  CFork fe;
  P_alpha.setForks(&fa, &fb);
  P_bravo.setForks(&fb, &fc);
  P_charlie.setForks(&fc, &fd);
  P_delta.setForks(&fd, &fe);
  P_epsilon.setForks(&fe, &fa);
  P_alpha.setID(1);
  P_bravo.setID(2);
  P_charlie.setID(3);
  P_delta.setID(4);
  P_epsilon.setID(5);
  std::thread phil1(&CPhilosopher::live, &P_alpha);
  std::thread phil2(&CPhilosopher::live, &P_bravo);
  std::thread phil3(&CPhilosopher::live, &P_charlie);
  std::thread phil4(&CPhilosopher::live, &P_delta);
  std::thread phil5(&CPhilosopher::live, &P_epsilon);
  phil1.join();
  phil2.join();
  phil3.join();
  phil4.join();
  phil5.join();
}

int main() {
  //green = true;
  //asdf();
  //CCounter cnt;
  //CConsumer cns(&cnt);
  //CProducer prd(&cnt);
  //std::thread thr0(producerMutex);
  //std::thread thr0(&CProducer::run, &prd);
  //std::thread thr1(consumerMutex);
  //std::thread thr1(&CConsumer::run, &cns);
  //thr0.join();
  //thr1.join();

 

  //HMODULE dllfoo = LoadLibrary("D3DXFoo.dll");
  //HINSTANCE dllD3DXfoo = LoadLibraryEx("D3DXFoo.dll", nullptr, LOAD_WITH_ALTERED_SEARCH_PATH);
  //HINSTANCE dllOGLfoo = LoadLibraryEx("OGLFoo.dll", nullptr, LOAD_WITH_ALTERED_SEARCH_PATH);
  //std::function<void()> myfunction;

  //OGLFoo gfoo;
  //D3DXFoo dfoo;
  //myfunction = GetProcAddress(dllD3DXfoo, "foo");
  //myfunction = std::bind(&foo, nullptr);
  //myfunction = std::bind(GetProcAddress(dllD3DXfoo, "foo"));
  //void()* vpf = GetProcAddress(dllD3DXfoo, "D3DXFoo::foo");
  //myfunction = vpf;
  //myfunction = GetProcAddress(dllD3DXfoo, "D3DXFoo::foo");
  //myfunction = std::bind(GetProcAddress(dllD3DXfoo, "D3DXFoo::foo"));
  //auto myFunc = static_cast<void*>(GetProcAddress(dllD3DXfoo, "foo"));
  //auto myFunc = std::bind(dllOGLfoo, "foo");
  //myFunc;
  //myfunction();
  //std::cout << myfunction() << std::endl;
  //myfunction = GetProcAddress(dllOGLfoo, "foo");
  //myfunction = std::bind(GetProcAddress(dllOGLfoo, "foo"));
  //myfunction();

  //Sleep(2000);

  philosophers();

  return 0;
}