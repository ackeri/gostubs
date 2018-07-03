#include <google/protobuf/compiler/plugin.h>
#include "go_generator.h"

int main(int argc, char* argv[]) {
  sapphire_go_generator::GoSapphireGenerator generator;
  return google::protobuf::compiler::PluginMain(argc, argv, &generator);
}
