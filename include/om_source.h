#ifndef OM_SOURCE_H
#define OM_SOURCE_H

#include <stdint.h>
#include <list>
#include <memory>
#include <string>
#include "om_wait.h"
#include "om_object.h"
#include "om_chunk.h"

class OM_Source : public OM_Object
{
public:
  OM_Source(const std::string& file_name, const uint32_t max_chunk);

protected:
  std::list<std::unique_ptr<OM_Chunk>> Cache;
  std::string FileName;
  uint32_t MaxChunk;
  bool SourceValid{true};
  OM_Wait Wait;

protected:
  void Thread();
  void ReadChunk(std::ifstream& file, uint32_t& line_counter, uint32_t& index_counter);
  virtual void Wake() override;
};

#endif // OM_SOURCE_H
