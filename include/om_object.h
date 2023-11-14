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

class OM_Object : public std::enable_shared_from_this<OM_Object>
{
  friend class OM_MessageManager;

public:
  OM_Object(const uint32_t id = OM_ReservedId::None);
  virtual ~OM_Object();
  void PushMessage(std::unique_ptr<OM_Msg>&& msg);
  inline uint32_t GetId();
  virtual void Init();

protected:
  struct
  {
    std::mutex Lock;
    std::list<std::unique_ptr<OM_Msg>> List;
  } Message;

  static uint32_t CounterId;
  uint32_t Id{0};
  std::atomic<int> MessageCount{0};

protected:
  virtual void Wake() = 0;
};

uint32_t OM_Object::GetId()
{
  return Id;
}

#endif // OM_OBJECT_H
