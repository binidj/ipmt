# CMakeList.txt : CMake project for pmt, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

# Add source to this project's executable.
add_executable (ipmt 
    "ipmt.cpp" "ipmt.h" 
    "util/Text.h" "util/Text.cpp" 
    "Constants.h" 
    "util/BenchmarkTimer.h" "util/BenchmarkTimer.cpp"
    "indexing/SuffixArray.h" "indexing/SuffixArray.cpp"
    "compression/Huffman.h" "compression/Huffman.cpp"
)

target_include_directories(ipmt
    PRIVATE
    util/
    ./
    compression/
    indexing/
)

# TODO: Add tests and install targets if needed.
