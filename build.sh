#g++ src/*.cpp -Ithird_party/repos/MetaStuff/include -Ithird_party/include -o a.out -std=c++17
mkdir -p build
set -x
cd build

cmake .. -G "Ninja"\
       	-DBUILD_TESTS=OFF \
	-DWITH_MATH=ON \
	-DWITH_GEOMETRY=ON \
	  &&  \
	  cmake --build . --config Debug && \
	  ./src/TestPlanet/MyApplication
#./src/Example/MyApplication
#cd src/Terrific/System/Test && ./SystemTest

