#ifndef GO_SAPPHIRE_GENERATOR_H
#define GO_SAPPHIRE_GENERATOR_H

#include <google/protobuf/compiler/plugin.h>
#include <google/protobuf/compiler/code_generator.h>

#include <string>

#include "generator_base.h"

using namespace std;
using google::protobuf::FileDescriptor;
using google::protobuf::compiler::GeneratorContext;

class GoSapphireGenerator : public BaseSapphireGenerator {
 public:  
  void GenerateSapphireStubs(GeneratorContext* context, string base, const FileDescriptor* file) const;
};

#endif  // GO_SAPPHIRE_GENERATOR_H
