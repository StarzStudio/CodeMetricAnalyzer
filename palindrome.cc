#include <nan.h>
#include <node.h>
#include <string>
using namespace v8;

void IsPalindrome(const FunctionCallbackInfo<Value>& info) 
{
	
  String::Utf8Value sentence(info[0]->ToString()); //convert to std::string
  std::string input = std::string(*sentence); 
  bool isPal = true;

  /* ...  Do Palindrome stuff ... */
  int left = 0;
	int right = input.size() - 1;
	for (int i = 0; i < input.size() / 2; i++)
	{
		if (input[left] != input[right])
			isPal = false;

		left++;
		right--;
	}

  info.GetReturnValue().Set(isPal);  //save output
}

void Init(Local<Object> exports, Local<Object> module) {
  NODE_SET_METHOD(module, "exports", IsPalindrome);
}

NODE_MODULE(addon, Init)