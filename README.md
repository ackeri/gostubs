Need to have protobuf installed to build

git clone git@github.com:google/protobuf.git
cd protobuf
git submodule update --init --recursive
cmake cmake/
cd cmake
make
sudo make install

Then running build (and providing your sudo password) will generate the protoc plugin and run it on the test file. Read the protoc documentation and/or inspect build.sh for how to run on other files.

./build.sh

