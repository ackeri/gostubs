
#build
mkdir -p bin
g++ -Isrc src/generator_base.cc src/go/generator.cc src/go/plugin.cc -std=c++11 `pkg-config --cflags --libs protobuf` -lprotoc -o bin/protoc-gen-go_sapphire && \

#install
chmod +x bin/protoc-gen-go_sapphire && \
sudo cp bin/protoc-gen-go_sapphire /usr/local/bin/ && \


#run code generator
mkdir -p gooutput
protoc --go_out=gooutput test.proto && \
protoc --go_sapphire_out=gooutput test.proto && \

#place in gopath so we can run
mkdir -p $GOPATH/src/CodeGenTest/test && \
mkdir -p $GOPATH/src/CodeGenTest/main && \
cp gooutput/test.pb.go $GOPATH/src/CodeGenTest/test && \
cp gooutput/Sapphiretest.pb.go $GOPATH/src/CodeGenTest/test && \
cp gooutput/test.go $GOPATH/src/CodeGenTest/main && \

#build and run go application
go install CodeGenTest/main && \
$GOPATH/bin/main

echo "done"


