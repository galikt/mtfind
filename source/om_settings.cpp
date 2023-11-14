#include "om_settings.h"

#include <thread>
#include <algorithm>
#include <fstream>

constexpr uint32_t CHUNK_SIZE{1024 * 128};

OM_Settings::OM_Settings(const int& argc, char** const argv)
{
  if (argc >= 3)
  {
    auto cores = std::thread::hardware_concurrency();
    Cores = cores == 0 ? 2 : cores;
    FileName = argv[1];

    std::string str = argv[2];
    std::replace(str.begin(), str.end(), '?', '.');
    Template = str;

    std::ifstream file(FileName.c_str());
    if (file.is_open())
    {
      long file_size;
      file.seekg(0, std::ios::end);
      file_size = file.tellg();
      file.close();

      // выставляем размер chunk для работы всех потоков
      uint32_t new_chunk_size = file_size / Cores;
      ChunkSize = new_chunk_size < CHUNK_SIZE ? new_chunk_size : CHUNK_SIZE;

      Valid = true;
    }
  }
}

OM_Settings::operator bool() const
{
  return Valid;
}
