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

private:
  uint32_t Index{0};
  uint32_t FirstLine{0};
  std::list<std::string> List;
};

void OM_Chunk::Push(std::string&& str)
{
  List.push_back(std::move(str));
}

#endif // OM_CHUNK_H
