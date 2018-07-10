
void Generator::GenerateMethod(const MethodDescriptor* md, Printer* out) {

}


void Generator::GenerateMethodHeader(const MethodDescriptor* md, Printer* out) {
  if(method->client_streaming() || method->server_streaming()) {
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

  out->Indent();
}


void Generator::GenerateMethodFooter(const MethodDescriptor* md, Printer* out) {
  out->Print("\n");
  
  out->Outdent();
  out->Print("}\n\n");
}



void Generator::GeneratePackHeader(const Descriptor* d, Printer* out) {

}


void Generator::GeneratePackFooter(const Descriptor* d, Printer* out) {

}


void Generator::GeneratePackField(const FieldDescriptor* fd, Printer* out) {

}



void Generator::GenerateUnpackHeader(const Descriptor* d, Printer* out) {

}


void Generator::GenerateUnpackFooter(const Descriptor* d, Printer* out) {

}


void Generator::GenerateUnpackField(const FieldDescriptor* fd, Printer* out) {

}



void Generator::GenerateKernelCall(const MethodDescriptor* md, Printer* out) {

}




