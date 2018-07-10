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

class Generator : public google::protobuf::compiler::CodeGenerator {
 public:
  Generator() {};
  ~Generator() {};

  string MethodToProto(MethodDescriptor* md);
  string ProtoToMethod(string id);
  
  string StructToProto(Descriptor* d);
  string ProtoToStruct(string id);

  string FieldToProto(FieldDescriptor* md);
  string ProtoToField(string id);

  void GenerateType(const FieldDescriptor* fd);

  void GenerateMethod(const MethodDescriptor* md, Printer* out);
  void GenerateMethodHeader(const MethodDescriptor* md, Printer* out);
  void GenerateMethodFooter(const MethodDescriptor* md, Printer* out);
  
  void GeneratePackHeader(const Descriptor* d, Printer* out);
  void GeneratePackFooter(const Descriptor* d, Printer* out);
  void GeneratePackField(const FieldDescriptor* fd, Printer* out);

  void GenerateUnpackHeader(const Descriptor* d, Printer* out);
  void GenerateUnpackFooter(const Descriptor* d, Printer* out);
  void GenerateUnpackField(const FieldDescriptor* fd, Printer* out);

  void GenerateKernelCall(const MethodDescriptor* md, Printer* out);

  void GenerateForFile(const FileDescriptor* fd);
  void GenerateFileHeader(const FileDescriptor* fd);
  void GenerateFileFooter(const FileDescriptor* fd);

  void GenerateStubs(GeneratorContext* context);

  bool Generate(const FileDescriptor* file,
                const string& parameter,
                GeneratorContext* context,
                string* error) const;

  bool GenerateAll(const vector<const FileDescriptor*>& files, const string& parameter, GeneratorContext* context, string * error) const {
	for(auto f : files) {
      if(!Generate(f, parameter, context, error)) {
        return false;
      }
    }
	return true;
  };
  bool HasGenerateAll() const { return true; }
 private:

};

}  // namespace sapphire_go_generator

#endif  // SAPPHIRE_GO_STUB_GENERATOR_H
