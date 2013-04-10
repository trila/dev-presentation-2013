
# options

usage(){
cat << EOF
usage: $0 [options]

Test the C++ <-> JS bindings generator

OPTIONS:
-h	this help

Dependencies :
PYTHON_BIN
CLANG_ROOT
NDK_ROOT

Define this to run from a different directory
CXX_GENERATOR_ROOT

EOF
}

while getopts "dvh" OPTION; do
case "$OPTION" in
d)
debug=1
;;
v)
verbose=1
;;
h)
usage
exit 0
;;
esac
done

# exit this script if any commmand fails
set -e

# paths

CLANG_ROOT="$HOME/bin/clang+llvm-3.1"
PYTHON_BIN="/opt/local/bin/python2.7"
CXX_GENERATOR_ROOT="/Users/minggo/SourceCode/cocos2d-x/tools/bindings-generator"

_CONTENTS=""
_CONTENTS+="[DEFAULT]"'\n'
_CONTENTS+="androidndkdir=$NDK_ROOT"'\n'
_CONTENTS+="clangllvmdir=$CLANG_ROOT"'\n'
_CONTENTS+="cxxgeneratordir=$CXX_GENERATOR_ROOT"'\n'

echo -e $_CONTENTS > userconf.ini


# Generate bindings for simpletest using Android's system headers
echo "Generating bindings for simpletest with Android headers..."
set -x
LD_LIBRARY_PATH=${CLANG_ROOT}/lib $PYTHON_BIN ${CXX_GENERATOR_ROOT}/generator.py /Users/minggo/SourceCode/dev-presentation-2013/tool/test.ini -s testandroid -o /Users/minggo/SourceCode/dev-presentation-2013/GeneratorSample/GeneratorSample/Classes
