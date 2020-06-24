#include "../../src/easyhttp/util.hpp"

#include <utki/debug.hpp>


int main(int argc, char** argv){
	// test URL escaping
	{
		auto res = easyhttp::escape("Hello World!");
		ASSERT_INFO_ALWAYS(res == "Hello%20World%21", "escaped = " << res)
	}

	return 0;
}
