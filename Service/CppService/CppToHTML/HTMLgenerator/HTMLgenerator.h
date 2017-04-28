#ifndef HTMLGENERATOR_H
#define HTMLGENERATOR_H

/////////////////////////////////////////////////////////////////////
// HTMLgenerator.h - generater html file for a .h or .cpp file     //
// ver 1.3                                                         //
// platform: Win 10												   //		
// Xing Zhou, work.xingzhou@gmail.com, Syracuse University         //
// Application: MEAN stack using C++ to analyze code metrics       //
/////////////////////////////////////////////////////////////////////
/*
* Package operation: 
* Take in a full spec c++ file name, generate its html file version
* using HTMLgenerator::transferFileType(std::string) 
*
* Note:
* - Shall provide a template html file
* - Shall specify a relative dest folder to store html files. 
*   original folder is /Debug
* 
* Maintenance History :
*--------------------
* ver 1.3 : 26 Apr 2016
* -move readFileContent() into head file, otherwise nodejs runtime will raise undefine symbol: readFileContent
* ?don't know why
* ver 1.2 : 25 Apr 2016
* -Fix Bug: storeFileContent issue: 
			must specify the correct folder by using FileSystem::Dirtory::create and 
			Dirtory::setCurrentDir() to go to that folder, then use Path::getName() to extract fileName
			out of the absolute path to store it.
* ver 1.1 : 24 Apr 2016
* -Fix Bug: readFileContent can not read file content
            - file path shall be absolute path, not relative
			- if using FileSystem::Path::getFullFileSpec("Only File name"), will generate 
			  false absolute path. Must be input absolute path in FileMgr using Path::FileSpec(path, FileName)
* ver 1.0 : 23 Apr 2016
* -first release
*/



#include <string>
#include <fstream>
#include <iostream>
#include "../FileMgr/IFileMgr.h"
#include "../FileSystem/FileSystem.h"
#include <vector>


// transfer single cpp file to html file 
class HTMLgenerator : public FileManager::IFileEventHandler {
	
public:
	using Path = std::string;
	using File = std::string;
	HTMLgenerator(Path in_templateHTMLFilePath, Path in_htmlDestFolder):
		_templateHTMLFilePath(in_templateHTMLFilePath),
		_htmlDestFolder(in_htmlDestFolder){
		// get HTML template content
		_templateHTML = readFileContent(_templateHTMLFilePath);
	}

	// transfer from cpp source text to html content
	void transferFileType(File in_fileName);
	virtual void execute(const File & in_fileName) {
		
	//	std::cout << in_fileName.c_str() << std::endl;
		if (in_fileName.find(".cpp") != std::string::npos ||
			in_fileName.find(".h") != std::string::npos) {
			transferFileType(in_fileName);
		}
	}
private:

	// given file path, read in its content
	void replaceSymbol(File& cppFileContent) ;
	//void HTMLgenerator::removeFile (const std::string& in_filename);

 	File readFileContent(File in_fileName) {
	//	std::cout << in_fileName.c_str() << std::endl;
		std::ifstream ifs(in_fileName);
		File content((std::istreambuf_iterator<char>(ifs)),
			(std::istreambuf_iterator<char>()));
		return content;
	}
	// store file content
	void storeFileContent(File in_htmlFileName, File in_htmlContent);
	// inject placeholder into html template
	File injectPlaceHolderIntoTemplate(std::vector<File> in_placeholder);
	// change \n to <br> , space tpo &nbsp
	void replaceSymbol(File& in_FileContent, File in_old, File in_new);
	// transfer cpp file name to html file name 
	File getHTMLFileName(File cpp_fileName);

	File _templateHTML;
	Path _templateHTMLFilePath;
	Path _htmlDestFolder;
};

#endif