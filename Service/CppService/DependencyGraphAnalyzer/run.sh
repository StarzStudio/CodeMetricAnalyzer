# build project first
make
# the generated html files will be stored in CppToHTML/htmlFolder/
./dependencyAnalyzer "." "*.h" "*.cpp" "*.CC" ".c" "*.C"
