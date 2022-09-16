if command -v clang &> /dev/null; then
    compiler="clang"
elif command -v gcc &> /dev/null; then
    compiler="gcc"
else
    echo "could not find a valid compiler"
    exit 1
fi

libs=("graph/csgraph" "utils/errors" "list/vector" "set/hashset" "queue/heap")
libs_s=""
for lib in ${libs[@]}; do
    libs_s="${libs_s} src/${lib}.c"
done

files=("test" "gen" "exe1" "exe2" "exe3")

rm -rf ./build &> /dev/null
mkdir build &> /dev/null
for file in ${files[@]}; do
    ${compiler} $1 src/${file}.c ${libs_s} -o build/${file}
done
