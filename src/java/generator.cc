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

string capitalizeFirst(string s) {
  s[0] = toupper(s[0]);
  return s;
}


//TODO check input is not reserved keyword in java
//TODO check name compatability with java->proto conversion tool
//TODO java doesn't support unsigned int types

// Map from protobuf type (from fielddescriptor)
// to in language type for primitives
map<FieldDescriptor::Type,string> typenames = {
  {FieldDescriptor::Type::TYPE_DOUBLE, "double"},
  {FieldDescriptor::Type::TYPE_FLOAT, "float"},
  {FieldDescriptor::Type::TYPE_INT64, "long"},
  {FieldDescriptor::Type::TYPE_UINT64, "long"},
  {FieldDescriptor::Type::TYPE_INT32, "int"},
  {FieldDescriptor::Type::TYPE_FIXED64, "long"},
  {FieldDescriptor::Type::TYPE_FIXED32, "int"},
  {FieldDescriptor::Type::TYPE_BOOL, "boolean"},
  {FieldDescriptor::Type::TYPE_STRING, "String"},
  {FieldDescriptor::Type::TYPE_GROUP, "(DEPRECEATED PROTOBUF TYPE GROUP)"},
  {FieldDescriptor::Type::TYPE_MESSAGE, "HANDLED_ELSEWHERE_MESSAGE"}, 
  {FieldDescriptor::Type::TYPE_BYTES, "char[]"},  //TODO: verify this is correct
  {FieldDescriptor::Type::TYPE_UINT32, "int"},
  {FieldDescriptor::Type::TYPE_ENUM, "TODO_ENUM"}, //TODO
  {FieldDescriptor::Type::TYPE_SFIXED32, "int"},
  {FieldDescriptor::Type::TYPE_SFIXED64, "long"},
  {FieldDescriptor::Type::TYPE_SINT32, "int"},
  {FieldDescriptor::Type::TYPE_SINT64, "long"}
};

//boxed types for maps and lists
map<FieldDescriptor::Type,string> boxtypenames = {
  {FieldDescriptor::Type::TYPE_DOUBLE, "Double"},
  {FieldDescriptor::Type::TYPE_FLOAT, "Float"},
  {FieldDescriptor::Type::TYPE_INT64, "Long"},
  {FieldDescriptor::Type::TYPE_UINT64, "Long"},
  {FieldDescriptor::Type::TYPE_INT32, "Integer"},
  {FieldDescriptor::Type::TYPE_FIXED64, "Long"},
  {FieldDescriptor::Type::TYPE_FIXED32, "Integer"},
  {FieldDescriptor::Type::TYPE_BOOL, "Boolean"},
  {FieldDescriptor::Type::TYPE_STRING, "String"},
  {FieldDescriptor::Type::TYPE_GROUP, "(DEPRECEATED PROTOBUF TYPE GROUP)"},
  {FieldDescriptor::Type::TYPE_MESSAGE, "HANDLED_ELSEWHERE_MESSAGE"},
  {FieldDescriptor::Type::TYPE_BYTES, "char[]"},  //TODO: verify this is correct
  {FieldDescriptor::Type::TYPE_UINT32, "Integer"},
  {FieldDescriptor::Type::TYPE_ENUM, "TODO_ENUM"}, //TODO
  {FieldDescriptor::Type::TYPE_SFIXED32, "Integer"},
  {FieldDescriptor::Type::TYPE_SFIXED64, "Long"},
  {FieldDescriptor::Type::TYPE_SINT32, "Integer"},
  {FieldDescriptor::Type::TYPE_SINT64, "Long"}
};


string GetJavaPrimitiveType(string name, const FieldDescriptor* d) {
  switch(d->type()) {
    case FieldDescriptor::Type::TYPE_GROUP:
      throw "Depreceated protobuf type group unsupported";
    case FieldDescriptor::Type::TYPE_MESSAGE:
	    return name + "." + d->message_type()->name();
  	default:
      return typenames[d->type()];
  }
}

string GetJavaBoxType(string name, const FieldDescriptor* d) {
  switch(d->type()) {
    case FieldDescriptor::Type::TYPE_GROUP:
      throw "Depreceated protobuf type group unsupported";
    case FieldDescriptor::Type::TYPE_MESSAGE:
	  return name + "." + d->message_type()->name();
	default:
      return boxtypenames[d->type()];
  }
}


string GetJavaType(string name, const FieldDescriptor* d) {
  //TODO how is protobuf oneof implemented?
  if(d->is_map()) {
    const Descriptor* entry = d->message_type();
    string out = "Map<";
    out += GetJavaBoxType(name, entry->field(0)) + ", " + GetJavaBoxType(name, entry->field(1)) + ">";
    return out;
  } else if(d->is_repeated()) {
    return "List<" + GetJavaBoxType(name, d) + ">";
  } else {
    return GetJavaPrimitiveType(name, d);
  }
}

void GenerateMethod(string name, Printer* out, const MethodDescriptor* method) {
  if(method->client_streaming() || method->server_streaming()) {
    throw "streaming services not supported";
  }
  StringMap methoddict;
  methoddict["name"] = name;
  methoddict["serv"] = method->service()->name();
  methoddict["method"] = method->name();

  // Documentation
  SourceLocation sl;
  if(method->GetSourceLocation(&sl)) {
    methoddict["comment"] = sl.leading_comments;
    out->Print(methoddict, "/*$comment$*/\n");
  }

  // Function Header
  auto ret = method->output_type();
  //TODO: build a new class to support multiple returns?
  if(ret->field_count() > 1)
	  throw "multiple returns not supported in Java";
  methoddict["rettype"] = GetJavaType(name, ret->field(0));
  out->Print(methoddict,"$rettype$ $method$(");

  auto args = method->input_type();
  for(int i = 0; i < args->field_count(); ++i) {
    const FieldDescriptor* d = args->field(i);
    methoddict["argname"] = d->name();
    methoddict["type"] = GetJavaType(name, d);
	  methoddict["comma"] = (i == args->field_count() - 1) ? "" : ", ";
	  out->Print(methoddict, "$type$ $argname$$comma$");
  }
  out->Print(") {\n");

  // Function Body
  out->Indent();

  //create protobuf object, pack it
  methoddict["argmsg"] = args->name();
  out->Print(methoddict, "$name$.$argmsg$.Builder msg = $name$.$argmsg$.newBuilder();\n");
  for(int i = 0; i < args->field_count(); ++i) {
    const FieldDescriptor* d = args->field(i);
    methoddict["argname"] = capitalizeFirst(d->name());
    methoddict["inname"] = d->name();
  	if(d->is_map()) {
      out->Print(methoddict, "msg.putAll$argname$($inname$);\n");
    } else if(d->is_repeated()) {
      out->Print(methoddict, "msg.addAll$argname$($inname$);\n");
    } else {
      out->Print(methoddict, "msg.set$argname$($inname$);\n");
    }
  }
  out->Print(methoddict, "$name$.$argmsg$ msgbuffer = msg.build();\n\n");

  //call kernel interface
  //TODO call kernel interface to make rpc 
  methoddict["ret"] = ret->name();
  out->Print(methoddict, "$name$.$ret$ retmsg = $name$.$ret$.getDefaultInstance();\n\n");

  //unpack return value
  const FieldDescriptor* d = ret->field(0);
  methoddict["retname"] = capitalizeFirst(d->name());
  if(d->is_map()) {
    out->Print(methoddict, "return retmsg.get$retname$Map();\n");
    //TODO this returns an immutable map
  } else if(d->is_repeated()) {
    out->Print(methoddict, "return retmsg.get$retname$List();\n");
    //TODO this returns an immutable list
  } else {
    out->Print(methoddict, "return retmsg.get$retname$();\n");
  }
  
  out->Outdent();
  out->Print("}\n\n");
}

void JavaSapphireGenerator::GenerateSapphireStubs(GeneratorContext* context, string name, const FileDescriptor* file) const {

  for(int i = 0; i < file->service_count(); ++i) {	   
    auto service = file->service(i);
    StringMap typedict;

    ZeroCopyOutputStream* zcos = context->Open(service->name() + ".java");; 
    auto out = new Printer(zcos, '$');

    //TODO Package and imports
    out->Print("import java.util.*;\n");
	
    // Documentation
	  SourceLocation sl;
	  if(service->GetSourceLocation(&sl)) {
      typedict["comment"] = sl.leading_comments;
      out->Print(typedict, "/*$comment$*/\n");
	  }

    // Class Header
    typedict["name"] = service->name();
    out->Print(typedict, "class $name$ {\n");
    out->Indent();
    out->Print("public long oid;\n\n");

    // Each method implementation
    for(int j = 0; j < service->method_count(); ++j) {
      GenerateMethod("Javatest", out, service->method(j));
    }

    out->Outdent();
    out->Print("}\n");

    if(out->failed()) {
        throw "IO error occured during proto generation";
    }

    //Printer needs to close before zcos
    delete out;
    delete zcos;

  }
}

