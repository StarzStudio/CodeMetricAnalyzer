#ifndef EXECUTIVE_H
#define EXECUTIVE_H


#include "../HTMLgenerator/HTMLgenerator.h"
#include "../FileMgr/FileMgr.h"
#include "sys/types.h"
#include "../FileSystem/FileSystem.h"


#ifdef NODE_ENV

#include <node.h>
#include <v8.h>
using namespace v8;



void generateHTML(const FunctionCallbackInfo<Value>& args) {
	 Isolate* isolate = args.GetIsolate();
    HandleScope scope(isolate);
    // get tempalte html file
	String::Utf8Value _templateFile(args[0]->ToString()); //convert to std::string
	std::string templateFile = std::string(*_templateFile); 
	std::cout << "html template file path: " << templateFile.c_str() << std::endl;
	 std::cout << "====================================================" << std::endl;
	
    // get destination path to store html files
	String::Utf8Value _destHTMLFolder(args[1]->ToString()); //convert to std::string
 	std::string destHTMLFolder = std::string(*_destHTMLFolder); 
    std::cout << "html destination path: " << destHTMLFolder.c_str() << std::endl;
	std::cout << "====================================================" << std::endl;
    // start path for file manager to search cpp files 
	String::Utf8Value _searchPath(args[2]->ToString()); //convert to std::string
	std::string searchPath  = std::string(*_searchPath); 
// 	std::cout << "searching path: " << FileSystem::Directory::getCurrentDirectory().c_str() << std::endl;
	std::cout << "searching path: " << searchPath.c_str() << std::endl;
	std::cout << "====================================================" << std::endl;
	
	// provide patterns for file manager
	std::vector<std::string> searchPatterns = {"*.h", "*.cpp"};

	FileManager::IFileMgr *pFileMgr = new FileManager::FileMgr(searchPath);
 	HTMLgenerator g( templateFile, destHTMLFolder);
	pFileMgr->regForFiles(&g);
	for (auto p : searchPatterns) {
		pFileMgr->addPattern(p);
	}
	pFileMgr->search();

	std::vector<std::string> htmlContentCollection = g.HTMLContentCollection();
	 Local<Array> array = Array::New(isolate,htmlContentCollection.size());

    for (int i = 0; i < htmlContentCollection.size(); i++) {
        array->Set(i,String::NewFromUtf8(isolate, htmlContentCollection[i].c_str()));
    }


    args.GetReturnValue().Set(array);
}


void Init(Local<Object> exports, Local<Object> module) {
  NODE_SET_METHOD(module, "exports", generateHTML);
}

NODE_MODULE(generateHTML, Init)

#endif

#endif
