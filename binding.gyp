{
	"targets": [
		{
			"target_name": "palindrome",
			"sources": [ "palindrome.cc" ],
			"include_dirs": [
				"<!(node -e \"require('nan')\")"
			]
		}
	]
}
