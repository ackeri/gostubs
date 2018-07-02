#include <algorithm>
#include <cassert>
#include <cctype>
#include <cstring>
#include <fstream>
#include <iostream>
#include <map>
#include <memory>
#include <ostream>
#include <set>
#include <sstream>
#include <tuple>
#include <vector>

#include "src/compiler/config.h"
#include "src/compiler/generator_helpers.h"
#include "src/compiler/protobuf_plugin.h"
#include "src/compiler/python_generator.h"
#include "src/compiler/python_generator_helpers.h"
#include "src/compiler/python_private_generator.h"

using grpc::protobuf::FileDescriptor;
using grpc::protobuf::compiler::GeneratorContext;
using grpc::protobuf::io::CodedOutputStream;
using grpc::protobuf::io::ZeroCopyOutputStream;
using std::make_pair;
using std::map;
using std::pair;
using std::replace;
using std::set;
using std::tuple;
using std::vector;

namespace sapphire_go_generator {

grpc::string generator_file_name;

namespace {

typedef map<grpc::string, grpc::string> StringMap;
typedef vector<grpc::string> StringVector;
typedef tuple<grpc::string, grpc::string> StringPair;
typedef set<StringPair> StringPairSet;

grpc::string GetGoType() {
	return "uint32";
}

void PrintAllComments(StringVector comments, grpc_generator::Printer* out) {
  for (auto it = comments.begin(); it != comments.end(); ++it) {
    out->Print("\\\\");
    size_t start_pos = it->find_first_not_of(' ');
    if (start_pos != grpc::string::npos) {
      out->PrintRaw(it->c_str() + start_pos);
    }
    out->Print("\n");
  }
}

void GetMethod(grpc::string name, grpc_generator::Printer* out, grpc_generator::Method* method) {
  StringMap methoddef_dict;
  methoddef_dict["name"] = name;
  methoddef_dict["method"] = method->name();

  // Function Header
  PrintAllComments(method->GetAllComments(), out);
  out->Print("func (o $name$) $method$(");

  auto args = method->input_type();
  for(int i = 0; i < args->field_count; ++i) {
    methoddef_dict["index"] = std::to_string(i);
    methoddef_dict["type"] = GetGoType(args->field(i));
	methoddef_dict["comma"] = (i == args->field_count - 1) ? "" : ",";
	out->Print(methoddef_dict, "arg$index$ $type$$comma$");
  }
  out->Print(") (");
  auto ret = method->output_type();
  for(int i = 0; i < ret->field_count; ++i) {
    methoddef_dict["type"] = GetGoType(ret->field(i));
	methoddef_dict["comma"] = (i == ret->field_count - 1) ? "" : ",";
	out->Print(methoddef_dict, "$type$$comma$");
  }
  out->Print(") {\n");

  // Function Body
  out->Indent();
  //TODO create protobuf object, pack it
  //call kernel interface
  //unpack return value
  out->Outdent();

  out->Print("}\n\n");
}

grpc::string GetSapphireStub(grpc::string name, grpc_generator::File* file) {
  grpc::string output;
  {
    // Scope the output stream so it closes and finalizes output to the string.
    auto out = std::unique_ptr<grpc_generator::Printer>(new ProtoBufPrinter(&output));

	// Package statement

	// Type Definition
	StringMap typedef_dict;
	typedef_dict["name"] = name;
	out->Print(typedef_dict, "type $name$ struct {\n");
	out->Indent();
	out->Print("oid uint64\n");
	out->Outdent();
	out->Print("}\n\n");

	// Each Service Definition
	//TODO: handle optionals
    for(int i = 0; i < file->service_count(); ++i) {
      auto service = file->service(i);
	  for(int j = 0; j < service->method_count(); ++j) {
        GetMethod(out, service->method(j));
      }
    }
  }
  return output;
}

}  // namespace

GoSapphireGenerator::GoSapphireGenerator() {}

GoSapphireGenerator::~GoSapphireGenerator() {}

bool PythonGrpcGenerator::Generate(const FileDescriptor* file,
                                   const grpc::string& parameter,
                                   GeneratorContext* context,
                                   grpc::string* error) const {
  // Verify valid filename
  if(file->name.size() <= strlen(".proto")) {
	  *error = "Invalid proto filename. Not long enough";
	  return false;
  }
  if(file->name.find_last_of(".proto") != file->name.size() - 1) {
	  *error = "Invalid proto filename. Proto file must end with .proto";
	  return false;
  }

  // Generate output filenames
  grpc::string pb2_file_name;
  grpc::string pb2_grpc_file_name;
    
  grpc::string base = file->name().substr(0, file->name().size() - strlen(".proto"));
  std::replace(base.begin(), base.end(), '-', '_');

  pb2_file_name = base + "_pb2.go";
  pb2_grpc_file_name = base + "_pb2_grpc.go";

  // Output stubs
  try {	
    ProtoBufFile pbfile(file);
    std::ofstream out(pb2_file_name);
    out << GetSapphireStub(base, pbfile);		  
    out.close();

  } catch (std::exception ex) {
    *error = exc.what();
	return false;
  }
  return true;
}
}  // namespace sapphire_go_generator
