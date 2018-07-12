#include <google/protobuf/compiler/plugin.h>

#include <string>

#include "go/generator.h"

//Interface for Protoc to call our generator
int main(int argc, char* argv[]) {
  GoSapphireGenerator generator;
  return google::protobuf::compiler::PluginMain(argc, argv, &generator);
}
