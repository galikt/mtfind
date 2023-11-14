#ifndef OM_MESSAGE_MANAGER_H
#define OM_MESSAGE_MANAGER_H

#include <cstdint>
#include <memory>
#include <map>
#include <mutex>
#include "om_message.h"

class OM_Object;

class OM_MessageManager
{
public:
  static OM_MessageManager* GetInstance();
  void SendMsg(const OM_Object* sender, uint32_t receiver, std::unique_ptr<OM_Msg>&& msg);
  void RegisterObject(std::shared_ptr<OM_Object> obj);
  void UnRegisterObject(std::shared_ptr<OM_Object> obj);

protected:
  struct
  {
    std::mutex Lock;
    std::map<uint32_t, std::shared_ptr<OM_Object>> Map;
  } Object;

protected:
  static OM_MessageManager* Instance;
};

#endif // OM_MESSAGE_MANAGER_H
