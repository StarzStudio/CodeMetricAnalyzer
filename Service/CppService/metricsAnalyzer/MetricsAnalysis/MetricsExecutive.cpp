 ////////////////////////////////////////////////////////////////////////
//  MetricExecutive.cpp - declares new parsing rules and actions      //
//  ver 1.4                                                           //
//  Platform:      Mac Book Pro, OS X EI Captain 10.11                //
//  Application:   Code Analysis, 687 OOD Project2                    //
//  Author:        Xing Zhou, Syracuse University                     //
//                 Software Engineering                               //
//                 xzhou118@syr.edu                                   //
//  Source:        Jim Fawcett, CST 4-187, Syracuse University        //
//                 (315) 443-3948, jfawcett@twcny.rr.com              //
////////////////////////////////////////////////////////////////////////

#include "MetricsExecutive.h"

using namespace FileSystem;
using Path = std::string;
using File = std::string;
using Files = std::vector<std::string>;
using Pattern = std::string;
using Patterns = std::vector<std::string>;



// initialization for all the parts which Executive needed
bool MetricExecutive::build() {


    // Configure the Parser and Repository
    configure = new ConfigParseToConsole();
    pParser = configure->Build();
    if (pParser)
        return true;
    else
        return false;
}

// take in a single file and generate an AST for it
ASTTree *MetricExecutive::generateTree(const std::string &fileName) {
    // rebuild configureConsole, mainly to reset pToker's pState pointer, if not, 
    // the initial state pointer will point to nullptr when receive a new file
    // result from reading at the end of last file 
    if (!build()) {
        std::cout << "build process failed";
        return NULL;
    }
    try {
        if (pParser) {
            if (!configure->Attach(fileName)) {
                std::cout << "\n  could not open file " << fileName << std::endl;
                return NULL;
            }
        } else {
            std::cout << "\n\n  Parser not built\n\n";
            return NULL;
        }
        // now that parser is built, use it

        while (pParser->next())
            pParser->parse();
        std::cout << "\n";
    }
    catch (std::exception &ex) {
        std::cout << "\n\n    " << ex.what() << "\n\n";
    }

    // get AST 
    ASTTree *tree = configure->getRepo()->getASTTree();
    // reset the line count number
    configure->resetLineCount();
    return tree;
}

// file all the files meet the pattern in a given directory
void MetricExecutive::fileSearch(const std::string &path) {
    //std::cout << "Current dir: " << FileSystem::Directory::getCurrentDirectory() << std::endl;
    std::string fpath = FileSystem::Path::getFullFileSpec(path);
    // std::cout << "path name: " << fpath << std::endl;

    for (auto patt : _patterns) {
        std::vector <std::string> files = FileSystem::Directory::getFiles(fpath, patt);
        if (files.size() == 0) {
            //   std::cout << "there is no files" <<  std::endl;
        }

        for (auto f : files) {
            File _file = FileSystem::Path::fileSpec(fpath, f);
            //   std::cout << "file name: " << _file << std::endl;
            this->_fileCollection.push_back(_file);
        }
    }
    std::vector <std::string> dirs = FileSystem::Directory::getDirectories(fpath);
    for (auto d : dirs) {
        if (d == "." || d == "..")
            continue;
        std::string dpath = fpath + "/" + d;
        fileSearch(dpath);
    }

}

// analysis all the files meet the pattern in a given directory
void MetricExecutive::start() {
    // do analysis
    fileSearch(_path);
    if (_fileCollection.size() == 0) {
        std::cout << "whooops....can not find any file matches : (";
        return;
    }
    for (int i = 0; i < _fileCollection.size(); i++) {
        std::string currentFile = FileSystem::Path::getName(_fileCollection[i]);
        std::cout << "\n  ==============================================================";
        std::cout << "\n  The No." << i + 1 << " file is: " << currentFile << std::endl;
//        analyzer.metricsResultBuf << "\n  ==============================================================";
//        analyzer.metricsResultBuf << "\n  The No." << i + 1 << " file is: " << currentFile;

        ASTTree *tree = generateTree(_fileCollection[i]);

        bool showTree = true;
        if (showTree) {
            std::cout << tree->ExtractScopeInfo();
        }
        analyzer.setTree(tree);
        analyzer.analyzeFunc();
        _metricInfoCollection.push_back(analyzer.metricInfoJSON());
        analyzer.clean();
    }
}

//----< test stub >------------------------------------------------------

#ifdef TEST_METRICSEXECUTIVE

int main(int argc, char ** argv)
{
    std::cout << "\n  Testing Metrics Executive tool which analyzes a set of packages";
    std::cout << "\n  user need to type (path, file pattern) in commmand line";
    std::cout << "\n  =====================================================  \n";

    // create analyzer
    if (argc < 2) {
        std::cout << "  Please provide paras. \n  example: ./a.out path pattern1 [pattern2, ...]" << std::endl;
        return -1;
    }
    std::vector<std::string> patterns;
    std::string path = argv[1];
    for (int i = 2; i < argc; i ++) {
     patterns.push_back(argv[i]);
    }

    MetricExecutive exe(path, patterns);


    // test find files meeting a specific pattern given a path name
  //  analyzer.addPattern("1s");
  //  analyzer.addPattern("1p");
  //  std::vector<std::string> fileCollection = analyzer.fileSearch("..");
    // display the files that has found
    // for (int i = 0; i < fileCollection.size(); i++)
    // {
    //     std::cout << "  The No." << i + 1 << " file is: " << fileCollection[i] << std::endl;
    // }

       // test analysis for a single file:
       // if (!analyzer.analyzeSingle("./MetricsAnalysis.cpp"))
       //     std::cout << "\n  analysis failed";          // if analyzeFunc return false

        // test analysis for a several files:
       /* if (!analyzer.analyzeSingle("./MetricExecutive.cpp")) 
            std::cout << "\n  analysis failed";
        if (!analyzer.analyzeSingle("./MetricsAnalysis.h"))
            std::cout << "\n  analysis failed";*/
   
  

    // test Analyze for a given Directory
  // std::ofstream metricsResult("metricsResult.txt");


   exe.start();
   Files metrics =  exe.metricInfoCollection();

   for (auto s : metrics) {
    std::cout << s << std::endl;
   }
//   analyzer.analyzer.metricsResultBuf << "\n\n\n";
//   for (auto it = metrics.begin() ; it != metrics.end() ; it++) {
//        if (it->second.overlinedFunctions.size() > 0 ||
//            it->second.overComplexityFunctions.size() > 0 ) {
////            std::cout << "File: " << it->first << " need to be improved." << std::endl;
////			analyzer.analyzer.metricsResultBuf <<  "File: " << it->first << " need to be improved.\n" ;
//        }
//         if (it->second.overlinedFunctions.size() > 0 ) {
//            std::cout << "  functions that exceed 50 lines: "  << std::endl;
//             for (auto e : it->second.overlinedFunctions) {
////                std::cout << "    " << e << std::endl;
////				analyzer.analyzer.metricsResultBuf <<  "    " << e << "\n";
//             }
//         }
//         if (it->second.overComplexityFunctions.size() > 0 ) {
//            std::cout << "  functions that exceed 10 complexity: "  << std::endl;
//             for (auto e : it->second.overComplexityFunctions) {
////                 std::cout << "    " << e << std::endl;
////				 analyzer.analyzer.metricsResultBuf  <<  "    " << e << "\n";
//             }
//         }
//   }
  // std::cout << "buf is:" << analyzer.analyzer.metricsResultBuf.str() << std::endl;
//   std::string bufferContent = analyzer.analyzer.metricsResultBuf.str();
//   metricsResult << bufferContent;
//   metricsResult.close();
  
}
#endif