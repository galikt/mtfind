#include "om_visualizer.h"

#include <iostream>
#include <algorithm>

OM_Visualizer::OM_Visualizer() : OM_Object(OM_ReservedId::Visualizer)
{
}

void OM_Visualizer::Run()
{
  bool run{true};
  while (run)
  {
    std::list<std::unique_ptr<OM_Msg>> message_list;
    {
      std::unique_lock<std::mutex> lock(Message.Lock);
      message_list = std::move(Message.List);
      MessageCount.store(0);
    }

    for (auto iter_msg = message_list.begin(); iter_msg != message_list.end(); ++iter_msg)
    {
      switch ((*iter_msg)->Type)
      {
        case OM_MsgType::VisualizerChunk:
        {
          auto msg_visualizer_chunk = static_cast<OM_MsgVisualizerChunk*>(iter_msg->get());
          ChunkList.push_back(std::move(msg_visualizer_chunk->Chunk));
          break;
        }
        case OM_MsgType::Done:
        {
          run = false;
          break;
        }
      }
    }

    //    if (MessageCount.load() == 0)
    //    {
    Wait.Wait();
    //    }
  }

  // сортировка по id
  auto comparator = [](const std::unique_ptr<OM_VisualizerChunk>& v1, const std::unique_ptr<OM_VisualizerChunk>& v2) -> bool { return v1->Index < v2->Index; };
  ChunkList.sort(comparator);

  uint32_t total{0};
  for (auto& chunk : ChunkList)
  {
    total += chunk->List.size();
  }

  std::cout << total << std::endl;

  for (auto& chunk : ChunkList)
  {
    for (auto& line : chunk->List)
    {
      std::cout << line.Line << " " << line.Position << " " << line.string << std::endl;
    }
  }
}

void OM_Visualizer::Wake()
{
  Wait.Wake();
}
