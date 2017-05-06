{
	"targets": [
		{
            			"target_name": "cppToHTML",
            			"defines": ["NODE_ENV"],
            			"sources": [
            			"Service/CppService/CppToHTML/Executive/Executive.cpp",
            			"Service/CppService/CppToHTML/FileMgr/FileMgr.cpp",
            			"Service/CppService/CppToHTML/FileSystem/FileSystem.cpp",
            		    "Service/CppService/CppToHTML/HTMLgenerator/HTMLgenerator.cpp",
            	   	    "Service/CppService/CppToHTML/Utilities/Utilities.cpp",
            			],
            			'cflags' : [
            			    '-std=c++14',
            			    '-fpermissive',
            			    'fexceptions'
            			],
            			  'conditions': [
                                             [ 'OS=="mac"', {

                                               'xcode_settings': {

                                                 'GCC_ENABLE_CPP_EXCEPTIONS': 'YES'
                                                 },

                                             }],
                          ],


         },
         {
             "target_name": "metricAnalyzer",
             "defines": ["NODE_ENV"],
             "sources": [
                  "Service/CppService/metricsAnalyzer/MetricsAnalysis/MetricsExecutive.cpp",
                  "Service/CppService/metricsAnalyzer/ASTNode/ASTNode.cpp",
                  "Service/CppService/metricsAnalyzer/FileSystem/FileSystem.cpp",
                  "Service/CppService/metricsAnalyzer/Parser/Parser.cpp",
                  "Service/CppService/metricsAnalyzer/Parser/Parser.cpp",
                  "Service/CppService/metricsAnalyzer/Parser/ConfigureParser.cpp",
                  "Service/CppService/metricsAnalyzer/Parser/ActionsAndRules.cpp",
                   "Service/CppService/metricsAnalyzer/SemiExp/SemiExp.cpp",
                   "Service/CppService/metricsAnalyzer/Tokenizer/Tokenizer.cpp",
                   "Service/CppService/metricsAnalyzer/ScopeStack/ScopeStack.cpp",
                   "Service/CppService/metricsAnalyzer/MetricsAnalysis/MetricsAnalysis.cpp",
                   "Service/CppService/metricsAnalyzer/Utilities/Utilities.cpp",
             ],
             'cflags' : [
                 '-std=c++14',
                '-fpermissive',
                'fexceptions'
             ],
              'conditions': [
                     [ 'OS=="mac"', {

                       'xcode_settings': {

                         'GCC_ENABLE_CPP_EXCEPTIONS': 'YES'
                         },

                     }],
              ],

         }
	]

}
