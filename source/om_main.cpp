#include "om_source.h"
#include "om_worker.h"
#include "om_visualizer.h"
#include "om_settings.h"

int main(int argc, char** argv)
{
  std::setlocale(LC_ALL, "en_US.US-ASCII");

  OM_Settings settings(argc, argv);
  if (settings == false)
  {
    return -1;
  }

  auto source = std::make_shared<OM_Source>(settings.FileName, settings.Cores, settings.ChunkSize);
  source->Init();

  auto worker = std::make_shared<OM_Worker>(settings.Cores, settings.Template);
  worker->Init();

  auto visualizer = std::make_shared<OM_Visualizer>();
  visualizer->Init();

  source->Run();
  worker->Run();
  visualizer->Run();

  return 0;
}
