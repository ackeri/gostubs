
#build
echo "building code generator" && \
mkdir -p bin && \
g++ -Isrc src/generator_base.cc src/java/generator.cc src/java/plugin.cc -std=c++11 `pkg-config --cflags --libs protobuf` -lprotoc -o bin/protoc-gen-java_sapphire && \

#install
echo "installing" && \
chmod +x bin/protoc-gen-java_sapphire && \
sudo cp bin/protoc-gen-java_sapphire /usr/local/bin/ && \


#run code generator
echo "generating code" && \
mkdir -p javaoutput && \
protoc --java_out=javaoutput test.proto && \
protoc --java_sapphire_out=javaoutput test.proto && \

#build and run java test
echo "building generated code" && \
#cd javaoutput && \
javac -cp "/usr/share/java/*:javalib/*:javaoutput/" javaoutput/test.java javaoutput/api/Api.java javaoutput/api/MgmtgrpcServiceGrpc.java && \
echo "running generated code" && \
java -cp "/usr/share/java/*:javalib/*:javaoutput/" test && \

echo "done"


