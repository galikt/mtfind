#ifndef OM_MESSAGE_H
#define OM_MESSAGE_H

#include <cstdint>
#include <memory>
#include "om_chunk.h"
#include "om_visualizer_chunk.h"

enum OM_MsgType
{
  Blank,
  RequestChunk,
  ResponceChunk,
  VisualizerChunk,
  Done,
  Stop
};

class OM_Msg
{
public:
  OM_MsgType Type{OM_MsgType::Blank};
  uint32_t SenderId;

public:
  OM_Msg(OM_MsgType type);
  virtual ~OM_Msg() = default;
};

class OM_MsgRequestChunk : public OM_Msg
{
public:
  OM_MsgRequestChunk();
};

class OM_MsgResponceChunk : public OM_Msg
{
public:
  OM_MsgResponceChunk();

public:
  std::unique_ptr<OM_Chunk> Chunk;
};

class OM_MsgDone : public OM_Msg
{
public:
  OM_MsgDone();
};

class OM_MsgVisualizerChunk : public OM_Msg
{
public:
  OM_MsgVisualizerChunk();

public:
  std::unique_ptr<OM_VisualizerChunk> Chunk;
};

class OM_MsgStop : public OM_Msg
{
public:
  OM_MsgStop();
};

#endif // OM_MESSAGE_H
