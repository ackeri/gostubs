#include "google/protobuf/descriptor.h"
#include "google/protobuf/io/zero_copy_stream.h"
#include "google/protobuf/io/printer.h"

#include <map>
#include <string>

#include "generator.h"

using google::protobuf::FileDescriptor;
using google::protobuf::MethodDescriptor;
using google::protobuf::OneofDescriptor;
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
	//TODO remove _ and capitalize
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
    case FieldDescriptor::Type::TYPE_MESSAGE: //TODO strip _impl_
	    return name + "." + d->message_type()->name();
    default:
      return boxtypenames[d->type()];
  }
}


string GetJavaType(string name, const FieldDescriptor* d) {
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

void GenerateComments(string comment, Printer* out) {
  if(comment.length() == 0)
      return;

  out->Print("/**\n");
  string::size_type ind = 0;
  string::size_type next;
  while((next = comment.find("\n",ind)) != string::npos) {
    out->Print(" * $line$ \n", "line", comment.substr(ind, next - ind));
    ind = next + 1;
  }
  out->Print(" * $line$ \n", "line", comment.substr(ind));
  out->Print(" **/\n");
}

void GeneratePack(string package, string prefix, Printer* out, const Descriptor* d) {
	StringMap packdict;
	packdict["package"] = package;
	packdict["prefix"] = prefix;
	packdict["type"] = d->name();
	packdict["name"] = prefix + d->name();

	out->Print(packdict, "$package$.$type$.Builder msg; {\n");
	out->Indent();

	out->Print(packdict, "$package$.$type$.Builder builder = new $package$.$type$.newBuilder();\n");
	if(IsInterface(d)) {
		const OneofDescriptor* ood = d->oneof_decl(0);
		//Determine runtime type
		for(int i = 0; i < ood->field_count(); ++i) {
			packdict["argname"] = capitalizeFirst(ood->name());
			packdict["ftype"] = GetJavaType(package, ood->field(i));
			out->Print(packdict, "if($argname$.getClass().equals($ftype$.class)) {\n");
			out->Indent();
			GeneratePack(package, prefix, out, ood->field(i)->message_type());
			out->Print("builder = msg;\n");
			out->Outdent();
			out->Print("}\n");
		}
	} else {
		for(int i = 0; i < d->field_count(); ++i) {
			const FieldDescriptor* fd = d->field(i);
			packdict["argname"] = capitalizeFirst(fd->name());
			packdict["inname"] = fd->name();

			//Recurse to generate more packing code
			const Descriptor* fieldmessage = fd->message_type();
			if(fieldmessage) {
				if(fd->is_map()) {
					packdict["keytype"] = GetJavaType(package, fieldmessage->field(0));
					out->Print(packdict, "for($keytype$ key : $inname$.keyset()) {\n");
					out->Indent();
					GeneratePack(package, prefix, out, fieldmessage);
					out->Print(packdict, "builder.put$argname$(msg)\n");
					out->Outdent();
					out->Print("}\n");
				} else if(fd->is_repeated()) {
					packdict["ftype"] = GetJavaType(package, fd);
					out->Print(packdict, "for($ftype$ key : $inname$) {\n");
					out->Indent();
					GeneratePack(package, prefix, out, fieldmessage);
					out->Print(packdict, "builder.put$argname$(msg);\n");
					out->Outdent();
					out->Print("}\n");
				} else {
					out->Print("{\n");
					out->Indent();
					GeneratePack(package, prefix, out, fieldmessage);
					out->Print(packdict, "builder.set$argname$(msg);");
					out->Outdent();
					out->Print("}\n");
				}
			} else { //we can manually pack primitives
				if(fd->is_map()) {
					out->Print(packdict, "builder.putAll$argname$($inname$);\n");
				} else if(fd->is_repeated()) {
					out->Print(packdict, "builder.addAll$argname$($inname$);\n");
				} else {
					out->Print(packdict, "builder.set$argname$($inname$);\n");
				}
			}
		}
	}

	out->Print("msg = builder;\n");
	out->Outdent();
	out->Print("}");
}

void GenerateMethod(string name, Printer* out, const MethodDescriptor* method) {
  if(method->client_streaming() || method->server_streaming()) {
    throw "streaming services not supported";
  }
  StringMap methoddict;
  methoddict["name"] = name;
  methoddict["serv"] = method->service()->name().substr(strlen("_method_"));
  methoddict["method"] = method->name();

  // Documentation
  SourceLocation sl;
  if(method->GetSourceLocation(&sl)) {
    GenerateComments(sl.leading_comments, out);
  }

  // Function Header
  auto ret = method->output_type();
  if(ret->field_count() == 1) {
    methoddict["rettype"] = GetJavaType(name, ret->field(0));
  } else {
    methoddict["rettype"] = ret->name();
  }
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
	GeneratePack(name, "", out,  args);

	//call kernel interface
  //TODO call kernel interface to make rpc 
  methoddict["ret"] = ret->name();
  out->Print(methoddict, "$name$.$ret$ retmsg = $name$.$ret$.getDefaultInstance();\n\n");

  //unpack return value
  if(ret->field_count() == 1) {
    const FieldDescriptor* d = ret->field(0);
    methoddict["rettype"] = GetJavaType(name, d);
    methoddict["retname"] = capitalizeFirst(d->name());
    if(d->is_map()) {
        out->Print(methoddict, "return new Hash$rettype$(retmsg.get$retname$Map());\n");
    } else if(d->is_repeated()) {
        out->Print(methoddict, "return new Array$rettype$(retmsg.get$retname$List());\n");
    } else {
        out->Print(methoddict, "return retmsg.get$retname$();\n");
    }

  } else {
    out->Print(methoddict, "$ret$ retval = new $ret$();\n");
    for(int i = 0; i < ret->field_count(); ++i) {
        const FieldDescriptor* d = ret->field(i);
        methoddict["rettype"] = GetJavaType(name, d);
        methoddict["retname"] = capitalizeFirst(d->name());
        if(d->is_map()) {
            out->Print(methoddict, "retval.$retname$ = new Hash$rettype$(retmsg.get$retname$Map());\n");
        } else if(d->is_repeated()) {
            out->Print(methoddict, "retval.$retname$ = new Array$rettype$(retmsg.get$retname$List());\n");
        } else {
            out->Print(methoddict, "retval.$retname$ = retmsg.get$retname$();\n");
        }
    }
    out->Print(methoddict, "return retval;\n");
  }
  out->Outdent();
  out->Print("}\n\n");
}

void GenerateReturnType(string name, Printer* out, const MethodDescriptor* method) {
  const Descriptor* d = method->output_type();
  if(d->field_count() < 2)
      return;

  StringMap returndict;
  returndict["rettype"] = d->name();
  out->Print(returndict, "static class $rettype$ {\n");
  out->Indent();

  for(int i = 0; i < d->field_count(); ++i) {
    const FieldDescriptor* fd = d->field(i);
    returndict["type"] = GetJavaType(name, fd);
    returndict["name"] = capitalizeFirst(fd->name()); 
    out->Print(returndict, "public $type$ $name$;\n");
  }

  out->Outdent();
  out->Print("}\n\n");
}

void JavaSapphireGenerator::GenerateSapphireStubs(GeneratorContext* context, string name, const FileDescriptor* file) const {
  name = capitalizeFirst(name);

  for(int i = 0; i < file->service_count(); ++i) {	   
    auto service = file->service(i);
    StringMap typedict;
		string servicename = service->name().substr(strlen("_method_"));

    ZeroCopyOutputStream* zcos = context->Open(servicename + ".java");; 
    auto out = new Printer(zcos, '$');

    //TODO Package and imports
    out->Print("import java.util.*;\n");
	
    // Documentation
	  SourceLocation sl;
	  if(service->GetSourceLocation(&sl)) {
      GenerateComments(sl.leading_comments, out);
	  }

    // Class Header
    typedict["name"] = servicename;
    out->Print(typedict, "class $name$ {\n");
    out->Indent();
    out->Print("public long oid;\n\n");

    // Each method implementation
    for(int j = 0; j < service->method_count(); ++j) {
      GenerateMethod(name, out, service->method(j));
    }

    // Tuple types for returns
    for(int j = 0; j < service->method_count(); ++j) {
      GenerateReturnType(name, out, service->method(j));
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

