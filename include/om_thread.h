#ifndef OM_THREAD_H
#define OM_THREAD_H

#include <functional>
#include <memory>
#include "om_wait.h"
#include "om_object.h"

class OM_Thread : public OM_Object
{
public:
  OM_Thread(const std::string& template_str);
  void Run();
  virtual void Wake() override;

protected:
  OM_Wait Wait;
  std::string Template;
  bool Proceed{true};

protected:
  void Thread();
  void Process(std::unique_ptr<OM_Msg>&& msg);
};

#endif // OM_THREAD_H
