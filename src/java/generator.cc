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


string GetJavaPrimitiveType(string package, const FieldDescriptor* d) {
  string name;
  switch(d->type()) {
    case FieldDescriptor::Type::TYPE_GROUP:
      throw "Depreceated protobuf type group unsupported";
    case FieldDescriptor::Type::TYPE_MESSAGE:
      name = d->message_type()->name();
	    return "SerialUtil." + name;
  	default:
      return typenames[d->type()];
  }
}

string GetJavaBoxType(string package, const FieldDescriptor* d) {
  string name;
  switch(d->type()) {
    case FieldDescriptor::Type::TYPE_GROUP:
      throw "Depreceated protobuf type group unsupported";
    case FieldDescriptor::Type::TYPE_MESSAGE: 
	    name = d->message_type()->name();
	    return "SerialUtil." + name;
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

//argname is name of argument
//capname can be used for protobuf message get and set calls
//type is the language type that represents the argument
//comma is , for all except for the last entry
void GenerateArgList(string package, Printer* out, const Descriptor* d, const char* format) {
  StringMap argdict;
  out->Print("(");
  if(IsInterface(d)) {
    const OneofDescriptor* ood = d->oneof_decl(0);
    argdict["argname"] = ood->name();
    argdict["capname"] = capitalizeFirst(ood->name());
    argdict["type"] = "SerialUtil." + d->name();
    argdict["comma"] = "";
    out->Print(argdict, format);
  } else {
    for(int i = 0; i < d->field_count(); ++i) {
      const FieldDescriptor* fd = d->field(i);
      argdict["argname"] = fd->name();
      argdict["capname"] = capitalizeFirst(fd->name());
      argdict["type"] = GetJavaType(package, fd);
  	  argdict["comma"] = (i == d->field_count() - 1) ? "" : ", ";
	    out->Print(argdict, format);
    }
  }
  out->Print(")");
}

void GeneratePack(string package, Printer* out, const Descriptor* d) {
	StringMap packdict;
	packdict["package"] = package;
	packdict["type"] = d->name();
	packdict["msgtype"] = package + "." + d->name();

  out->Print(packdict, "public static $msgtype$.Builder Pack$type$");
  GenerateArgList(package, out, d, "$type$ $argname$$comma$");
  out->Print(" {\n");
  out->Indent();

	out->Print(packdict, "$msgtype$.Builder msg = $msgtype$.newBuilder();\n");
  if(IsInterface(d)) {
		const OneofDescriptor* ood = d->oneof_decl(0);
    packdict["inname"] = ood->name();
		//Determine runtime type
		for(int i = 0; i < ood->field_count(); ++i) {
			packdict["argname"] = ood->name();
			packdict["capname"] = capitalizeFirst(ood->name());
			packdict["ftype"] = GetJavaType(package, ood->field(i));
      packdict["fname"] = ood->field(i)->message_type()->name();
      packdict["elseif"] = i == 0 ? "if" : " else if";
      packdict["num"] = to_string(i + 1);
			out->Print(packdict, "$elseif$($argname$.getClass().equals($ftype$.class)) {\n");
      out->Indent();
      out->Print(packdict, "$ftype$ tmp = ($ftype$)$inname$;\n");
      out->Print(packdict, "msg.setOpt$num$(Pack$fname$");
      if(IsInterface(ood->field(i)->message_type())) {
        out->Print("(tmp)");
      } else {
        GenerateArgList(package, out, ood->field(i)->message_type(), "tmp.$argname$$comma$");
      }
      out->Print(");\n");
      out->Outdent();
      out->Print("}");
		}
    out->Print(" else {\n");
    out->Indent();
    out->Print(packdict, "throw new ClassCastException(\"Could not find message type for \" + $inname$);\n");
    out->Outdent();
    out->Print("}\n");
	} else {
		for(int i = 0; i < d->field_count(); ++i) {
			const FieldDescriptor* fd = d->field(i);
			packdict["capname"] = capitalizeFirst(fd->name());
			packdict["argname"] = fd->name();

			const Descriptor* fieldmessage = fd->message_type();
			if(fieldmessage) {
				if(fd->is_map()) {
          const Descriptor* key = fieldmessage->field(0)->message_type();
          const Descriptor* value = fieldmessage->field(1)->message_type();
					packdict["keytype"] = GetJavaBoxType(package, fieldmessage->field(0));
          packdict["valuetype"] = GetJavaBoxType(package, fieldmessage->field(1));

					out->Print(packdict, "for($keytype$ key : $argname$.keySet()) {\n");
					out->Indent();
          if(value) {
            out->Print(packdict, "$valuetype$ value = $argname$.get(key);\n");
          }

          out->Print(packdict, "msg.put$capname$(");
          if(key) {
            packdict["key"] = capitalizeFirst(key->name());
            out->Print(packdict, "Pack$key$");
            GenerateArgList(package, out, key, "key.$argname$$comma$");
            out->Print(", ");
          } else {
            out->Print(packdict, "key, ");
          }

          if(value) {
            packdict["value"] = capitalizeFirst(value->name());
            out->Print(packdict, "Pack$value$");
            GenerateArgList(package, out, value, "value.$argname$$comma$");
            out->Print(".build()");
          } else {
            out->Print(packdict, "$argname$.get(key)");
          }
          out->Print(");\n");
					
          out->Outdent();
					out->Print("}\n");

				} else if(fd->is_repeated()) {
					packdict["ftype"] = GetJavaBoxType(package, fd);

					out->Print(packdict, "for($ftype$ val : $inname$) {\n");
					out->Indent();
					out->Print(packdict, "msg.put$capname$(Pack$ftype$(val));\n");
					out->Outdent();
					out->Print("}\n");
				} else {
					packdict["ftype"] = capitalizeFirst(fieldmessage->name());
					out->Print(packdict, "msg.set$capname$(Pack$ftype$");
          const char* format = (packdict["argname"].append(".$argname$$comma$")).c_str();
          GenerateArgList(package, out, fieldmessage, format);
          out->Print(packdict, ");\n");
				}
			} else { //we can manually pack primitives
				if(fd->is_repeated()) {
					out->Print(packdict, "msg.addAll$capname$($argname$);\n");
				} else {
					out->Print(packdict, "msg.set$capname$($argname$);\n");
				}
			}
		}
	}

	out->Print("return msg;\n");
	out->Outdent();
	out->Print("}\n\n");
}

void GenerateUnpack(string package, Printer* out, const Descriptor* d) {
  StringMap unpackdict;
  unpackdict["package"] = package;
  unpackdict["name"] = d->name();
  bool inlined = d->field_count() == 1;
  if(inlined) {
    unpackdict["type"] = GetJavaType(package, d->field(0));
  } else {
    unpackdict["type"] = "SerialUtil." + d->name();
  }
  
  out->Print(unpackdict, "public static $type$ Unpack$name$($package$.$name$ in) {\n");
  out->Indent();

  out->Print(unpackdict, "$type$ ret");
  if(inlined) {
    out->Print(unpackdict, ";\n");
  } else {
    out->Print(unpackdict, " = new $type$();\n");
  }
  if(IsInterface(d)) {
      //TODO implement
  } else {
    for(int i = 0; i < d->field_count(); ++i) {
      const FieldDescriptor* fd = d->field(i);
      unpackdict["rettype"] = GetJavaType(package, fd);
      if(inlined) {
        unpackdict["retname"] = "";
      } else {
        unpackdict["retname"] = "." + fd->name();
      }
      unpackdict["capname"] = capitalizeFirst(fd->name());

      if(fd->is_map()) {
        const Descriptor* d = fd->message_type();
        const FieldDescriptor* key = d->field(0);
        const FieldDescriptor* value = d->field(1);
        unpackdict["keyname"] = key->message_type() ? key->message_type()->name() : "";
        unpackdict["keytype"] = GetJavaBoxType(package, key);
        unpackdict["inkeytype"] = (key->message_type() ? package + "." + unpackdict["keyname"] : unpackdict["keytype"]); 
        unpackdict["valuename"] = value->message_type() ? value->message_type()->name() : "";
        unpackdict["valuetype"] = GetJavaBoxType(package, value);
        unpackdict["invaluetype"] = (value->message_type() ? package + "." + unpackdict["valuename"]: unpackdict["valuetype"]); 

        out->Print(unpackdict, "ret$retname$ = new HashMap<$keytype$, $valuetype$>();\n");
        out->Print(unpackdict, "Map<$inkeytype$, $invaluetype$> inmap = in.get$capname$Map();\n");
        out->Print(unpackdict, "for($inkeytype$ key : inmap.keySet()) {;\n");
        out->Indent();
        out->Print(unpackdict, "ret$retname$.put(");
        if(key->message_type()) {
          out->Print(unpackdict, "Unpack$keyname$(key)");
        } else {
          out->Print(unpackdict, "key");
        }
        out->Print(unpackdict, ", ");
        if(value->message_type()) {
          out->Print(unpackdict, "Unpack$valuename$(inmap.get(key))");
        } else {
          out->Print(unpackdict, "inmap.get(key)");
        }
        out->Print(unpackdict, ");\n");
        out->Outdent();
        out->Print(unpackdict, "}\n");
      } else if(fd->is_repeated()) {
        unpackdict["ftype"] = GetJavaBoxType(package, fd);

        out->Print(unpackdict, "ret$retname$ = new ArrayList<$ftype$>();\n");
        out->Print(unpackdict, "for($ftype$ value : in.get$capname$List()) {\n");
        out->Indent();

        out->Print(unpackdict, "ret$retname$.add(");
        if(fd->message_type()) {
            out->Print(unpackdict, "Unpack$ftype$(value)");
        } else {
            out->Print(unpackdict, "value");
        }
        out->Print(");\n");

        out->Outdent();
        out->Print(unpackdict, "}\n");
      } else {
        out->Print(unpackdict, "ret$retname$ = ");
        if(fd->message_type()) {
          unpackdict["fname"] = fd->message_type()->name();
          out->Print(unpackdict, "Unpack$fname$(in.get$capname$())");
        } else {
          out->Print(unpackdict, "in.get$capname$()");
        }
        out->Print(unpackdict, ";\n");
      }
    }
  }

  out->Print(unpackdict, "return ret;\n");
  out->Outdent();
  out->Print("}\n\n");
}

void GenerateMethod(string name, Printer* out, const MethodDescriptor* method) {
  if(method->client_streaming() || method->server_streaming()) {
    throw "streaming services not supported";
  }
  StringMap methoddict;
  methoddict["package"] = name;
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
    methoddict["rettype"] = "SerialUtil." + ret->name();
  }
  out->Print(methoddict,"$rettype$ $method$");

  auto args = method->input_type();
  GenerateArgList(name, out, args, "$type$ $argname$$comma$");
  out->Print(" {\n");
  
  // Function Body
  out->Indent();

  //create protobuf object, pack it
  methoddict["arg"] = args->name();
  out->Print(methoddict, "byte[] buf = SerialUtil.Pack$arg$");
  GenerateArgList(name, out, args, "$argname$$comma$");
  out->Print(methoddict, ".build().toByteArray();\n");

	//call kernel interface
  //TODO call kernel interface to make rpc
  out->Print(methoddict, "api.Api.GenericMethodReply retmsg = \n");
  out->Indent();
  out->Print(methoddict, "api.MgmtgrpcServiceGrpc.newBlockingStub(ManagedChannelBuilder.forAddress(\"localhost\", 2000).usePlaintext().build()).genericMethodInvoke(\n");
  out->Print(methoddict, "api.Api.GenericMethodRequest.newBuilder()\n");
  out->Print(methoddict, ".setObjId(Long.toString(oid))\n");
  out->Print(methoddict, ".setSapphireObjName(\"$serv$\")\n");
  out->Print(methoddict, ".setFuncName(\"$method$\")\n");
  out->Print(methoddict, ".setParams(com.google.protobuf.ByteString.copyFrom(buf))\n");
  out->Print(methoddict, ".build()\n");
  out->Outdent();
  out->Print(methoddict, ");\n");
  methoddict["ret"] = ret->name();

  out->Print(methoddict, "$package$.$ret$ ret;\n");
  out->Print(methoddict, "try {\n");
  out->Indent();
  out->Print(methoddict, "ret = $package$.$ret$.parseFrom(retmsg.getRet());\n");
  out->Outdent();
  out->Print(methoddict, "} catch(com.google.protobuf.InvalidProtocolBufferException ex) {\n");
  out->Indent();
  out->Print(methoddict, "throw new ClassCastException(ex.getMessage());\n");
  out->Outdent();
  out->Print(methoddict, "}\n");

  //unpack return value
  out->Print(methoddict, "return SerialUtil.Unpack$ret$(ret);\n");
  out->Outdent();
  out->Print("}\n\n");
}
//TODO void returns/args probably don't work
void GenerateType(string name, Printer* out, const Descriptor* d) {
  //TODO skip _impl_ if in native language
  
  StringMap returndict;
  returndict["type"] = d->name();
  out->Print(returndict, "public static class $type$");
  if(!d->name().compare(0, strlen("_impl_"), "_impl_")) {
    returndict["parent"] = d->name().substr(strlen("_impl_"));
    out->Print(returndict, " extends $parent$ ");
  } else if(IsInterface(d) && d->name().compare("Testobj")) {
    //TODO find parent
    returndict["parent"] = "Testobj";
    out->Print(returndict, " extends $parent$ ");
  }
  out->Print("{\n");
  out->Indent();

  for(int i = 0; i < d->field_count(); ++i) {
    const FieldDescriptor* fd = d->field(i);
    if(i == 0 && !fd->name().compare("parent"))
        continue;
    returndict["ftype"] = GetJavaType(name, fd);
    returndict["name"] = fd->name(); 
    out->Print(returndict, "public $ftype$ $name$;\n");
  }

  out->Outdent();
  out->Print("}\n\n");
}

void JavaSapphireGenerator::GenerateSapphireStubs(GeneratorContext* context, string package, const FileDescriptor* file) const {
  package = capitalizeFirst(package);

  //Create all pack/unpack helpers
  ZeroCopyOutputStream* utilzcos = context->Open("SerialUtil.java");
  Printer* util = new Printer(utilzcos, '$');

  util->Print("import java.util.*;\n\n");
  util->Print("public class SerialUtil {\n\n");
  util->Indent();
  std::set<const Descriptor*> generated;

  for(int i = 0; i < file->message_type_count(); ++i) {
    GenerateType(package, util, file->message_type(i));
    GeneratePack(package, util, file->message_type(i));
    GenerateUnpack(package, util, file->message_type(i));
  }

  util->Outdent();
  util->Print("}\n");
  delete util;
  delete utilzcos;

  //Create stub for each service
  for(int i = 0; i < file->service_count(); ++i) {	   
    auto service = file->service(i);
    StringMap typedict;
		string servicename = service->name().substr(strlen("_method_"));

    ZeroCopyOutputStream* zcos = context->Open(servicename + "_stub.java");; 
    auto out = new Printer(zcos, '$');

    //TODO Package and imports
    out->Print("import java.util.*;\n");
    out->Print("import api.*;\n");
    out->Print("import io.grpc.*;\n");
	
    // Documentation
	  SourceLocation sl;
	  if(service->GetSourceLocation(&sl)) {
      GenerateComments(sl.leading_comments, out);
	  }

    // Class Header
    typedict["name"] = servicename;
    out->Print(typedict, "class $name$_stub {\n");
    out->Indent();
    out->Print("public long oid;\n\n");

    // Each method implementation
    for(int j = 0; j < service->method_count(); ++j) {
      GenerateMethod(package, out, service->method(j));
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

