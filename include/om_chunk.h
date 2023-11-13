#ifndef OM_CHUNK_H
#define OM_CHUNK_H

#include <cstdint>
#include <list>
#include <string>

class OM_Chunk
{
  friend class OM_Thread;

public:
  OM_Chunk(const uint32_t index, const uint32_t first_line);
  inline void Push(std::string&& str);
  inline bool IsFull();

private:
  static const uint32_t MaxSize;
  uint32_t Index{0};
  uint32_t FirstLine{0};
  uint32_t Filling{0};
  std::list<std::string> List;
};

void OM_Chunk::Push(std::string&& str)
{
  Filling += str.size();
  List.push_back(std::move(str));
}

bool OM_Chunk::IsFull()
{
  return Filling > MaxSize ? true : false;
}

#endif // OM_CHUNK_H
