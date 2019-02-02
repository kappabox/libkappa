#!/bin/sh
event() {
    thing="$1"
    shift
    printf "\033[1m…\033[m %-15b \r" "$thing"
    ("$@" 2>build.log)
    if [ $? -eq 0 ]; then
        printf "\033[32m✓\033[m\n"
    else
        printf "\033[31m✗\033[m\n"
        exit 1
    fi
}
event "cleaning up previous builds" rm -rf release build.log
event "generating build directories" mkdir -p build release/include/kappa
for file in $(find lib/src -name *.cpp); do
    name=$(basename $file .cpp)
    event "compiling $name" g++.exe -g -c $file -o build/$name.o -Ilib/include
done
event "archiving libkappa" ar.exe rcs release/libkappa.a $(find build -name *.o)
event "cleaning build directory" rm -rf build
event "copying in include directory" cp lib/include/*.hpp release/include/kappa
for example in $(find examples -name *.cpp); do
    name=$(basename $example .cpp)
    event "compiling example '$name'" g++.exe $example -g -Irelease/include -Lrelease -lkappa -o release/$name.exe
done
