#ifndef OM_WAIT_H
#define OM_WAIT_H

#include <mutex>
#include <condition_variable>
#include <atomic>

class OM_Wait
{
public:
  inline void Wait();
  inline void Continue();

private:
  std::mutex Mutex;
  std::condition_variable Condition;
  std::atomic<bool> Run{false};
};

void OM_Wait::Wait()
{
  std::unique_lock<std::mutex> lock(Mutex);

  while (Run == false)
  {
    Condition.wait(lock);
  }
}

void OM_Wait::Continue()
{
  std::unique_lock<std::mutex> lock(Mutex);
  Run = true;
  Condition.notify_all();
}

#endif // OM_WAIT_H
