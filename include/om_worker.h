#ifndef OM_WORKER_H
#define OM_WORKER_H

#include "om_source.h"
#include "om_visualizer.h"

class OM_Worker
{
public:
  OM_Worker(std::shared_ptr<OM_Source> source, std::shared_ptr<OM_Visualizer> receiver, const uint32_t cores);

protected:
  std::shared_ptr<OM_Source> Source;
  std::shared_ptr<OM_Visualizer> Receiver;

protected:
  void Thread();
};

#endif // OM_WORKER_H
