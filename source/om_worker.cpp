#include "om_worker.h"
#include <thread>
#include <memory>
#include "om_message_manager.h"

OM_Worker::OM_Worker(const uint32_t cores, const std::string& template_str) : OM_Object(OM_ReservedId::Worker), Cores(cores), Template(template_str)
{
}

void OM_Worker::Run()
{
  std::thread thread_worker(&OM_Worker::Thread, this);
  thread_worker.detach();

  for (int i = 0; i < Cores; ++i)
  {
    auto thread = std::make_shared<OM_Thread>(Template);
    thread->Init();
    thread->Run();
    ThreadList.push_back(std::move(thread));
  }
}

void OM_Worker::Thread()
{
  while (Proceed)
  {
    // сон если нет сообщений
    if (MessageCount.load() == 0)
    {
      Wait.Wait();
    }

    // обработка сообщений
    std::list<std::unique_ptr<OM_Msg>> message_list;
    {
      std::unique_lock<std::mutex> lock(Message.Lock);
      message_list = std::move(Message.List);
      MessageCount.store(0);
    }

    for (auto&& iter_msg = message_list.begin(); iter_msg != message_list.end(); ++iter_msg)
    {
      Process(std::move(*iter_msg));
    }
    message_list.clear();
  }

  // завершить работу Source
  auto msg_stop = std::make_unique<OM_MsgStop>();
  OM_MessageManager::GetInstance()->SendMsg(this, OM_ReservedId::Source, std::move(msg_stop));

  // уведомление Visualizer о завершении работы Worker
  auto msg_done = std::make_unique<OM_MsgDone>();
  OM_MessageManager::GetInstance()->SendMsg(this, OM_ReservedId::Visualizer, std::move(msg_done));
}

void OM_Worker::Process(std::unique_ptr<OM_Msg>&& msg)
{
  switch (msg->Type)
  {
    case OM_MsgType::Done:
    {
      auto iter = ThreadList.begin();
      while (iter != ThreadList.end())
      {
        if ((*iter)->GetId() == msg->SenderId)
        {
          iter = ThreadList.erase(iter);
          break;
        }
        else
        {
          ++iter;
        }
      }

      if (ThreadList.empty())
      {
        Proceed = false;
      }
      break;
    }
  }
}

void OM_Worker::Wake()
{
  Wait.Wake();
}
