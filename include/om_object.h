#ifndef OM_OBJECT_H
#define OM_OBJECT_H

#include <cstdint>
#include <list>
#include <memory>
#include <mutex>
#include <atomic>
#include <om_message_manager.h>

// todo заменить на enum class
enum OM_ReservedId
{
  None = 0,
  Source,
  Worker,
  Visualizer,
  Max
};

class OM_Object
{
  friend class OM_MessageManager;

public:
  OM_Object(const uint32_t id = OM_ReservedId::None);
  virtual ~OM_Object() = default;
  void PushMessage(std::unique_ptr<OM_Msg>&& msg);
  inline uint32_t GetId();

protected:
  struct
  {
    std::mutex Lock;
    std::list<std::unique_ptr<OM_Msg>> List;
  } Message;

  static uint32_t CounterId;
  uint32_t Id;
  std::atomic<int> MessageCount;

protected:
  virtual void Wake() = 0;
};

uint32_t OM_Object::GetId()
{
  return Id;
}

#endif // OM_OBJECT_H
