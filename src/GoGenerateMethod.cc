
void GoGenerator::GenerateMethodHeader(const MethodDescriptor* md, Printer* out) {
  if(method->client_streaming() || method->server_streaming()) {
    throw "streaming services not supported";
  }
  StringMap methoddef_dict;
  methoddef_dict["host"] = method->service()->name();
  methoddef_dict["method"] = method->name();

  // Documentation
  SourceLocation sl;
  if(method->GetSourceLocation(&sl)) {
    out->Print("/*$comment$*/\n", "comment", sl.leading_comments);
  }
  out->Print(methoddef_dict, "func (o $host$) $method$(");

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


void GoGenerator::GenerateMethodFooter(const MethodDescriptor* md, Printer* out) {
  out->Print("\n");
  
  out->Outdent();
  out->Print("}\n\n");
}



void GoGenerator::GeneratePackHeader(const Descriptor* d, Printer* out) {
  out->Print(methoddef_dict, "msg := &$argmsg${\n", "argmsg", d->name());
  out->Indent();
}


void Generator::GeneratePackFooter(const Descriptor* d, Printer* out) {
  out->Outdent();
  out->Print("}\n");
  //TODO check err
  out->Print("msgbuffer, _ := proto.Marshal(msg)\n\n");
}


void Generator::GeneratePackField(const FieldDescriptor* fd, Printer* out) {
	//protofield: value
    out->Print(methoddef_dict, "$name$: $name$,\n", "name", d->name());
}



void Generator::GenerateUnpackHeader(const Descriptor* d, Printer* out) {

}


void Generator::GenerateUnpackFooter(const Descriptor* d, Printer* out) {

}


void Generator::GenerateUnpackField(const FieldDescriptor* fd, Printer* out) {

}



void Generator::GenerateKernelCall(const MethodDescriptor* md, Printer* out) {

}




