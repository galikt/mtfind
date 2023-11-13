#include "om_message.h"

OM_Msg::OM_Msg(OM_MsgType type) : Type(type)
{
}

OM_MsgRequestChunk::OM_MsgRequestChunk() : OM_Msg(OM_MsgType::RequestChunk)
{
}

OM_MsgResponceChunk::OM_MsgResponceChunk() : OM_Msg(OM_MsgType::ResponceChunk)
{
}

OM_MsgDone::OM_MsgDone() : OM_Msg(OM_MsgType::Done)
{
}

OM_MsgVisualizerChunk::OM_MsgVisualizerChunk() : OM_Msg(OM_MsgType::VisualizerChunk)
{
}

OM_MsgStop::OM_MsgStop() : OM_Msg(OM_MsgType::Stop)
{
}
