#include <string>
#include <memory>
#include <thread>
#include "om_source.h"
#include "om_worker.h"
#include "om_visualizer.h"

struct OM_Settings
{
  std::string File{""};
  std::string Template{""};
  uint32_t Cores{0};
};

bool InitSettings(const int argc, char** const argv, OM_Settings& settings)
{
  if (argc < 3)
  {
    return false;
  }

  settings.File = argv[1];
  settings.Template = argv[2];

  auto cores = std::thread::hardware_concurrency();
  settings.Cores = cores == 0 ? 2 : cores;

  return true;
}

int main(int argc, char** argv)
{
  OM_Settings settings;
  if (InitSettings(argc, argv, settings) == false)
  {
    return -1;
  }

  auto source = std::make_shared<OM_Source>(settings.File, settings.Cores);
  auto visualizer = std::make_shared<OM_Visualizer>();
  auto worker = std::make_unique<OM_Worker>(source, visualizer, settings.Cores);

  visualizer->Run();

  return 0;
}
