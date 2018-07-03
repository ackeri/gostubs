#ifndef SAPPHIRE_GO_STUB_GENERATOR_H
#define SAPPHIRE_GO_STUB_GENERATOR_H

#include <utility>
#include <string>

#include <google/protobuf/compiler/plugin.h>
#include <google/protobuf/compiler/code_generator.h>

using namespace std;
using google::protobuf::FileDescriptor;
using google::protobuf::compiler::GeneratorContext;

namespace sapphire_go_generator {

class GoSapphireGenerator : public google::protobuf::compiler::CodeGenerator {
 public:
  GoSapphireGenerator() {};
  ~GoSapphireGenerator() {};

  bool Generate(const FileDescriptor* file,
                const string& parameter,
                GeneratorContext* context,
                string* error) const;

  bool GenerateAll(const vector<const FileDescriptor*>& files, const string& parameter, GeneratorContext* generator_context, string * error) const {return false;};
  bool HasGenerateAll() const { return false; }
 private:

};

}  // namespace sapphire_go_generator

#endif  // SAPPHIRE_GO_STUB_GENERATOR_H
