# Building dependencies

Need to have protobuf installed to build (note that protobuf does not have an uninstaller)

```
git clone git@github.com:google/protobuf.git
cd protobuf
git submodule update --init --recursive
cmake cmake/
cd cmake
make
sudo make install
```

## Go

To run tests, need golang and protoc-gen-go installed

```
sudo apt-get install golang
go get -u github.com/golang/protobuf/protoc-gen-go 
```

Then running build (and providing your sudo password) will generate the protoc plugin and run it on the test file. Read the protoc documentation and/or inspect build.sh for how to run on other files.

```
./gobuild.sh
```

## Java

You'll need to install jdk
Installing java protoc runtime requires using maven, which I didn't set up, so I built it from source and included the jar in the repository.

Running javabuild.sh should run the tests if your java version matches mine


```
cd java
protoc --java_out=core/src/main/java -I../src ../src/google/protobuf/descriptor.proto
cd core/src/main/java/com/google/protobuf
javac -d ./build *.java
cd build
jar cvf javaprotobuf.jar *
```


If your using a different java version, you need to build protobuf.

```
sudo apt-get install autoconf automake libtool curl make g++ unzip
git clone git@github.com:google/protobuf.git
cd protobuf
git submodule update --init --recursive
./autogen.sh
./configure
make
cd java
mvn package
```

jar is in protobuf/java/core/target/ somewhere (you want the protobuf-java.jar)
