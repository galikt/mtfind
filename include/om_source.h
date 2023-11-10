#ifndef OM_SOURCE_H
#define OM_SOURCE_H

#include <stdint.h>
#include <string>
#include <list>
#include <memory>

#include <mutex>
//#include <condition_variable>
//#include <atomic>

#include "om_wait.h"

// constexpr uint32_t CHUNK_SIZE{1 * (1024 * 1024)};
constexpr uint64_t CHUNK_SIZE{1024};

struct OM_Сhunk
{
  uint32_t Index;
  uint32_t FirstLine;
  std::list<std::string> List;
};

class OM_Source
{
public:
  OM_Source(const std::string& file_name, const uint32_t max_chunk);

public:
  std::unique_ptr<OM_Сhunk> PullChunk();

protected:
  std::string FileName;
  std::list<std::unique_ptr<OM_Сhunk>> Cache;
  uint32_t MaxChunk;
  uint32_t CountChunk{0};
  bool Done{false};

  OM_Wait Wait;
  std::mutex MutexData;

protected:
  void Thread();
};

#endif // OM_SOURCE_H
