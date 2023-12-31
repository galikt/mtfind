#include "om_object.h"

uint32_t OM_Object::CounterId{OM_ReservedId::Max};

OM_Object::OM_Object(const uint32_t id)
{
  if (id == OM_ReservedId::None)
  {
    Id = CounterId;
    ++CounterId;
  }
  else
  {
    Id = id;
  }
}

OM_Object::~OM_Object()
{
  OM_MessageManager::GetInstance()->UnRegisterObject(shared_from_this());
}

void OM_Object::Init()
{
  OM_MessageManager::GetInstance()->RegisterObject(shared_from_this());
}

void OM_Object::PushMessage(std::unique_ptr<OM_Msg>&& msg)
{
  std::unique_lock<std::mutex> lock(Message.Lock);
  Message.List.push_back(std::move(msg));
  MessageCount.store(Message.List.size());
  Wake();
}
