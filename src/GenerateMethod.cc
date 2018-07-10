
void Generator::GenerateMethod(const MethodDescriptor* md, Printer* out) {
  GenerateMethodHeader(md, out);

  //Packing
  Descriptor* arg = md->input_type();
  GeneratePackHeader(arg, out);
  for(int i = 0; i < arg->field_count(); i++) {
    GeneratePackField(arg->get_field(i), out);
  }
  GeneratePackFooter(arg, out);

  //Kernel call
  GenerateKernelCall(md, out);

  //Unpacking
  Descriptor* ret = md->ret_type();
  GenerateUnpackHeader(ret, out);
  for(int i = 0; i < ret->field_count(); i++) {
    GenereateUnpackField(ret->get_field(i), out);
  }
  GenerateUnpackFooter(ret, out);

  GenerateMethodFooter(md, out);
}

