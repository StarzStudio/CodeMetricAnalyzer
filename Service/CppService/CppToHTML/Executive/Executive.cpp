// Executive.cpp : Defines the entry point for the console application.
//

#include "Executive.h"
#include <string>




int main(int argc, char **argv)
{
	std::string templateFile (argv[1]);

	std::string destHTMLFolder(argv[2]);
	std::string searchPath(argv[3]);
	std::vector<std::string> searchPatterns;
	for  (int i = 4; i < argc; i++) {
		searchPatterns.push_back(std::string(argv[i]));
	}
	FileManager::IFileMgr *pFileMgr = new FileManager::FileMgr(searchPath);
	HTMLgenerator g( templateFile, destHTMLFolder);
	pFileMgr->regForFiles(&g);
	for (auto p : searchPatterns) {
		pFileMgr->addPattern(p);
	}
	pFileMgr->search();

	std::vector<std::string> htmlContentCollection = g.HTMLContentCollection();
	for (auto e : htmlContentCollection) {
		std::cout << e << std::endl;
	}
	return 0;
}
