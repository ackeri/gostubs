Need to have protobuf installed to build (note that protobuf does not have an uninstaller)

git clone git@github.com:google/protobuf.git
cd protobuf
git submodule update --init --recursive
cmake cmake/
cd cmake
make
sudo make install

To run tests, need golang and protoc-gen-go installed

sudo apt-get install golang
go get -u github.com/golang/protobuf/protoc-gen-go 

Then running build (and providing your sudo password) will generate the protoc plugin and run it on the test file. Read the protoc documentation and/or inspect build.sh for how to run on other files.

./build.sh

