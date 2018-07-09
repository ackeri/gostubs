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

namespace {

typedef map<string, string> StringMap;
typedef vector<string> StringVector;
typedef tuple<string, string> StringPair;
typedef set<StringPair> StringPairSet;

// Map from protobuf type (from fielddescriptor)
// to in language type for primitives
map<FieldDescriptor::Type,string> typenames = {
  {FieldDescriptor::Type::TYPE_DOUBLE, "float64"},
  {FieldDescriptor::Type::TYPE_FLOAT, "float32"},
  {FieldDescriptor::Type::TYPE_INT64, "int64"},
  {FieldDescriptor::Type::TYPE_UINT64, "uint64"},
  {FieldDescriptor::Type::TYPE_INT32, "int32"},
  {FieldDescriptor::Type::TYPE_FIXED64, "uint64"},
  {FieldDescriptor::Type::TYPE_FIXED32, "uint32"},
  {FieldDescriptor::Type::TYPE_BOOL, "bool"},
  {FieldDescriptor::Type::TYPE_STRING, "string"},
  {FieldDescriptor::Type::TYPE_GROUP, "(DEPRECEATED PROTOBUF TYPE GROUP)"},
  {FieldDescriptor::Type::TYPE_MESSAGE, ""}, //TODO
  {FieldDescriptor::Type::TYPE_BYTES, ""}, //TODO
  {FieldDescriptor::Type::TYPE_UINT32, "uint32"},
  {FieldDescriptor::Type::TYPE_ENUM, ""}, //TODO
  {FieldDescriptor::Type::TYPE_SFIXED32, "int32"},
  {FieldDescriptor::Type::TYPE_SFIXED64, "int64"},
  {FieldDescriptor::Type::TYPE_SINT32, "int32"},
  {FieldDescriptor::Type::TYPE_SINT64, "int64"}
};

string GetGoType(const FieldDescriptor* d) {
  if(d->is_repeated()) {
    return "[]" + typenames[d->type()]; 
  } else if(d->is_map()) {
    return "";//"map[" + "key" + "]" + "value";
  } else {
    return typenames[d->type()];
  }
}

void GetMethod(Printer* out, const MethodDescriptor* method) {
  StringMap methoddef_dict;
  methoddef_dict["name"] = method->service()->name();
  methoddef_dict["method"] = method->name();

  // Function Header
  // TODO something like PrintAllComments(method->GetAllComments(), out);
  out->Print(methoddef_dict, "func (o $name$) $method$(");

  auto args = method->input_type();
  for(int i = 0; i < args->field_count(); ++i) {
    const FieldDescriptor* d = args->field(i);
	//TODO: d->is_optional() d->is_repeated() d->is_map()
    methoddef_dict["argname"] = d->name();
    methoddef_dict["type"] = GetGoType(d);
	methoddef_dict["comma"] = (i == args->field_count() - 1) ? "" : ", ";
	out->Print(methoddef_dict, "$argname$ $type$$comma$");
  }
  out->Print(") (");
  auto ret = method->output_type();
  for(int i = 0; i < ret->field_count(); ++i) {
    const FieldDescriptor* d = ret->field(i);
    methoddef_dict["type"] = GetGoType(d);
	methoddef_dict["comma"] = (i == ret->field_count() - 1) ? "" : ", ";
	out->Print(methoddef_dict, "$type$$comma$");
  }
  out->Print(") {\n");

  // Function Body
  out->Indent();

  //create protobuf object, pack it
  methoddef_dict["argmsg"] = args->name();
  out->Print(methoddef_dict, "msg := $argmsg${\n");
  out->Indent();
  for(int i = 0; i < args->field_count(); ++i) {
    const FieldDescriptor* d = args->field(i);
    methoddef_dict["argname"] = d->name();
    out->Print(methoddef_dict, "$argname$: $argname$,\n");
  }
  out->Outdent();
  out->Print("}\n");
  //TODO check err
  out->Print("msgbuffer, err := proto.Marshal(msg)\n\n");

  //call kernel interface
  //TODO call kernel interface to make rpc
  out->Print("retmsgbuffer := makeRPC(msgbuffer)\n");

  //unpack return value
  methoddef_dict["ret"] = ret->name();
  out->Print(methoddef_dict, "retmsg := &pb.$ret${}\n");
  out->Print("err = proto.Unmarshal(retmsgbuffer, retmsg\n");
  out->Print("return (");
  for(int i = 0; i < ret->field_count(); ++i) {
    const FieldDescriptor* d = ret->field(i);
	methoddef_dict["retname"] = d->name();
	methoddef_dict["comma"] = (i == ret->field_count() - 1) ? "" : ", ";
	out->Print(methoddef_dict, "retmsg.$retname$$comma$");
  }
  out->Print(")\n");
  
  out->Outdent();
  out->Print("}\n\n");
}

void GetSapphireStub(GeneratorContext* context, string name, const FileDescriptor* file) {
  
  ZeroCopyOutputStream* zcos = context->Open(name + ".pb.go");; 
  auto out = new Printer(zcos, '$');

  // Package statement
  //TODO fill in

  for(int i = 0; i < file->service_count(); ++i) {
    auto service = file->service(i);

    // Type Definition
    StringMap typedef_dict;
    typedef_dict["name"] = service->name();
    out->Print(typedef_dict, "type $name$ struct {\n");
    out->Indent();
    out->Print("oid uint64\n");
    out->Outdent();
    out->Print("}\n\n");

	// Each method implementation
    for(int j = 0; j < service->method_count(); ++j) {
      GetMethod(out, service->method(j));
    }
  }

  if(out->failed()) {
    //TODO throw appropriate exception
  }

  //Printer needs to close before zcos
  delete out;
  delete zcos;
}

}  // namespace

bool GoSapphireGenerator::Generate(const FileDescriptor* file,
                                   const string& parameter,
                                   google::protobuf::compiler::GeneratorContext* context,
                                   string* error) const {
  // Verify valid filename
  if(file->name().size() <= strlen(".proto")) {
	  *error = "Invalid proto filename. Not long enough";
	  return true;
  }
  if(file->name().find_last_of(".proto") != file->name().size() - 1) {
	  *error = "Invalid proto filename. Proto file must end with .proto";
	  return true;
  }

  // Generate base filename for output
  string base = file->name().substr(0, file->name().size() - strlen(".proto"));
 
  // Output stubs
  try {
    GetSapphireStub(context, base, file);
  } catch (std::exception ex) {
    *error = ex.what();
	return true;
  }
  return true;
}
}  // namespace sapphire_go_generator



