////////////////////////////////////////////////////////////////////////
//  MetricAnalysis.cpp - declares new parsing rules and actions       //
//  ver 2.0                                                           //
//  Platform:      Mac Book Pro, OS X EI Captain 10.11                //
//  Application:   Code Analysis, 687 OOD Project2                    //
//  Author:        Xing Zhou, Syracuse University                     //
//                 Software Engineering                               //
//                 xzhou118@syr.edu                                   //
//  Source:        Jim Fawcett, CST 4-187, Syracuse University        //
//                 (315) 443-3948, jfawcett@twcny.rr.com              //
////////////////////////////////////////////////////////////////////////

#include "../MetricsAnalysis/MetricsAnalysis.h"


using namespace rapidjson;


std::string MetricAnalysis::metricInfoJSON() {
    Document document;
    document.SetObject();
    Document::AllocatorType &allocator = document.GetAllocator();

    Value metricValue(kObjectType);

    Value functions(kArrayType);
    for (auto it = this->_fileMetricInfo._funcMetricInfoCollection.begin();
         it != this->_fileMetricInfo._funcMetricInfoCollection.end();
         it++) {
        Value func(kObjectType);
        Value name(kStringType);
        std::string name_str(it->second.name);
        name.SetString(name_str.c_str(),
                       name_str.size(), allocator);

        func.AddMember("name", name, allocator);

        Value size;
        size.SetInt(it->second.size);

        func.AddMember("size", size, allocator);

        Value complexity;
        complexity.SetInt(it->second.complexity);

        func.AddMember("complexity", complexity, allocator);

        functions.PushBack(func, allocator);
    }


    Value overlinedFunctions(kArrayType);
    for (auto e : this->_fileMetricInfo.overlinedFunctions) {
        Value overlinedFunc(kStringType);
        overlinedFunc.SetString(e.c_str(),
                                e.size(), allocator);

        overlinedFunctions.PushBack(overlinedFunc, allocator);
    }
    Value overComplexFunctions(kArrayType);
    for (auto e : this->_fileMetricInfo.overComplexityFunctions) {
        Value overComlexFunc(kStringType);
        overComlexFunc.SetString(e.c_str(),
                                 e.size(), allocator);

        overComplexFunctions.PushBack(overComlexFunc, allocator);
    }


    metricValue.AddMember("functions", functions, allocator);
    metricValue.AddMember("overlinedFunctions", overlinedFunctions, allocator);
    metricValue.AddMember("overComplexFunctions", overComplexFunctions, allocator);

    Value root(kObjectType);

     Value fileName(kStringType);
        fileName.SetString(this->_currentFile.c_str(), this->_currentFile.size(), allocator);
    root.AddMember("fileName", fileName, allocator);
    root.AddMember("metricValue", metricValue, allocator);
    StringBuffer buffer;
    Writer<StringBuffer> writer(buffer);
    root.Accept(writer);
    std::string result = buffer.GetString();
    //std::cout << result << std::endl;
    return result;
}

// wrapper of the TreeWalk function
void MetricAnalysis::analyzeFunc() {
    std::cout << "\n  The size and complexity of functions in this package are: ";
    std::cout << "\n  =========================================================";
    std::string travserResult = TreeWalk(tree->getRoot());
    //metricsResultBuf << travserResult;
    //std::cout << travserResult;
}

// walk through tree and display information of nodes with function type
std::string MetricAnalysis::TreeWalk(ASTNode *root) {
    std::ostringstream temp;

    static size_t indentLevel = 0;
    //std::cout << "root name:" << root->name() << root->size() << root->complexity() << std::endl;
    if (root->type() == "function") {
        FuncMetricInfo funcInfo(root->name(),
                                root->size(),
                                root->complexity());

        _fileMetricInfo._funcMetricInfoCollection[root->name()] = funcInfo;

        if (root->size() > 50) {
            this->_fileMetricInfo.overlinedFunctions.push_back(root->name());
        }
        if (root->complexity() > 10) {
            this->_fileMetricInfo.overComplexityFunctions.push_back(root->name());
        }
    }
    auto iter = root->children().begin();
    ++indentLevel;
    while (iter != root->children().end()) {
        temp << TreeWalk(*iter);
        ++iter;
    }
    --indentLevel;
    return temp.str();
}

//----< test stub >------------------------------------------------------

#ifdef TEST_METRICSANALYSIS
int main()
{
    std::cout << "\n  Testing Metrics Analysis tool based on DFS traserval of AST";
    std::cout << "\n  Notice in this test, I assume the AST is already exits";
    std::cout << "\n  =====================================================  \n";


    ASTNode* root = new ASTNode("root", "class");
    root->setStartLine(1);
    root->setEndLine(156);
    root->setComplexity(16);

    ASTNode* fun1 = new ASTNode("fun1","function");
    fun1->setStartLine(22);
    fun1->setEndLine(155);
    fun1->setComplexity(22);

    ASTNode* fun2 = new ASTNode("fun2", "function");
    fun2->setStartLine(56);
    fun2->setEndLine(178);
    fun2->setComplexity(12);

    ASTNode* fun3 = new ASTNode("fun3", "function");
    fun3->setStartLine(79);
    fun3->setEndLine(183);
    fun3->setComplexity(26);

    ASTNode* fun4 = new ASTNode("fun4", "function");
    fun4->setStartLine(104);
    fun4->setEndLine(156);
    fun4->setComplexity(7);

    // add functions to root's children     
     /* structure:
           * root
          /
         *   fun1
        /
       *     fun2
      /
     *       fun3
    /
   *         fun4          */

    root->addChildren(fun1);
    fun1->addChildren(fun2);
    fun2->addChildren(fun3);
    fun3->addChildren(fun4);
    ASTTree* tree = new ASTTree(root);

    // create analyzer
    MetricAnalysis analyzer;
    analyzer.setTree(tree);
    analyzer.analyzeFunc();
    analyzer.metricInfoJSON();
    std::cout << std::endl;

}
#endif