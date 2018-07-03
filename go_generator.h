#ifndef SAPPHIRE_GO_STUB_GENERATOR_H
#define SAPPHIRE_GO_STUB_GENERATOR_H

#include <utility>
#include <string>

#include <google/protobuf/compiler/plugin.h>
#include <google/protobuf/compiler/code_generator.h>

using namespace std;

namespace sapphire_go_generator {

class GoSapphireGenerator : public google::protobuf::compiler::CodeGenerator {
 public:
  GoSapphireGenerator();
  ~GoSapphireGenerator();

  bool Generate(const google::protobuf::FileDescriptor* file,
                const string& parameter,
                google::protobuf::compiler::GeneratorContext* context,
                string* error) const;

 private:

};

}  // namespace sapphire_go_generator

#endif  // SAPPHIRE_GO_STUB_GENERATOR_H
