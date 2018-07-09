
#build
g++ go_generator.cc go_plugin.cc -std=c++11 `pkg-config --cflags --libs protobuf` -lprotoc -o protoc-gen-go_sapphire

#install
chmod +x protoc-gen-go_sapphire
sudo cp protoc-gen-go_sapphire /usr/local/bin/


#run
protoc --go_sapphire_out=output test.proto


