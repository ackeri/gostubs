
#build
mkdir -p bin
g++ -Isrc src/generator_base.cc src/java/generator.cc src/java/plugin.cc -std=c++11 `pkg-config --cflags --libs protobuf` -lprotoc -o bin/protoc-gen-java_sapphire && \

#install
chmod +x bin/protoc-gen-java_sapphire && \
sudo cp bin/protoc-gen-java_sapphire /usr/local/bin/ && \


#run code generator
mkdir -p javaoutput
protoc --java_out=javaoutput gotest.proto && \
protoc --java_sapphire_out=javaoutput gotest.proto && \

echo "done"

