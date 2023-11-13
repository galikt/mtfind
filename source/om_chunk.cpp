#include "om_chunk.h"

constexpr uint32_t OM_Chunk::MaxSize{1024};

OM_Chunk::OM_Chunk(const uint32_t index, const uint32_t first_line) : Index(index), FirstLine(first_line)
{
}
