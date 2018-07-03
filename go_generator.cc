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

#include "google/protobuf/descriptor.h"
#include "google/protobuf/io/zero_copy_stream.h"
#include "google/protobuf/io/zero_copy_stream_impl.h"
#include "google/protobuf/io/printer.h"

#include "go_generator.h"

using google::protobuf::FileDescriptor;
using google::protobuf::MethodDescriptor;
using google::protobuf::FieldDescriptor;
using google::protobuf::io::ZeroCopyOutputStream;
using google::protobuf::io::OstreamOutputStream;
using google::protobuf::io::Printer;
using std::make_pair;
using std::map;
using std::pair;
using std::replace;
using std::set;
using std::tuple;
using std::vector;
using std::string;

namespace sapphire_go_generator {

string generator_file_name;

namespace {

typedef map<string, string> StringMap;
typedef vector<string> StringVector;
typedef tuple<string, string> StringPair;
typedef set<StringPair> StringPairSet;

string GetGoType(const FieldDescriptor* d) {
	return "uint32";
}

void GetMethod(Printer* out, const MethodDescriptor* method) {
  StringMap methoddef_dict;
  methoddef_dict["name"] = method->service()->name();
  methoddef_dict["method"] = method->name();

  // Function Header
  // TODO something like PrintAllComments(method->GetAllComments(), out);
  out->Print("func (o $name$) $method$(");

  auto args = method->input_type();
  for(int i = 0; i < args->field_count(); ++i) {
    const FieldDescriptor* d = args->field(i);
    methoddef_dict["argname"] = d->name();
    methoddef_dict["type"] = GetGoType(d);
	methoddef_dict["comma"] = (i == args->field_count() - 1) ? "" : ",";
	out->Print(methoddef_dict, "$name$ $type$$comma$");
  }
  out->Print(") (");
  auto ret = method->output_type();
  for(int i = 0; i < ret->field_count(); ++i) {
    const FieldDescriptor* d = ret->field(i);
    methoddef_dict["type"] = GetGoType(d);
	methoddef_dict["comma"] = (i == ret->field_count() - 1) ? "" : ",";
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

void GetSapphireStub(string name, const FileDescriptor* file) {
  // Scope the output stream so it closes and finalizes output to the string.
  std::ofstream os(name + ".pb.go");
  OstreamOutputStream oos(&os);
  auto out = new Printer(&oos, '$');

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

}  // namespace

bool GoSapphireGenerator::Generate(const FileDescriptor* file,
                                   const string& parameter,
                                   google::protobuf::compiler::GeneratorContext* context,
                                   string* error) const {
  // Verify valid filename
  if(file->name().size() <= strlen(".proto")) {
	  *error = "Invalid proto filename. Not long enough";
	  return false;
  }
  if(file->name().find_last_of(".proto") != file->name().size() - 1) {
	  *error = "Invalid proto filename. Proto file must end with .proto";
	  return false;
  }

  string base = "cat"; //TODO fix

  // Output stubs
  try {	
    GetSapphireStub(base, file);
  } catch (std::exception ex) {
    *error = ex.what();
	return false;
  }
  return true;
}
}  // namespace sapphire_go_generator



