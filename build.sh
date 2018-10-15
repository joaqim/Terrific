#g++ src/*.cpp -Ithird_party/repos/MetaStuff/include -Ithird_party/include -o a.out -std=c++17
mkdir -p build
set -x
cd build

cmake .. -G "Ninja" -DBUILD_TESTS=ON &&  \
cmake --build . --config Debug && \
cd src/Terrific/System/Test && \
	./SystemTest

#&& \ ./src/Example/MyApplication
