#include "om_worker.h"
#include <thread>

OM_Worker::OM_Worker(std::shared_ptr<OM_Source> source, std::shared_ptr<OM_Visualizer> receiver, const uint32_t cores) : Source(source), Receiver(receiver)
{
  for (int i = 0; i < cores; ++i)
  {
    std::thread thread(&OM_Worker::Thread, this);
    thread.detach();
  }
}

void OM_Worker::Thread()
{

  auto chunk = Source->PullChunk();
  if (chunk.get() == nullptr)
  {
  }
  Receiver->PushChunk();
}
