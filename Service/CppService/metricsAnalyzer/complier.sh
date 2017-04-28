g++ -o MetricsAnalyzer -std=c++14 -DTEST_METRICSEXECUTIVE \
./MetricsAnalysis/MetricsAnalysis.cpp \
./MetricsAnalysis/MetricsExecutive.cpp \
./Tokenizer/Tokenizer.cpp ./Parser/ConfigureParser.cpp \
./Parser/ActionsAndRules.cpp \
./Parser/Parser.cpp \
./ScopeStack/ScopeStack.cpp \
./SemiExp/SemiExp.cpp \
./FileSystem/FileSystem.cpp \
./ASTNode/ASTNode.cpp \
./Utilities/Utilities.cpp \


