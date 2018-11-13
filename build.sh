#g++ src/*.cpp -Ithird_party/repos/MetaStuff/include -Ithird_party/include -o a.out -std=c++17
CMAKE_FLAGS="\
 -DBUILD_TESTS=ON \
 -DWITH_GL=ON \
 -DWITH_UTILITY=ON \
 -DWITH_META=ON \
 -DWITH_MATH=ON \
 -DWITH_SYSTEM=OFF \
 -DWITH_GEOMETRY=ON \
 "


if [ "$1" == "clean" ]; then
    git clean --exclude=data/savefile_dirs.txt -xid
    exit $?
fi

mkdir -p build
set -e
cd build 

if [ "$1" == "ycm" ]; then
	ycm_generator -vfb cmake .. --configure_opts="$CMAKE_FLAGS"
    exit $?
fi

cmake .. -G "Ninja" \
	$CMAKE_FLAGS 
cmake --build . --config Debug
./src/Terrific/Meta/Test/MetaTest

if [ "$1" == "run" ]; then
    ./src/Example/MyApplication
    #cd src/Terrific/System/Test && ./SystemTest
fi

#exit 24
exit $?

