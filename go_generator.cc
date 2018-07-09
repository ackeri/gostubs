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
using google::protobuf::Descriptor;
using google::protobuf::FieldDescriptor;
using google::protobuf::SourceLocation;
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

//TODO check input is not reserved keyword in golang
//TODO Camelcase all the names (because the go protobuf compiler does)

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
  {FieldDescriptor::Type::TYPE_MESSAGE, "TODO_MESSAGE"}, //TODO
  {FieldDescriptor::Type::TYPE_BYTES, "[]byte"},  //TODO: verify this is correct
  {FieldDescriptor::Type::TYPE_UINT32, "uint32"},
  {FieldDescriptor::Type::TYPE_ENUM, "TODO_ENUM"}, //TODO
  {FieldDescriptor::Type::TYPE_SFIXED32, "int32"},
  {FieldDescriptor::Type::TYPE_SFIXED64, "int64"},
  {FieldDescriptor::Type::TYPE_SINT32, "int32"},
  {FieldDescriptor::Type::TYPE_SINT64, "int64"}
};

string GetGoPrimitiveType(const FieldDescriptor* d) {
  switch(d->type()) {
    case FieldDescriptor::Type::TYPE_GROUP:
      throw "Depreceated protobuf type group unsupported";
    case FieldDescriptor::Type::TYPE_MESSAGE:
	  return "*" + d->message_type()->name();
	default:
      return typenames[d->type()];
  }
}

string GetGoType(const FieldDescriptor* d) {
  //TODO how is protobuf oneof implemented?
  if(d->is_map()) {
    const Descriptor* entry = d->message_type();
    string out = "map[";
    out += GetGoType(entry->field(0)) + "]" + GetGoType(entry->field(1));
    return out;
  } else if(d->is_repeated()) {
    //if(d->message_type() && d->message_type()->options().has_map_entry()) {
    //  return "WE GOT EM";
    //} else {
      return "[]" + GetGoPrimitiveType(d);
    //}
  } else {
    return GetGoPrimitiveType(d);
  }
}

void GenerateMethod(Printer* out, const MethodDescriptor* method) {
  if(method->client_streaming() || method->server_streaming) {
    throw "streaming services not supported";
  }
  StringMap methoddef_dict;
  methoddef_dict["name"] = method->service()->name();
  methoddef_dict["method"] = method->name();

  // Documentation
  SourceLocation sl;
  if(method->GetSourceLocation(&sl)) {
    methoddef_dict["comment"] = sl.leading_comments;
    out->Print(methoddef_dict, "/*$comment$*/\n");
  }
  out->Print(methoddef_dict, "func (o $name$) $method$(");

  // Function Header
  auto args = method->input_type();
  for(int i = 0; i < args->field_count(); ++i) {
    const FieldDescriptor* d = args->field(i);
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
  out->Print(methoddef_dict, "msg := &$argmsg${\n");
  out->Indent();
  for(int i = 0; i < args->field_count(); ++i) {
    const FieldDescriptor* d = args->field(i);
    methoddef_dict["argname"] = d->name();
    out->Print(methoddef_dict, "$argname$: $argname$,\n");
  }
  out->Outdent();
  out->Print("}\n");
  //TODO check err
  out->Print("msgbuffer, _ := proto.Marshal(msg)\n\n");

  //call kernel interface
  //TODO call kernel interface to make rpc
  out->Print("_ = msgbuffer //suppresses warning\n");
  out->Print("retmsgbuffer := make([]byte, 0)\n\n");//makeRPC(msgbuffer)\n\n");

  //unpack return value
  methoddef_dict["ret"] = ret->name();
  out->Print(methoddef_dict, "retmsg := &$ret${}\n");
  out->Print("_ = proto.Unmarshal(retmsgbuffer, retmsg)\n");
  out->Print("return ");
  for(int i = 0; i < ret->field_count(); ++i) {
    const FieldDescriptor* d = ret->field(i);
	methoddef_dict["retname"] = d->name();
	methoddef_dict["comma"] = (i == ret->field_count() - 1) ? "" : ", ";
	out->Print(methoddef_dict, "retmsg.$retname$$comma$");
  }
  out->Print("\n");
  
  out->Outdent();
  out->Print("}\n\n");
}

void GenerateSapphireStub(GeneratorContext* context, string name, const FileDescriptor* file) {
  
  ZeroCopyOutputStream* zcos = context->Open("Sapphire" + name + ".pb.go");; 
  auto out = new Printer(zcos, '$');

  // Package statement
  // TODO include protobuf package structure, and nested message imports
  out->Print("package $pkg$\n\n", "pkg", name);
  out->Print("import \"github.com/golang/protobuf/proto\"\n\n");

  for(int i = 0; i < file->service_count(); ++i) {
    auto service = file->service(i);
    StringMap typedef_dict;

	// Documentation
	SourceLocation sl;
	if(service->GetSourceLocation(&sl)) {
      typedef_dict["comment"] = sl.leading_comments;
      out->Print(typedef_dict, "/*$comment$*/\n");
	}

    // Type Definition
    typedef_dict["name"] = service->name();
    out->Print(typedef_dict, "type $name$ struct {\n");
    out->Indent();
    out->Print("Oid uint64\n");
    out->Outdent();
    out->Print("}\n\n");

	// Each method implementation
    for(int j = 0; j < service->method_count(); ++j) {
      GenerateMethod(out, service->method(j));
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
    GenerateSapphireStub(context, base, file);
  } catch (std::exception ex) {
    *error = ex.what();
	return false;
  } catch (string ex) {
    *error = ex;
	return false;
  }
  return true;
}
}  // namespace sapphire_go_generator



