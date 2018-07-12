#include "google/protobuf/descriptor.h"
#include "google/protobuf/io/zero_copy_stream.h"
#include "google/protobuf/io/printer.h"

#include <map>
#include <string>

#include "generator.h"

using google::protobuf::FileDescriptor;
using google::protobuf::MethodDescriptor;
using google::protobuf::Descriptor;
using google::protobuf::FieldDescriptor;
using google::protobuf::SourceLocation;
using google::protobuf::io::ZeroCopyOutputStream;
using google::protobuf::io::Printer;
using std::map;
using std::string;
typedef map<string, string> StringMap;


//TODO check input is not reserved keyword in golang
//TODO Camelcase all the names (because the go protobuf compiler does for public access)

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
    return "[]" + GetGoPrimitiveType(d);
  } else {
    return GetGoPrimitiveType(d);
  }
}

void GenerateMethod(Printer* out, const MethodDescriptor* method) {
  if(method->client_streaming() || method->server_streaming()) {
    throw "streaming services not supported";
  }
  StringMap methoddict;
  methoddict["name"] = method->service()->name();
  methoddict["method"] = method->name();

  // Documentation
  SourceLocation sl;
  if(method->GetSourceLocation(&sl)) {
    methoddict["comment"] = sl.leading_comments;
    out->Print(methoddict, "/*$comment$*/\n");
  }
  out->Print(methoddict, "func (o $name$) $method$(");

  // Function Header
  auto args = method->input_type();
  for(int i = 0; i < args->field_count(); ++i) {
    const FieldDescriptor* d = args->field(i);
    methoddict["argname"] = d->name();
    methoddict["type"] = GetGoType(d);
	methoddict["comma"] = (i == args->field_count() - 1) ? "" : ", ";
	out->Print(methoddict, "$argname$ $type$$comma$");
  }
  out->Print(") (");
  auto ret = method->output_type();
  for(int i = 0; i < ret->field_count(); ++i) {
    const FieldDescriptor* d = ret->field(i);
    methoddict["type"] = GetGoType(d);
	methoddict["comma"] = (i == ret->field_count() - 1) ? "" : ", ";
	out->Print(methoddict, "$type$$comma$");
  }
  out->Print(") {\n");

  // Function Body
  out->Indent();

  //create protobuf object, pack it
  methoddict["argmsg"] = args->name();
  out->Print(methoddict, "msg := &$argmsg${\n");
  out->Indent();
  for(int i = 0; i < args->field_count(); ++i) {
    const FieldDescriptor* d = args->field(i);
    methoddict["argname"] = d->name();
    out->Print(methoddict, "$argname$: $argname$,\n");
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
  methoddict["ret"] = ret->name();
  out->Print(methoddict, "retmsg := &$ret${}\n");
  out->Print("_ = proto.Unmarshal(retmsgbuffer, retmsg)\n");
  out->Print("return ");
  for(int i = 0; i < ret->field_count(); ++i) {
    const FieldDescriptor* d = ret->field(i);
	methoddict["retname"] = d->name();
	methoddict["comma"] = (i == ret->field_count() - 1) ? "" : ", ";
	out->Print(methoddict, "retmsg.$retname$$comma$");
  }
  out->Print("\n");
  
  out->Outdent();
  out->Print("}\n\n");
}

void GoSapphireGenerator::GenerateSapphireStubs(GeneratorContext* context, string name, const FileDescriptor* file) const {
  
  ZeroCopyOutputStream* zcos = context->Open("Sapphire" + name + ".pb.go");; 
  auto out = new Printer(zcos, '$');

  // Package statement
  // TODO include protobuf package structure, and nested message imports
  out->Print("package $pkg$\n\n", "pkg", name);
  out->Print("import \"github.com/golang/protobuf/proto\"\n\n");

  for(int i = 0; i < file->service_count(); ++i) {
    auto service = file->service(i);
    StringMap typedict;

	// Documentation
	SourceLocation sl;
	if(service->GetSourceLocation(&sl)) {
      typedict["comment"] = sl.leading_comments;
      out->Print(typedict, "/*$comment$*/\n");
	}

    // Type Definition
    typedict["name"] = service->name();
    out->Print(typedict, "type $name$ struct {\n");
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
    throw "IO error occured during proto generation";
  }

  //Printer needs to close before zcos
  delete out;
  delete zcos;
}

