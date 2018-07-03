g++ go_generator.cc go_plugin.cc -std=c++11 `pkg-config --cflags --libs protobuf` -lprotoc
