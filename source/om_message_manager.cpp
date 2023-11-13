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
  auto iter_obj = ObjectMap.find(receiver);
  if (iter_obj != ObjectMap.end())
  {
    msg->SenderId = sender == nullptr ? OM_MsgType::Blank : sender->Id;
    iter_obj->second->PushMessage(std::move(msg));
  }
}

void OM_MessageManager::RegisterObject(std::shared_ptr<OM_Object> obj)
{
  ObjectMap.insert(std::pair(obj->Id, obj));
}
