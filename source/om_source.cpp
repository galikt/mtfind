#include "om_source.h"
#include <fstream>
#include <thread>
#include "om_message_manager.h"

OM_Source::OM_Source(const std::string& file_name, const uint32_t max_chunk) : FileName(file_name), MaxChunk(max_chunk), OM_Object(OM_ReservedId::Source)
{
  std::thread thread(&OM_Source::Thread, this);
  thread.detach();
}

void OM_Source::ReadChunk(std::ifstream& file, uint32_t& line_counter, uint32_t& index_counter)
{
  auto chunk = std::make_unique<OM_Chunk>(index_counter, line_counter);
  ++index_counter;
  std::string buf;
  while (chunk->IsFull() == false)
  {
    if (std::getline(file, buf))
    {
      chunk->Push(std::move(buf));
      ++line_counter;
    }
    else
    {
      file.close();
      SourceValid = false;
      break;
    }
  }
  Cache.push_back(std::move(chunk));
}

void OM_Source::Thread()
{
  std::ifstream file(FileName.c_str());
  if (file.is_open() == false)
  {
    return;
  }

  uint32_t line_counter{1};
  uint32_t index_counter{0};
  bool run{true};
  while (run)
  {
    // пополнение Cache
    if (SourceValid)
    {
      ReadChunk(file, line_counter, index_counter);
    }

    // обработка сообщений
    {
      std::unique_lock<std::mutex> lock_message(Message.Lock);
      for (auto iter_msg = Message.List.begin(); iter_msg != Message.List.end(); ++iter_msg)
      {
        switch ((*iter_msg)->Type)
        {
          case OM_MsgType::RequestChunk:
          {
            auto iter_chunk = Cache.begin();
            if (iter_chunk != Cache.end())
            {
              auto msg = std::make_unique<OM_MsgResponceChunk>();
              msg->Chunk = std::move((*iter_chunk));
              OM_MessageManager::GetInstance()->SendMsg(this, (*iter_msg)->SenderId, std::move(msg));

              iter_msg = Message.List.erase(iter_msg);
              Cache.erase(iter_chunk);
            }
            else if (SourceValid == false)
            {
              // уведомление Thread о завершонной работе Source
              auto msg = std::make_unique<OM_MsgDone>();
              OM_MessageManager::GetInstance()->SendMsg(this, (*iter_msg)->SenderId, std::move(msg));

              iter_msg = Message.List.erase(iter_msg);
            }
            break;
          }
          case OM_MsgType::Stop:
          {
            run = false;
            break;
          }
        }
      }
      MessageCount.store(Message.List.size());
    }

    // поток засыпает если Cache полон или файл прочитан
    if ((Cache.size() > MaxChunk) || (SourceValid == false))
    {
      Wait.Wait();
    }
  }
}

void OM_Source::Wake()
{
  Wait.Wake();
}
