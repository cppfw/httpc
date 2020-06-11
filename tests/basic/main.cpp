#include "../../src/easyhttp/init_guard.hpp"

#include <utki/debug.hpp>
#include <nitki/semaphore.hpp>

int main(int argc, char** argv){
	{
		nitki::semaphore completed;

		easyhttp::init_guard easyhttp_guard;

		auto r = std::make_shared<easyhttp::request>([&completed](const easyhttp::response& r){
			completed.signal();
		});

		r->set_url("http://izdelie.icu");

		r->start();

		completed.wait();
	}

	return 0;
}
