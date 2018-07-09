type testobj struct {
  oid uint64
}

func (o testobj) func1(name string, id int32, floatingvalue float64) (string, []string, []) {
  msg := msg1{
    name: name,
    id: id,
    floatingvalue: floatingvalue,
  }
  msgbuffer, err := proto.Marshal(msg)

  retmsgbuffer := makeRPC(msgbuffer)
  retmsg := &pb.msg2{}
  err = proto.Unmarshal(retmsgbuffer, retmsg
  return (retmsg.optstring, retmsg.liststring, retmsg.idtostringmap)
}

func (o testobj) func2(optstring string, liststring []string, idtostringmap []) () {
  msg := msg2{
    optstring: optstring,
    liststring: liststring,
    idtostringmap: idtostringmap,
  }
  msgbuffer, err := proto.Marshal(msg)

  retmsgbuffer := makeRPC(msgbuffer)
  retmsg := &pb.msg3{}
  err = proto.Unmarshal(retmsgbuffer, retmsg
  return (retmsg.nested)
}

func (o testobj) func3(optstring string, liststring []string, idtostringmap []) (string, int32, float64) {
  msg := msg2{
    optstring: optstring,
    liststring: liststring,
    idtostringmap: idtostringmap,
  }
  msgbuffer, err := proto.Marshal(msg)

  retmsgbuffer := makeRPC(msgbuffer)
  retmsg := &pb.msg1{}
  err = proto.Unmarshal(retmsgbuffer, retmsg
  return (retmsg.name, retmsg.id, retmsg.floatingvalue)
}

