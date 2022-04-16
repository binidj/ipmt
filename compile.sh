mkdir -p build
cd build
cmake -D CMAKE_BUILD_TYPE=Release ../
make
cd ..
mkdir -p bin
cp build/src/ipmt bin/