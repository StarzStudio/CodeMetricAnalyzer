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

         }
	]

}
