// Executive.cpp : Defines the entry point for the console application.
//

#include "Executive.h"


int main()
{
	std::string path = "..";

	FileManager::IFileMgr *pFileMgr = new FileManager::FileMgr(path);
	HTMLgenerator g("../HTMLgenerator/template.htm", "../htmlFolder");
	pFileMgr->regForFiles(&g);
	pFileMgr->addPattern("*.h");
	pFileMgr->addPattern("*.cpp");
	pFileMgr->search();
	return 0;
}
