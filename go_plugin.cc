
#include "src/compiler/config.h"
#include "src/compiler/protobuf_plugin.h"
#include "src/compiler/go_generator.h"

int main(int argc, char* argv[]) {
  sapphire_go_generator::GoSapphireGenerator generator();
  return grpc::protobuf::compiler::PluginMain(argc, argv, &generator);
}
