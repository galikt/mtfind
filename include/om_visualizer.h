#ifndef OM_VISUALIZER_H
#define OM_VISUALIZER_H

#include "om_object.h"
#include "om_wait.h"

class OM_Visualizer : public OM_Object
{
public:
  OM_Visualizer();
  void Run();

protected:
  OM_Wait Wait;
  std::list<std::unique_ptr<OM_VisualizerChunk>> ChunkList;

protected:
  virtual void Wake() override;
  bool Proceed{true};
};

#endif // OM_VISUALIZER_H
