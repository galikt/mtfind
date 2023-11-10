#include "om_source.h"
#include <thread>
#include <fstream>

OM_Source::OM_Source(const std::string& file_name, const uint32_t max_chunk) : FileName(file_name), MaxChunk(max_chunk)
{
  std::thread thread(&OM_Source::Thread, this);
  thread.detach();
}

std::unique_ptr<OM_Сhunk> OM_Source::PullChunk()
{
  std::unique_lock<std::mutex> lock(MutexData);

  if (Done == true)
  {
    return std::unique_ptr<OM_Сhunk>(nullptr);
  }

  auto result(std::move(*Cache.begin()));
  Cache.pop_front();

  Wait.Continue();

  return result;
}

void OM_Source::Thread()
{
  std::ifstream file(FileName.c_str());
  if (file.is_open())
  {
    auto chunk = std::make_unique<OM_Сhunk>();
    uint64_t filling_chunk{0};

    std::string buf;
    uint32_t line_counter{0};
    uint32_t index_counter{0};
    for (;;)
    {
      if (std::getline(file, buf))
      {
        ++line_counter;

        if (filling_chunk > CHUNK_SIZE)
        {
          {
            std::unique_lock<std::mutex> lock(MutexData);
            Cache.push_back(std::move(chunk));
          }

          chunk = std::make_unique<OM_Сhunk>();
          chunk->FirstLine = line_counter;
          chunk->Index = index_counter;

          filling_chunk = 0;
          ++CountChunk;
          ++index_counter;
        }

        filling_chunk += buf.size();
        chunk->List.push_back(std::move(buf));
      }
      else
      {
        {
          std::unique_lock<std::mutex> lock(MutexData);
          Cache.push_back(std::move(chunk));
          Done = true;
        }
        ++CountChunk;

        break;
      }

      if (CountChunk > MaxChunk)
      {
        Wait.Wait();
      }
    }
    file.close();
  }
}
