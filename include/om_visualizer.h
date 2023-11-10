#ifndef OM_VISUALIZER_H
#define OM_VISUALIZER_H

class OM_Visualizer
{
public:
  OM_Visualizer();
  void Run();
  void PushChunk();

protected:
  //  std::shared_ptr<OM_Worker> Source;
};

#endif // OM_VISUALIZER_H
