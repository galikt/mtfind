#include "om_message_manager.h"
#include "om_object.h"

// todo не очищаетчя память
OM_MessageManager* OM_MessageManager::Instance{new OM_MessageManager()};

OM_MessageManager* OM_MessageManager::GetInstance()
{
  return Instance;
}

void OM_MessageManager::SendMsg(const OM_Object* sender, uint32_t receiver, std::unique_ptr<OM_Msg>&& msg)
{
  std::unique_lock<std::mutex>(Object.Lock);

  auto iter_obj = Object.Map.find(receiver);
  if (iter_obj != Object.Map.end())
  {
    msg->SenderId = sender == nullptr ? OM_MsgType::Blank : sender->Id;
    iter_obj->second->PushMessage(std::move(msg));
  }
}

void OM_MessageManager::RegisterObject(std::shared_ptr<OM_Object> obj)
{
  std::unique_lock<std::mutex>(Object.Lock);
  Object.Map.insert(std::pair(obj->Id, obj));
}

void OM_MessageManager::UnRegisterObject(std::shared_ptr<OM_Object> obj)
{
  std::unique_lock<std::mutex>(Object.Lock);
  auto find = Object.Map.find(obj->Id);
  if (find != Object.Map.end())
  {
    Object.Map.erase(find);
  }
}
