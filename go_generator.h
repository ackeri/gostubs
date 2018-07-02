#ifndef SAPPHIRE_GO_STUB_GENERATOR_H
#define SAPPHIRE_GO_STUB_GENERATOR_H

#include <utility>

#include "src/compiler/config.h"
#include "src/compiler/schema_interface.h"

namespace sapphire_go_generator {

class GoSapphireGenerator : public grpc::protobuf::compiler::CodeGenerator {
 public:
  GoSapphireGenerator();
  ~GoSapphireGenerator();

  bool Generate(const grpc::protobuf::FileDescriptor* file,
                const grpc::string& parameter,
                grpc::protobuf::compiler::GeneratorContext* context,
                grpc::string* error) const;

 private:
};

}  // namespace grpc_python_generator

#endif  // SAPPHIRE_GO_STUB_GENERATOR_H
