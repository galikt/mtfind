#ifndef OM_WAIT_H
#define OM_WAIT_H

#include <mutex>
#include <condition_variable>
#include <atomic>
#include <chrono>

class OM_Wait
{
public:
  inline void Wait();
  inline void Wake();

private:
  std::mutex Mutex;
  std::condition_variable Condition;
};

void OM_Wait::Wait()
{
  std::unique_lock<std::mutex> lock(Mutex);
  Condition.wait(lock);
}

void OM_Wait::Wake()
{
  std::unique_lock<std::mutex> lock(Mutex);
  Condition.notify_all();
}

#endif // OM_WAIT_H
