#include "om_thread.h"

#include <thread>
#include <regex>

OM_Thread::OM_Thread(const std::string& template_str) : Template(template_str)
{
  std::thread thread(&OM_Thread::Thread, this);
  thread.detach();
}

void OM_Thread::Wake()
{
  Wait.Wake();
}

void OM_Thread::Thread()
{
  bool run{true};
  while (run)
  {
    // запрос даных у источника
    auto msg = std::make_unique<OM_MsgRequestChunk>();
    OM_MessageManager::GetInstance()->SendMsg(this, OM_ReservedId::Source, std::move(msg));

    // поток зысыпает до появления сообщений
    //    if (MessageCount.load() == 0)
    //    {
    Wait.Wait();
    //    }

    // обработка сообщений
    std::list<std::unique_ptr<OM_Msg>> message_list;
    {
      std::unique_lock<std::mutex> lock(Message.Lock);
      message_list = std::move(Message.List);
      MessageCount.store(0);
    }

    for (auto&& iter = message_list.begin(); iter != message_list.end(); ++iter)
    {
      switch ((*iter)->Type)
      {
        case OM_MsgType::ResponceChunk:
        {
          Process(std::move(*iter));
          break;
        }
        case OM_MsgType::Done:
        {
          run = false;
          break;
        }
      }
    }
    message_list.clear();
  }

  // уведомление Worker о завершении работы Thread
  auto msg = std::make_unique<OM_MsgDone>();
  OM_MessageManager::GetInstance()->SendMsg(this, OM_ReservedId::Worker, std::move(msg));
}

void OM_Thread::Process(std::unique_ptr<OM_Msg>&& msg)
{
  auto msg_responce = static_cast<OM_MsgResponceChunk*>(msg.get());
  auto msg_visualizer_chunk = std::make_unique<OM_MsgVisualizerChunk>();

  auto chunk = std::make_unique<OM_VisualizerChunk>();
  chunk->Index = msg_responce->Chunk->Index;

  std::regex regex(Template);
  uint32_t line_counter{0};
  OM_VisualizerChunk::OM_VisualizerChunkLine line;
  for (auto str : msg_responce->Chunk->List)
  {
    std::smatch match;
    regex_search(str, match, regex);

    std::string find = match[0];
    if (find.empty() == false)
    {
      line.Line = msg_responce->Chunk->FirstLine + line_counter;
      line.Position = match.position();
      line.string = find;
      chunk->List.push_back(std::move(line));
    }
    ++line_counter;
  }
  msg_visualizer_chunk->Chunk = std::move(chunk);

  OM_MessageManager::GetInstance()->SendMsg(this, OM_ReservedId::Visualizer, std::move(msg_visualizer_chunk));
}
