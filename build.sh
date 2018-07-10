
#build
g++ src/go_generator.cc src/go_plugin.cc -std=c++11 `pkg-config --cflags --libs protobuf` -lprotoc -o bin/protoc-gen-go_sapphire && \

#install
chmod +x bin/protoc-gen-go_sapphire && \
sudo cp bin/protoc-gen-go_sapphire /usr/local/bin/ && \


#run code generator
protoc --go_out=output test.proto && \
protoc --go_sapphire_out=output test.proto && \

#place in gopath so we can run
mkdir -p $GOPATH/src/CodeGenTest/test && \
mkdir -p $GOPATH/src/CodeGenTest/main && \
cp -r output/* $GOPATH/src/CodeGenTest/test && \
cp test.go $GOPATH/src/CodeGenTest/main/test.go && \

#build and run go application
go install CodeGenTest/main && \
$GOPATH/bin/main

echo "done"


