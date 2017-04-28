{
	"targets": [
		{
			"target_name": "palindrome",
			"sources": [ "palindrome.cc" ],
			"include_dirs": [
				"<!(node -e \"require('nan')\")"
			]
		} ,
		{
            			"target_name": "cppToHTML",
            			"sources": [
            			"Service/CppService/CppToHTML/Executive/Executive.cpp",
            			"Service/CppService/CppToHTML/FileMgr/FileMgr.cpp",
            			"Service/CppService/CppToHTML/FileSystem/FileSystem.cc",
            		    "Service/CppService/CppToHTML/HTMLgenerator/HTMLgenerator.cpp",
            	   	    "Service/CppService/CppToHTML/Utilities/Utilities.cpp",
            			],
            			'cflags' : [
            			    '-std=c++14',
            			    '-fpermissive'
            			],
            			'cflags!' : [
            			    '-fno-exceptions'
            			],
            			'cflags_cc!' : [
            			    '-fno-exceptions'
            			]

         },
         {
             "target_name": "metricAnalyzer",
             "sources": [
                    "Service/CppService/CodeAnalyzer/MetricsAnalysis/MetricsExecutive.cpp",
                  "Service/CppService/CodeAnalyzer/ASTNode/ASTNode.cpp",
                  "Service/CppService/CodeAnalyzer/FileSystem/FileSystem.cpp",
                   "Service/CppService/CodeAnalyzer/Parser/Parser.cpp",
                    "Service/CppService/CodeAnalyzer/Parser/Parser.cpp",
                     "Service/CppService/CodeAnalyzer/Parser/ConfigureParser.cpp",
                      "Service/CppService/CodeAnalyzer/Parser/ActionsAndRules.cpp",
                   "Service/CppService/CodeAnalyzer/SemiExp/SemiExp.cpp",
                   "Service/CppService/CodeAnalyzer/Tokenizer/Tokenizer.cpp",
                   "Service/CppService/CodeAnalyzer/ScopeStack/ScopeStack.cpp",
                   "Service/CppService/CodeAnalyzer/MetricsAnalysis/MetricsAnalysis.cpp",
                   "Service/CppService/CodeAnalyzer/Utilities/Utilities.cpp",
             ],
             'cflags' : [
                 '-std=c++14',
                '-fpermissive'
             ],
             'cflags!' : [
                 '-fno-exceptions'
              ],
              'cflags_cc!' : [
                  '-fno-exceptions'
            	]

         }
	]

}
