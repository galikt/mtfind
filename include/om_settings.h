#ifndef OM_SETTINGS_H
#define OM_SETTINGS_H

#include <cstdint>
#include <string>

class OM_Settings
{
public:
  OM_Settings(const int &argc, char** const argv);
  operator bool() const;

public:
  std::string FileName{""};
  std::string Template{""};
  uint32_t Cores{0};
  uint32_t ChunkSize{0};
  bool Valid{false};
};

#endif // OM_SETTINGS_H
