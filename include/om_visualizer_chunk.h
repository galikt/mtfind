#ifndef OM_VISUALIZERCHUNK_H
#define OM_VISUALIZERCHUNK_H

#include <cstdint>
#include <list>
#include <string>

class OM_VisualizerChunk
{
  friend class OM_Thread;
  friend class OM_Visualizer;

public:
  OM_VisualizerChunk();

private:
  struct OM_VisualizerChunkLine
  {
    uint32_t Line;
    uint32_t Position;
    std::string string;
  };

  uint32_t Index;
  std::list<OM_VisualizerChunkLine> List;
};

#endif // OM_VISUALIZERCHUNK_H
