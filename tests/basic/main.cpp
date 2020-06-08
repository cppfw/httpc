#include "../../src/easyhttp/init_guard.hpp"

#include <utki/debug.hpp>

int main(int argc, char** argv){

	{
		easyhttp::init_guard();
	}

	return 0;
}
