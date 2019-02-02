#!/bin/sh
set -e
event() {
    set +e
    thing="$1"
    shift
    printf "\033[1m::\033[m %15b... " "$thing"
    "$@"
    if [ $? -eq 0 ]; then
        printf "\033[32mok\033[m\n"
    else
        printf "\033[31mfailed\033[m\n"
        exit 1
    fi
}
rm -rf release
mkdir -p build release/include/kappa
for file in $(find lib/src -name *.cpp); do
    name=$(basename $file .cpp)
    event "compiling $name.o" g++.exe -g -c $file -o build/$name.o -Ilib/include
done
event "archiving libkappa.a" ar.exe rcs release/libkappa.a $(find build -name *.o)
event "cleaning build directory" rm -rf build
event "copying in include directory" cp lib/include/*.hpp release/include/kappa
for example in $(find examples -name *.cpp); do
    name=$(basename $example .cpp)
    event "compiling $name example" g++.exe $example -g -Irelease/include -Lrelease -lkappa -o release/$name.exe
done
