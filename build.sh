mkdir -p build
cd build
cmake ..
cmake --build .
cp ../assets . -r
./breakout
