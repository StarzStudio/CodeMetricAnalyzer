////////////////////////////////////////////////////////////////////////
//  MetricAnalysis.cpp - declares new parsing rules and actions       //
//  ver 1.3                                                           //
//  Platform:      Mac Book Pro, OS X EI Captain 10.11                //
//  Application:   Code Analysis, 687 OOD Project2                    //
//  Author:        Xing Zhou, Syracuse University                     //
//                 Software Engineering                               //
//                 xzhou118@syr.edu                                   //
//  Source:        Jim Fawcett, CST 4-187, Syracuse University        //
//                 (315) 443-3948, jfawcett@twcny.rr.com              //
////////////////////////////////////////////////////////////////////////

#include "../MetricsAnalysis/MetricsAnalysis.h"

// wrapper of the TreeWalk function
void MetricAnalysis::analyzeFunc()
{
    std::cout << "\n  The size and complexity of functions in this package are: ";
    std::cout << "\n  =========================================================";
    std::string travserResult = TreeWalk(tree->getRoot());
	metricsResultBuf << travserResult;
	std::cout << travserResult;
}

// walk through tree and display information of nodes with function type
std::string MetricAnalysis::TreeWalk(ASTNode* root)
{
    std::ostringstream temp;
    static size_t indentLevel = 0;
    if (root->type() == "function")
    {
        //if (root->size() > 50 || root->complexity() > 10)               // to test my project fucntions whether meeting requirements
        //{
            temp << "\n  " << root->name() << ":";
            temp << "\n        " << "size: " << root->size() << ":";
            if (root->size() > 50) {
                this->_metricInfo.overlinedFunctions.push_back(root->name());
            }
            if (root->complexity() > 50) {
                this->_metricInfo.overComplexityFunctions.push_back(root->name());
            }
            temp << "\n        " << "complexity: " << root->complexity() << ":\n";

        //}
    }
    auto iter = root->children().begin();
    ++indentLevel;
    while (iter != root->children().end())
    {
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
    fun1->setEndLine(55);
    fun1->setComplexity(8);

    ASTNode* fun2 = new ASTNode("fun2", "function");
    fun2->setStartLine(56);
    fun2->setEndLine(78);
    fun2->setComplexity(7);

    ASTNode* fun3 = new ASTNode("fun3", "function");
    fun3->setStartLine(79);
    fun3->setEndLine(103);
    fun3->setComplexity(6);

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


    // create analyzer
    MetricAnalysis analyzer;
    if (analyzer.build("./MetricsAnalysis.cpp"))
    {
        std::cout << "\n  analyzer go through the AST: \n";
        analyzer.analyzeFunc();
    }
    else
        std::cout << "build process failed";
    
    std::cout << std::endl;

}
#endif