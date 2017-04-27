/////////////////////////////////////////////////////////////////////
// HTMLgenerator.cpp - generater html file for a .h or .cpp file   //
// ver 1.2                                                         //
// platform: Win 10												   //		
// Xing Zhou, work.xingzhou@gmail.com, Syracuse University         //
// Application: MEAN stack using C++ to analyze code metrics       //
/////////////////////////////////////////////////////////////////////

#include "HTMLgenerator.h"


using Path = std::string;
using File = std::string;

// transfer from cpp source text to html content
void HTMLgenerator::transferFileType(File in_fileName) {
	File cppFileContent = this->readFileContent(in_fileName);
	if (cppFileContent == "") {
		return;
	}
	replaceSymbol(cppFileContent);
	File shortName = FileSystem::Path::getName(in_fileName);
	std::vector<File> placeholder = { shortName, cppFileContent };
	File htmlFileContent = injectPlaceHolderIntoTemplate(placeholder);
	File htmlFileName = getHTMLFileName(in_fileName);
	storeFileContent(htmlFileName, htmlFileContent);
	//removeFile(in_fileName);
}

void HTMLgenerator::replaceSymbol(File& cppFileContent) {
    replaceSymbol(cppFileContent, "&", "&amp");
    replaceSymbol(cppFileContent, "<", "&lt");
	replaceSymbol(cppFileContent, ">", "&gt");
	replaceSymbol(cppFileContent, "\n", "<br>");
	replaceSymbol(cppFileContent, "\"", "&quot");
	replaceSymbol(cppFileContent, "\t", "&nbsp&nbsp&nbsp&nbsp");
	replaceSymbol(cppFileContent, " ", "&nbsp");
}

//void HTMLgenerator::removeFile (const std::string& in_filename) {
//	FileSystem::File::remove(in_filename);
//}


// store file content

inline void HTMLgenerator::storeFileContent(File in_htmlFileName, File in_htmlContent) {

	if (!FileSystem::Directory::exists(_htmlDestFolder)) {
		FileSystem::Directory::create(_htmlDestFolder);
	}
	

	File fileName = FileSystem::Path::getName(in_htmlFileName);
	File newFilePath =  FileSystem::Path::fileSpec(_htmlDestFolder, fileName);
	std::ofstream htmlFile(newFilePath);
	htmlFile << in_htmlContent;
	htmlFile.close();
}

// inject placeholder into html template

inline File HTMLgenerator::injectPlaceHolderIntoTemplate(std::vector<File> in_placeholder) {
	File htmlContent = _templateHTML;
	File fileName = in_placeholder[0];
	File fileContent = in_placeholder[1];

	size_t posOfFileName = htmlContent.find(std::string("<title>")) + 7;
	htmlContent.insert(posOfFileName, fileName);

	size_t posOfFileContent = htmlContent.find(std::string("<div>")) + 5;
	htmlContent.insert(posOfFileContent, fileContent);
	return htmlContent;
}

// change \n to <br> , space tpo &nbsp

inline void HTMLgenerator::replaceSymbol(File & in_FileContent, File in_old, File in_new) {

	size_t p = 0;
	while (p < in_FileContent.size()) {
		p = in_FileContent.find(std::string(in_old), p);
		if (p == std::string::npos) break;
		in_FileContent.replace(p, in_old.size(), in_new);
		p += in_new.size();
	}
}

// transfer cpp file name to html file name 

inline File HTMLgenerator::getHTMLFileName(File cpp_fileName) {

	File htmlFileName = cpp_fileName + ".html";
	return htmlFileName;
}




#ifdef TEST_FILEMGR

int main()
{

	return 0;
}

#endif
