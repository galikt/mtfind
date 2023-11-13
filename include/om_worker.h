#ifndef OM_WORKER_H
#define OM_WORKER_H

#include "om_object.h"
#include "om_message.h"
#include "om_thread.h"
#include "om_wait.h"

class OM_Worker : public OM_Object
{
public:
  OM_Worker(const uint32_t cores, const std::string& template_str);

protected:
  std::list<std::shared_ptr<OM_Thread>> ThreadList;
  OM_Wait Wait;
  bool Run{true};

protected:
  void Thread();
  void Process(std::unique_ptr<OM_Msg>&& msg);
  virtual void Wake() override;
};

#endif // OM_WORKER_H
