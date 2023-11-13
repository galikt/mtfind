#include <string>
#include <memory>
#include <thread>
#include <algorithm>
#include "om_source.h"
#include "om_worker.h"
#include "om_visualizer.h"
#include "om_message_manager.h"

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

  auto cores = std::thread::hardware_concurrency();
  settings.Cores = cores == 0 ? 2 : cores;

  settings.File = argv[1];

  std::string str = argv[2];
  std::replace(str.begin(), str.end(), '?', '.');
  settings.Template = str;

  return true;
}

int main(int argc, char** argv)
{
  std::setlocale(LC_ALL, "en_US.US-ASCII");

  OM_Settings settings;
  if (InitSettings(argc, argv, settings) == false)
  {
    return -1;
  }

  auto source = std::make_shared<OM_Source>(settings.File, settings.Cores);
  OM_MessageManager::GetInstance()->RegisterObject(source);

  auto worker = std::make_shared<OM_Worker>(settings.Cores, settings.Template);
  OM_MessageManager::GetInstance()->RegisterObject(worker);

  auto visualizer = std::make_shared<OM_Visualizer>();
  OM_MessageManager::GetInstance()->RegisterObject(visualizer);
  visualizer->Run();

  return 0;
}
